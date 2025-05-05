// Fill out your copyright notice in the Description page of Project Settings.


#include "S_GameModeBase.h"
#include "S_GameInstance.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "Snake.h"
#include "S_PlayerController.h"
#include "S_GameInstance.h"
#include "S_GameInstanceSubsystem.h"
#include "S_GameHUD.h"
#include "S_GameDataState.h"
#include "S_GameDataSubsystem.h"
#include "SnakePlayerState.h"
#include "AIController.h"
#include "S_AIController.h"

DEFINE_LOG_CATEGORY_STATIC(AS_GameModeBaseLog, All, All);

AS_GameModeBase::AS_GameModeBase()
{
	DefaultPawnClass = ASnake::StaticClass();
	PlayerControllerClass = AS_PlayerController::StaticClass();
	HUDClass = AS_GameHUD::StaticClass();
	GameStateClass = AS_GameDataState::StaticClass();
}

void AS_GameModeBase::SetNumberOfApplesToFinishLevel(int32 Number)
{
	NumberOfApplesToFinishLevel = Number;
	UE_LOG(AS_GameModeBaseLog, Display, TEXT("NumberOfApplesToFinishLevel : %d"), NumberOfApplesToFinishLevel);
}

void AS_GameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UE_LOG(AS_GameModeBaseLog, Display, TEXT("InitGame entry on %s"), *GetName());
}

void AS_GameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(AS_GameModeBaseLog, Display, TEXT("PostLogin"));

	//RestartPlayer(NewPlayer);

	AS_PlayerController* PC = Cast<AS_PlayerController>(NewPlayer);
	if(!IsValid(PC)) return;

	ASnakePlayerState* PS = PC->GetPlayerState<ASnakePlayerState>();
	if (!IsValid(PS)) return;

	if (!IsValid(GetThisGameDataState())) return;

	if (PC->GetLocalPlayer()->GetLocalPlayerIndex() == 0)
	{
		PS->SetControllerType(EGamePlayerControllerType::Keyboard1);
	}
	else
	{
		switch (GetThisGameDataState()->GetGamePlayer())
		{
		case EGamePlayer::Real:
			PS->SetControllerType(EGamePlayerControllerType::Keyboard2);
			break;
		case EGamePlayer::AI:
			PS->SetControllerType(EGamePlayerControllerType::AI);
			break;
		}
	}
	PC->ApplyPlayerStateData();
	PS->OnApplesEatenChanged.AddDynamic(this, &AS_GameModeBase::HandleApplesCountChanged);
}

void AS_GameModeBase::StartPlay()
{
	Super::StartPlay();

	UE_LOG(AS_GameModeBaseLog, Display, TEXT("StartPlay"));

	if (!GetThisGameInstance())
	{
		UE_LOG(AS_GameModeBaseLog, Display, TEXT("GetThisGameInstance not set"));
		return;
	}

	US_GameDataSubsystem* GameDataSubsystem = GetThisGameInstance()->GetSubsystem<US_GameDataSubsystem>();
	if (!GameDataSubsystem)
	{
		UE_LOG(AS_GameModeBaseLog, Display, TEXT("GameDataSubsystem not set"));
		return;
	}

	GameDataSubsystem->LoadGameData();

	if (!GetThisGameDataState()) return;

	FString ErrorMessage;

	UE_LOG(AS_GameModeBaseLog, Display, TEXT("EGameMode %s"), *StaticEnum<EGameMode>()->GetNameStringByValue((int64)GetThisGameDataState()->GetGameMode()));
	switch (GetThisGameDataState()->GetGameMode())
	{
	case EGameMode::Singleplayer:
		break;
	case EGameMode::Cooperative:
		AddSecondPlayer(GetThisGameDataState()->GetGamePlayer(), ErrorMessage);
		break;
	case EGameMode::Competitive:
		AddSecondPlayer(GetThisGameDataState()->GetGamePlayer(), ErrorMessage);
		break;
	}
}

UClass* AS_GameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (InController && InController->IsA<AAIController>())
	{
		return AIPawnClass;
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

bool AS_GameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
	if (PauseSet)
	{
		GetThisGameInstanceSubsystem()->SetGameState(EGameState::Pause);
	}

	return PauseSet;
}

bool AS_GameModeBase::ClearPause()
{
	const auto PauseCleared = Super::ClearPause();
	if (PauseCleared)
	{
		GetThisGameInstanceSubsystem()->SetGameState(EGameState::InProgress);
	}

	return PauseCleared;
}

AActor* AS_GameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UE_LOG(AS_GameModeBaseLog, Display, TEXT("ChoosePlayerStart_Implementation : %s"), *Player->GetName());
	/*AS_PlayerController* PC = Cast<AS_PlayerController>(Player);
	if (!PC)
	{
		UE_LOG(AS_GameModeBaseLog, Warning, TEXT("ChoosePlayerStart: Cast to AS_PlayerController FAILED"));
		return Super::ChoosePlayerStart_Implementation(Player);
	}*/

	int32 Index = PlayerControllers.IndexOfByKey(Player);
	if (Index == INDEX_NONE)
	{
		UE_LOG(AS_GameModeBaseLog, Warning, TEXT("ChoosePlayerStart: Index is INDEX_NONE"));
		PlayerControllers.AddUnique(Player);
		Index = PlayerControllers.IndexOfByKey(Player);
		//return Super::ChoosePlayerStart_Implementation(Player);
	}

	const FName DesiredTag = (Index == 0) ? TEXT("P1") : TEXT("P2");

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		if (It->PlayerStartTag == DesiredTag)
		{
			UE_LOG(AS_GameModeBaseLog, Display, TEXT("It->PlayerStartTag == DesiredTag"));
			UE_LOG(AS_GameModeBaseLog, Display, TEXT("Index : %d | PlayerStart : %s | DesiredTag : %s"), Index, *It->GetName(), *DesiredTag.ToString());
			return *It;
		}
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}

void AS_GameModeBase::StartGame()
{
	UE_LOG(AS_GameModeBaseLog, Display, TEXT("StartGame"));

	if (!IsValid(GetThisGameDataState())) return;
	if (PlayerControllers.IsEmpty()) return;

	OnCalculateApples.Broadcast(GetThisGameDataState()->GetNumberOfApplesToEat(), PlayerControllers.Num());
	
	for (AController* Controller : PlayerControllers)
	{
		AS_AIController* S_AIController = Cast<AS_AIController>(Controller);
		if (S_AIController)
		{
			//S_AIController->ApplyPlayerStateData();
		}
		AS_PlayerController* PC = Cast<AS_PlayerController>(Controller);
		if (PC)
		{
			PC->ApplyPlayerStateData();
		}
	}

	if (!GetThisGameInstanceSubsystem()) return;

	GetThisGameInstanceSubsystem()->SetGameState(EGameState::InProgress);
	
}

void AS_GameModeBase::StartNextLevel()
{
	if (!GetThisGameInstance())
	{
		UE_LOG(AS_GameModeBaseLog, Display, TEXT("GetThisGameInstance not set"));
		return;
	}

	US_GameDataSubsystem* GameDataSubsystem = GetThisGameInstance()->GetSubsystem<US_GameDataSubsystem>();
	if (!GameDataSubsystem)
	{
		UE_LOG(AS_GameModeBaseLog, Display, TEXT("GameDataSubsystem not set"));
		return;
	}

	GameDataSubsystem->SaveGameData();

	for (AController* Controller : PlayerControllers)
	{
		AS_AIController* S_AIController = Cast<AS_AIController>(Controller);
		if (S_AIController)
		{
			//S_AIController->ResetPlayerStateData(); + Snake->ClearBody();
		}
		AS_PlayerController* PC = Cast<AS_PlayerController>(Controller);
		if (PC)
		{
			PC->ResetPlayerStateData();
		}
	}

	ResetPlayer();

	/*for (AS_PlayerController* PC : PlayerControllers)
	{
		if (PC && PC->IsLocalController())
		{
			UE_LOG(AS_GameModeBaseLog, Display, TEXT("RestartPlayer: %s"), *PC->GetName());
			RestartPlayer(PC);
		}
	}*/

	if (!GetThisGameInstanceSubsystem()) return;

	GetThisGameInstanceSubsystem()->SetGameState(EGameState::Init);
}

void AS_GameModeBase::OnChooseMainMenu()
{
	if (!GetThisGameInstance()) return;

	if (GetThisGameInstance()->GetNumLocalPlayers() > 1)
	{
		ULocalPlayer* SecondLP = GetThisGameInstance()->GetLocalPlayerByIndex(1);
		if (SecondLP)
		{
			GetThisGameInstance()->RemoveLocalPlayer(SecondLP);
			UE_LOG(AS_GameModeBaseLog, Display, TEXT("Removed second local player"));
		}
	}
	PlayerControllers.Empty();
	NextLevel = EGameLevel::MenuLevel;

	GetThisGameInstance()->SetNextLevel(NextLevel);
}

void AS_GameModeBase::Dead(AController* DeadController)
{
	if (!GetThisGameInstanceSubsystem()) return;
	if (!GetThisGameInstance()) return;

	if (GetThisGameInstanceSubsystem()->GetGameState() == EGameState::Death) return;

	for (auto Controller : PlayerControllers)
	{
		APlayerController* PC = Cast<APlayerController>(Controller);
		if (IsValid(PC))
		{
			if (ASnake* Snake = Cast<ASnake>(PC->GetPawn()))
			{
				UE_LOG(AS_GameModeBaseLog, Display, TEXT("Snake: %s"), *Snake->GetName());
				Snake->SetDirection(EDirectionState::None);
				Snake->DisableInput(PC);
			}
		}
		// stop behaviour tree
	}
	GetThisGameInstanceSubsystem()->SetGameState(EGameState::Death);
	OnPlayerDied.Broadcast(DeadController);

	UE_LOG(AS_GameModeBaseLog, Display, TEXT("AS_GameModeBase::Dead GetCurrentLevel: %s"),
		*StaticEnum<EGameLevel>()->GetNameStringByValue((int64)GetThisGameInstance()->GetCurrentLevel()));

	GetThisGameInstance()->SetNextLevel(GetThisGameInstance()->GetCurrentLevel());
}

void AS_GameModeBase::GameOver(AController* Winner)
{
	if (!GetThisGameInstance()) return;
	if (GetThisGameInstanceSubsystem()->GetGameState() == EGameState::GameOver) return;

	switch (GetThisGameInstance()->GetCurrentLevel())
	{
		case EGameLevel::Level1:
			GetThisGameInstanceSubsystem()->SetGameState(EGameState::GameOver);
			NextLevel = EGameLevel::Level2;
			break;
		case EGameLevel::Level2:
			GetThisGameInstanceSubsystem()->SetGameState(EGameState::GameOver);
			NextLevel = EGameLevel::Level3;
			break;
		case EGameLevel::Level3:
			GetThisGameInstanceSubsystem()->SetGameState(EGameState::GameOver);
			NextLevel = EGameLevel::Level4;
			break;
		case EGameLevel::Level4:
			GetThisGameInstanceSubsystem()->SetGameState(EGameState::GameOver);
			NextLevel = EGameLevel::Level5;
			break;
		case EGameLevel::Level5:
			GetThisGameInstanceSubsystem()->SetGameState(EGameState::GameOver);
			NextLevel = EGameLevel::MenuLevel;
			break;
	}

	OnPlayerWon.Broadcast(Winner);

	for (auto Controller : PlayerControllers)
	{
		APlayerController* PC = Cast<APlayerController>(Controller);
		if (IsValid(PC))
		{
			if (ASnake* Snake = Cast<ASnake>(PC->GetPawn()))
			{
				UE_LOG(AS_GameModeBaseLog, Display, TEXT("Snake: %s"), *Snake->GetName());
				Snake->SetDirection(EDirectionState::None);
				Snake->DisableInput(PC);
			}
		}
		// stop behaviour tree
	}

	GetThisGameInstance()->SetNextLevel(NextLevel);
}

TArray<AController*> AS_GameModeBase::GetPlayerControllers()
{
	return PlayerControllers;
}

void AS_GameModeBase::SetGridManager(AS_GridManager* Manager)
{
	if(!Manager) return;

	GridManager = Manager;
}

AS_GridManager* AS_GameModeBase::GetGridManager() const
{
	return GridManager;
}

void AS_GameModeBase::HandleApplesCountChanged(int32 NewCount)
{
	if (!GetThisGameDataState()) return;

	TMap<AController*, int32> PlayersScore;
	for (AController* Controller : PlayerControllers)
	{
		if (!IsValid(Controller)) continue;
		ASnakePlayerState* PS = Controller->GetPlayerState<ASnakePlayerState>();
		if (!IsValid(PS)) continue;

		if (!PlayersScore.Contains(Controller))
			PlayersScore.Add(Controller, PS->GetApplesEatenCount());
	}

	AController* Winner = nullptr;

	switch (GetThisGameDataState()->GetGameMode())
	{
		case EGameMode::Singleplayer:
			{
				if (PlayerControllers.Num() > 0)
				{
					AController* Controller = PlayerControllers[0];
					if (PlayersScore.Contains(Controller) && PlayersScore[Controller] >= NumberOfApplesToFinishLevel)
					{
						Winner = Controller;
					}
				}
			}
			break;
		case EGameMode::Cooperative:
			{
				int32 Total = 0;
				for (auto& Pair : PlayersScore)
				{
					Total += Pair.Value;
				}
				if (Total >= NumberOfApplesToFinishLevel)
				{
					//Winner = nullptr;
					GameOver(Winner);
				}
			}
			break;
		case EGameMode::Competitive:
			{
				int32 BestScore = -1;
				for (auto& Pair : PlayersScore)
				{
					if (Pair.Value >= NumberOfApplesToFinishLevel && Pair.Value > BestScore)
					{
						BestScore = Pair.Value;
						Winner = Pair.Key;
					}
				}
			}
			break;
	}

	if (Winner != nullptr)
	{
		UE_LOG(AS_GameModeBaseLog, Display, TEXT("Winner : %s"), *Winner->GetName());
		/*APlayerController* PC = Cast<APlayerController>(Winner);
		if(!IsValid(PC)) return;

		UE_LOG(AS_GameModeBaseLog, Display, TEXT("Winner PC : %s"), *PC->GetName());*/
		GameOver(Winner);
	}
}

void AS_GameModeBase::AddSecondPlayer(EGamePlayer Player, FString& ErrorMessage)
{
	switch (Player)
	{
		case EGamePlayer::Real:
			UE_LOG(AS_GameModeBaseLog, Display, TEXT("CreateLocalPlayer"));
			GetGameInstance()->CreateLocalPlayer(1, ErrorMessage, true);
			break;
		case EGamePlayer::AI:
			SpawnAISnake();
			break;
	}
}

US_GameInstanceSubsystem* AS_GameModeBase::GetThisGameInstanceSubsystem() const
{
	if (!GetThisGameInstance()) return nullptr;

	US_GameInstanceSubsystem* GameInstanceSubsystem = GetThisGameInstance()->GetSubsystem<US_GameInstanceSubsystem>();
	if (!GameInstanceSubsystem) return nullptr;

	return GameInstanceSubsystem;
}

US_GameInstance* AS_GameModeBase::GetThisGameInstance() const
{
	US_GameInstance* GameInstance = Cast<US_GameInstance>(GetGameInstance());
	if (!GameInstance) return nullptr;

	return GameInstance;
}

AS_GameDataState* AS_GameModeBase::GetThisGameDataState() const
{
	if (!GetWorld()) return nullptr;

	AS_GameDataState* GameDataState = GetWorld()->GetGameState<AS_GameDataState>();
	if (!GameDataState) return nullptr;

	return GameDataState;
}

void AS_GameModeBase::ResetPlayer()
{
	if (!GetWorld()) return;

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		if (It->Get() && It->Get()->GetPawn()/* && It->Get()->IsPlayerController()*/)
		{
			It->Get()->GetPawn()->Reset();
			RestartPlayer(It->Get());
		}
	}
}

void AS_GameModeBase::SpawnAISnake()
{
	if (!GetWorld()) return;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AAIController* S_AIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
	if(!S_AIController) return;

	RestartPlayer(S_AIController);
}

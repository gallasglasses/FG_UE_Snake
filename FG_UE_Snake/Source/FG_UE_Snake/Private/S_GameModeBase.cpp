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

DEFINE_LOG_CATEGORY_STATIC(AS_GameModeBaseLog, All, All);

AS_GameModeBase::AS_GameModeBase()
{
	DefaultPawnClass = ASnake::StaticClass();
	PlayerControllerClass = AS_PlayerController::StaticClass();
	HUDClass = AS_GameHUD::StaticClass();
	GameStateClass = AS_GameDataState::StaticClass();
}

void AS_GameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UE_LOG(AS_GameModeBaseLog, Display, TEXT("InitGame entry on %s"), *GetName());
}

void AS_GameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

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
	PlayerControllers.Add(PC);

	/*US_GameInstance* GameInstance = Cast<US_GameInstance>(GetGameInstance());
	if (!GameInstance) return;
	FGameSettings Settings = GameInstance->GetGameSettings();

	FVector SpawnPosition = (NewPlayer->GetLocalPlayer()->GetControllerId() == 0) 
	? FVector(Settings.FirstPlayerPosition.X * Settings.TileSize, Settings.FirstPlayerPosition.Y * Settings.TileSize, Settings.TileSize)
	: FVector(Settings.SecondPlayerPosition.X * Settings.TileSize, Settings.SecondPlayerPosition.Y * Settings.TileSize, Settings.TileSize);

	ASnake* SnakePawn = GetWorld()->SpawnActor<ASnake>(DefaultPawnClass, SpawnPosition, FRotator::ZeroRotator);
	NewPlayer->Possess(SnakePawn);

	SnakePawn->bIsFirstPlayer = (NewPlayer->GetLocalPlayer()->GetControllerId() == 0);*/
}

void AS_GameModeBase::StartPlay()
{
	Super::StartPlay();
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
	APlayerController* PC = Cast<APlayerController>(Player);
	if (!PC || !PC->GetLocalPlayer())
	{
		return Super::ChoosePlayerStart_Implementation(Player);
	}

	if (!GetThisGameInstance())
	{
		return Super::ChoosePlayerStart_Implementation(Player);
	}

	const bool bIsFirst = (PC->GetLocalPlayer()->GetControllerId() == 0);
	const FName DesiredTag = bIsFirst ? TEXT("P1") : TEXT("P2");

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		if (It->PlayerStartTag == DesiredTag)
		{
			return *It;
		}
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}

void AS_GameModeBase::StartGame()
{
	UE_LOG(AS_GameModeBaseLog, Display, TEXT("StartGame"));

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


	if (!GetThisGameInstanceSubsystem()) return;

	GetThisGameInstanceSubsystem()->SetGameState(EGameState::InProgress);

	/*for (AS_PlayerController* PC : PlayerControllers)
	{
		RestartPlayer(PC);
	}*/
}

void AS_GameModeBase::Dead()
{
	if (!GetThisGameInstanceSubsystem()) return;

	GetThisGameInstanceSubsystem()->SetGameState(EGameState::Death);

	if (!GetThisGameInstance()) return;

	GetThisGameInstance()->SetNextLevel(EGameLevel::MenuLevel);
}

void AS_GameModeBase::GameOver()
{
	for (auto Pawn : TActorRange<APawn>(GetWorld()))
	{
		if (Pawn)
		{
			Pawn->TurnOff();
			Pawn->DisableInput(nullptr);
		}
	}

	if (!GetThisGameInstance()) return;

	switch (GetThisGameInstance()->GetCurrentLevel())
	{
		case EGameLevel::Level1:
			NextLevel = EGameLevel::Level2;
			break;
		case EGameLevel::Level2:
			NextLevel = EGameLevel::Level3;
			break;
		case EGameLevel::Level3:
			NextLevel = EGameLevel::Level4;
			break;
		case EGameLevel::Level4:
			NextLevel = EGameLevel::Level5;
			break;
		case EGameLevel::Level5:
			GetThisGameInstanceSubsystem()->SetGameState(EGameState::GameOver);
			NextLevel = EGameLevel::MenuLevel;
			break;
	}
	GetThisGameInstance()->SetNextLevel(NextLevel);
}

TArray<AS_PlayerController*> AS_GameModeBase::GetPlayerControllers()
{
	return PlayerControllers;
}

void AS_GameModeBase::AddSecondPlayer(EGamePlayer Player, FString& ErrorMessage)
{
	switch (Player)
	{
		case EGamePlayer::Real:
			GetGameInstance()->CreateLocalPlayer(1, ErrorMessage, true);
			break;
		case EGamePlayer::AI:

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

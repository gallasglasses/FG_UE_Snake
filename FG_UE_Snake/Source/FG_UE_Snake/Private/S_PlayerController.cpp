// Fill out your copyright notice in the Description page of Project Settings.


#include "S_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Snake.h"
#include "S_GameModeBase.h"
#include "SnakePlayerState.h"
#include "S_GameModeBase.h"
#include "S_GameInstance.h"
#include "S_GameInstanceSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(AS_PlayerControllerLog, All, All);

void AS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	/*if (!IsValid(GetWorld())) return;

	US_GameInstance* GameInstance = Cast<US_GameInstance>(GetGameInstance());
	if (!GameInstance) return;

	US_GameInstanceSubsystem* GameInstanceSubsystem = GameInstance->GetSubsystem<US_GameInstanceSubsystem>();
	if (!GameInstanceSubsystem) return;

	GameInstanceSubsystem->OnGameStateChanged.AddDynamic(this, &AS_PlayerController::OnGameStateChanged);
	

	Snake = Cast<ASnake>(GetPawn());
	if (!IsValid(Snake))
	{
		UE_LOG(AS_PlayerControllerLog, Display, TEXT("!IsValid(Snake) %s"), *GetName());
		return;
	}

	UE_LOG(AS_PlayerControllerLog, Display, TEXT("%s OnDead.AddUObject %s"), *Snake.GetName(), *GetName());
	Snake->OnDead.AddUObject(this, &AS_PlayerController::OnDead);

	SnakePlayerState = Cast<ASnakePlayerState>(PlayerState);
	if (!SnakePlayerState) return;

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

    if (!IsValid(Subsystem) || !FirstPlayerMappingContext || !SecondPlayerMappingContext) return;

    Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(FirstPlayerMappingContext, 0);
	Subsystem->AddMappingContext(SecondPlayerMappingContext, 0);*/
}

void AS_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	Snake = Cast<ASnake>(aPawn);
	SnakePlayerState = Cast<ASnakePlayerState>(PlayerState);

	if (Snake)
	{
		UE_LOG(AS_PlayerControllerLog, Display, TEXT("%s possessed %s"), *GetName(), *Snake->GetName());

		Snake->OnDead.AddDynamic(this, &AS_PlayerController::OnDead);
	}
	else
	{
		UE_LOG(AS_PlayerControllerLog, Warning, TEXT("%s possessed something that's not ASnake"), *GetName());
	}

	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->ClearAllMappings();

			if (FirstPlayerMappingContext)
				Subsystem->AddMappingContext(FirstPlayerMappingContext, 0);
			if (SecondPlayerMappingContext)
				Subsystem->AddMappingContext(SecondPlayerMappingContext, 0);

			UE_LOG(AS_PlayerControllerLog, Display, TEXT("Added MappingContexts: %s and %s"),*FirstPlayerMappingContext.GetName(), *SecondPlayerMappingContext.GetName());
		}
	}

	if (UWorld* World = GetWorld())
	{
		if (US_GameInstance* GI = Cast<US_GameInstance>(World->GetGameInstance()))
		{
			if (US_GameInstanceSubsystem* Subsys = GI->GetSubsystem<US_GameInstanceSubsystem>())
			{
				Subsys->OnGameStateChanged.AddDynamic(this, &AS_PlayerController::OnGameStateChanged);
			}
		}
	}
}

void AS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UE_LOG(AS_PlayerControllerLog, Display, TEXT("SetupInputComponent called on %s"), *GetName());

	if (!IsValid(FirstPlayerMoveUpAction) || !IsValid(FirstPlayerMoveRightAction) || !IsValid(FirstPlayerMoveDownAction) || !IsValid(FirstPlayerMoveLeftAction) || !IsValid(FirstPlayerLookAction) ||
		!IsValid(SecondPlayerMoveUpAction) || !IsValid(SecondPlayerMoveRightAction) || !IsValid(SecondPlayerMoveDownAction) || !IsValid(SecondPlayerMoveLeftAction) || !IsValid(SecondPlayerLookAction))
	{
		UE_LOG(AS_PlayerControllerLog, Display, TEXT("Actions not set %s"), *GetName());
		return;
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(FirstPlayerMoveUpAction, ETriggerEvent::Triggered, this, &AS_PlayerController::FirstPlayerMoveUp);
		EnhancedInputComponent->BindAction(FirstPlayerMoveRightAction, ETriggerEvent::Triggered, this, &AS_PlayerController::FirstPlayerMoveRight);
		EnhancedInputComponent->BindAction(FirstPlayerMoveDownAction, ETriggerEvent::Triggered, this, &AS_PlayerController::FirstPlayerMoveDown);
		EnhancedInputComponent->BindAction(FirstPlayerMoveLeftAction, ETriggerEvent::Triggered, this, &AS_PlayerController::FirstPlayerMoveLeft);

		EnhancedInputComponent->BindAction(FirstPlayerLookAction, ETriggerEvent::Triggered, this, &AS_PlayerController::FirstPlayerLook);
		
		EnhancedInputComponent->BindAction(SecondPlayerMoveUpAction, ETriggerEvent::Triggered, this, &AS_PlayerController::SecondPlayerMoveUp);
		EnhancedInputComponent->BindAction(SecondPlayerMoveRightAction, ETriggerEvent::Triggered, this, &AS_PlayerController::SecondPlayerMoveRight);
		EnhancedInputComponent->BindAction(SecondPlayerMoveDownAction, ETriggerEvent::Triggered, this, &AS_PlayerController::SecondPlayerMoveDown);
		EnhancedInputComponent->BindAction(SecondPlayerMoveLeftAction, ETriggerEvent::Triggered, this, &AS_PlayerController::SecondPlayerMoveLeft);

		EnhancedInputComponent->BindAction(SecondPlayerLookAction, ETriggerEvent::Triggered, this, &AS_PlayerController::SecondPlayerLook);
	}
}

void AS_PlayerController::FirstPlayerMoveUp(const FInputActionValue& Value)
{
	//UE_LOG(AS_PlayerControllerLog, Display, TEXT("FirstPlayerMoveUp fired with Value=%s"), *Value.Get<FVector2D>().ToString());
	if (ASnake* PawnByKeyboard = GetPawnByKeyboard(0))
	{
		PawnByKeyboard->SetDirection(EDirectionState::Up);
	}
}

void AS_PlayerController::FirstPlayerMoveRight(const FInputActionValue& Value)
{
	if (ASnake* PawnByKeyboard = GetPawnByKeyboard(0))
	{
		PawnByKeyboard->SetDirection(EDirectionState::Right);
	}
}

void AS_PlayerController::FirstPlayerMoveDown(const FInputActionValue& Value)
{
	if (ASnake* PawnByKeyboard = GetPawnByKeyboard(0))
	{
		PawnByKeyboard->SetDirection(EDirectionState::Down);
	}
}

void AS_PlayerController::FirstPlayerMoveLeft(const FInputActionValue& Value)
{
	if (ASnake* PawnByKeyboard = GetPawnByKeyboard(0))
	{
		PawnByKeyboard->SetDirection(EDirectionState::Left);
	}
}

void AS_PlayerController::FirstPlayerLook(const FInputActionValue& Value)
{
	if (ASnake* PawnByKeyboard = GetPawnByKeyboard(0))
	{

	}
}

void AS_PlayerController::SecondPlayerMoveUp(const FInputActionValue& Value)
{
	if (ASnake* PawnByKeyboard = GetPawnByKeyboard(1))
	{
		PawnByKeyboard->SetDirection(EDirectionState::Up);
	}
}

void AS_PlayerController::SecondPlayerMoveRight(const FInputActionValue& Value)
{
	if (ASnake* PawnByKeyboard = GetPawnByKeyboard(1))
	{
		PawnByKeyboard->SetDirection(EDirectionState::Right);
	}
}

void AS_PlayerController::SecondPlayerMoveDown(const FInputActionValue& Value)
{
	if (ASnake* PawnByKeyboard = GetPawnByKeyboard(1))
	{
		PawnByKeyboard->SetDirection(EDirectionState::Down);
	}
}

void AS_PlayerController::SecondPlayerMoveLeft(const FInputActionValue& Value)
{
	if (ASnake* PawnByKeyboard = GetPawnByKeyboard(1))
	{
		PawnByKeyboard->SetDirection(EDirectionState::Left);
	}
}

void AS_PlayerController::SecondPlayerLook(const FInputActionValue& Value)
{
	if (ASnake* PawnByKeyboard = GetPawnByKeyboard(1))
	{
		
	}
}

void AS_PlayerController::OnPauseGame()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
	const auto GameMode = Cast<AS_GameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->SetPause(this);
	}
}

ASnake* AS_PlayerController::GetPawnByKeyboard(int32 KeyboardID)
{
	if (GetWorld())
	{
		const auto GameMode = Cast<AS_GameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			TArray<AS_PlayerController*> Controllers = GameMode->GetPlayerControllers();

			for (AS_PlayerController* PC : Controllers)
			{
				EGamePlayerControllerType Type = PC->SnakePlayerState->GetControllerType();
				if ( (Type == EGamePlayerControllerType::Keyboard1 && KeyboardID == static_cast<uint8>(Type)) ||
					 (Type == EGamePlayerControllerType::Keyboard2 && KeyboardID == static_cast<uint8>(Type)))
				{
					return PC->Snake;
				}
			}
		}
	}
	return nullptr;
}

bool AS_PlayerController::SetPause(bool bPause, FCanUnpause CanUnpauseDelegate)
{
	const auto PauseSet = Super::SetPause(bPause, CanUnpauseDelegate);
	return PauseSet;
}

void AS_PlayerController::OnGameStateChanged(EGameState PreviousState, EGameState NextState)
{
	if (NextState == EGameState::InProgress)
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;
	}
}

void AS_PlayerController::OnDead(ASnake* DeadSnake)
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
	const auto GameMode = Cast<AS_GameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->Dead();
	}
}

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
	if (Snake && SnakePlayerState)
	{
		Snake->OnAppleEaten.AddDynamic(SnakePlayerState, &ASnakePlayerState::OnAppleCountChanged);
	}

	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		UE_LOG(AS_PlayerControllerLog, Display, TEXT("%s | Local Player: %s"), *GetName(), *LP->GetName());
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
				if (!Subsys->OnGameStateChanged.IsAlreadyBound(this, &AS_PlayerController::OnGameStateChanged))
				{
					Subsys->OnGameStateChanged.AddDynamic(this, &AS_PlayerController::OnGameStateChanged);
				}
			}
		}
	}
}

void AS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UE_LOG(AS_PlayerControllerLog, Display, TEXT("SetupInputComponent called on %s"), *GetName());

	if (!IsValid(FirstPlayerMoveUpAction) || !IsValid(FirstPlayerMoveRightAction) || !IsValid(FirstPlayerMoveDownAction) || !IsValid(FirstPlayerMoveLeftAction) || 
		!IsValid(FirstPlayerLookUpAction) || !IsValid(FirstPlayerLookRightAction) || !IsValid(FirstPlayerLookDownAction) || !IsValid(FirstPlayerLookLeftAction) ||
		!IsValid(SecondPlayerMoveUpAction) || !IsValid(SecondPlayerMoveRightAction) || !IsValid(SecondPlayerMoveDownAction) || !IsValid(SecondPlayerMoveLeftAction) ||
		!IsValid(SecondPlayerLookUpAction) || !IsValid(SecondPlayerLookRightAction) || !IsValid(SecondPlayerLookDownAction) || !IsValid(SecondPlayerLookLeftAction) ||
		!IsValid(FirstPlayerShiftAction) || !IsValid(SecondPlayerShiftAction))
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

		EnhancedInputComponent->BindAction(FirstPlayerShiftAction, ETriggerEvent::Started, this, &AS_PlayerController::FirstPlayerShiftPressed);
		EnhancedInputComponent->BindAction(FirstPlayerShiftAction, ETriggerEvent::Completed, this, &AS_PlayerController::FirstPlayerShiftReleased);
		EnhancedInputComponent->BindAction(FirstPlayerLookUpAction, ETriggerEvent::Triggered, this, &AS_PlayerController::FirstPlayerLookUp);
		EnhancedInputComponent->BindAction(FirstPlayerLookRightAction, ETriggerEvent::Triggered, this, &AS_PlayerController::FirstPlayerLookRight);
		EnhancedInputComponent->BindAction(FirstPlayerLookDownAction, ETriggerEvent::Triggered, this, &AS_PlayerController::FirstPlayerLookDown);
		EnhancedInputComponent->BindAction(FirstPlayerLookLeftAction, ETriggerEvent::Triggered, this, &AS_PlayerController::FirstPlayerLookLeft);
		
		EnhancedInputComponent->BindAction(SecondPlayerMoveUpAction, ETriggerEvent::Triggered, this, &AS_PlayerController::SecondPlayerMoveUp);
		EnhancedInputComponent->BindAction(SecondPlayerMoveRightAction, ETriggerEvent::Triggered, this, &AS_PlayerController::SecondPlayerMoveRight);
		EnhancedInputComponent->BindAction(SecondPlayerMoveDownAction, ETriggerEvent::Triggered, this, &AS_PlayerController::SecondPlayerMoveDown);
		EnhancedInputComponent->BindAction(SecondPlayerMoveLeftAction, ETriggerEvent::Triggered, this, &AS_PlayerController::SecondPlayerMoveLeft);

		EnhancedInputComponent->BindAction(SecondPlayerShiftAction, ETriggerEvent::Started, this, &AS_PlayerController::SecondPlayerShiftPressed);
		EnhancedInputComponent->BindAction(SecondPlayerShiftAction, ETriggerEvent::Completed, this, &AS_PlayerController::SecondPlayerShiftReleased);
		EnhancedInputComponent->BindAction(SecondPlayerLookUpAction, ETriggerEvent::Triggered, this, &AS_PlayerController::SecondPlayerLookUp);
		EnhancedInputComponent->BindAction(SecondPlayerLookRightAction, ETriggerEvent::Triggered, this, &AS_PlayerController::SecondPlayerLookRight);
		EnhancedInputComponent->BindAction(SecondPlayerLookDownAction, ETriggerEvent::Triggered, this, &AS_PlayerController::SecondPlayerLookDown);
		EnhancedInputComponent->BindAction(SecondPlayerLookLeftAction, ETriggerEvent::Triggered, this, &AS_PlayerController::SecondPlayerLookLeft);
	}
}

void AS_PlayerController::FirstPlayerMoveUp(const FInputActionValue& Value)
{
	//UE_LOG(AS_PlayerControllerLog, Display, TEXT("FirstPlayerMoveUp %s fired with Value=%s"), *this->GetName(), *Value.Get<FVector2D>().ToString());
	if (!bIsInputModeUI)
	{
		if (bIsFirstPlayerShiftPressed) return;
		if (ASnake* PawnByKeyboard = GetPawnByKeyboard(0))
		{
			PawnByKeyboard->SetDirection(EDirectionState::Up);
		}
	}
}

void AS_PlayerController::FirstPlayerMoveRight(const FInputActionValue& Value)
{
	//UE_LOG(AS_PlayerControllerLog, Display, TEXT("FirstPlayerMoveRight %s fired with Value=%s"), *this->GetName(), *Value.Get<FVector2D>().ToString());
	if (!bIsInputModeUI)
	{
		if (bIsFirstPlayerShiftPressed) return;
		if (ASnake* PawnByKeyboard = GetPawnByKeyboard(0))
		{
			PawnByKeyboard->SetDirection(EDirectionState::Right);
		}
	}
}

void AS_PlayerController::FirstPlayerMoveDown(const FInputActionValue& Value)
{
	//UE_LOG(AS_PlayerControllerLog, Display, TEXT("FirstPlayerMoveDown %s fired with Value=%s"), *this->GetName(), *Value.Get<FVector2D>().ToString());
	if (!bIsInputModeUI)
	{
		if (bIsFirstPlayerShiftPressed) return;
		if (ASnake* PawnByKeyboard = GetPawnByKeyboard(0))
		{
			PawnByKeyboard->SetDirection(EDirectionState::Down);
		}
	}
}

void AS_PlayerController::FirstPlayerMoveLeft(const FInputActionValue& Value)
{
	//UE_LOG(AS_PlayerControllerLog, Display, TEXT("FirstPlayerMoveLeft %s fired with Value=%s"), *this->GetName(), *Value.Get<FVector2D>().ToString());
	if (!bIsInputModeUI)
	{
		if (bIsFirstPlayerShiftPressed) return;
		if (ASnake* PawnByKeyboard = GetPawnByKeyboard(0))
		{
			PawnByKeyboard->SetDirection(EDirectionState::Left);
		}
	}
}

void AS_PlayerController::FirstPlayerShiftPressed(const FInputActionValue& Value)
{
	//bIsFirstPlayerShiftPressed = Value.Get<bool>();
	bIsFirstPlayerShiftPressed = true;
	UE_LOG(AS_PlayerControllerLog, Display, TEXT("%s FirstPlayerShiftPressed: %s"), *GetName(), bIsFirstPlayerShiftPressed ? TEXT("TRUE") : TEXT("FALSE"));
}

void AS_PlayerController::FirstPlayerShiftReleased(const FInputActionValue& Value)
{
	bIsFirstPlayerShiftPressed = false;
	UE_LOG(AS_PlayerControllerLog, Display, TEXT("%s FirstPlayerShiftReleased: %s"), *GetName(), bIsFirstPlayerShiftPressed ? TEXT("TRUE") : TEXT("FALSE"));
}

void AS_PlayerController::FirstPlayerLookUp(const FInputActionValue& Value)
{
	//UE_LOG(AS_PlayerControllerLog, Display, TEXT("FirstPlayerLookUp %s fired with Value=%s"), *this->GetName(), *Value.Get<FVector2D>().ToString());
	if (!bIsInputModeUI)
	{
		if (ASnake* PawnByKeyboard = GetPawnByKeyboard(0))
		{
			PawnByKeyboard->RotateCamera(EDirectionState::Up);
		}
	}
}

void AS_PlayerController::FirstPlayerLookRight(const FInputActionValue& Value)
{
	if (!bIsInputModeUI)
	{
		if (ASnake* PawnByKeyboard = GetPawnByKeyboard(0))
		{
			PawnByKeyboard->RotateCamera(EDirectionState::Right);
		}
	}
}

void AS_PlayerController::FirstPlayerLookDown(const FInputActionValue& Value)
{
	if (!bIsInputModeUI)
	{
		if (ASnake* PawnByKeyboard = GetPawnByKeyboard(0))
		{
			PawnByKeyboard->RotateCamera(EDirectionState::Down);
		}
	}
}

void AS_PlayerController::FirstPlayerLookLeft(const FInputActionValue& Value)
{
	if (!bIsInputModeUI)
	{
		if (ASnake* PawnByKeyboard = GetPawnByKeyboard(0))
		{
			PawnByKeyboard->RotateCamera(EDirectionState::Left);
		}
	}
}

void AS_PlayerController::SecondPlayerMoveUp(const FInputActionValue& Value)
{
	//UE_LOG(AS_PlayerControllerLog, Display, TEXT("SecondPlayerMoveUp %s fired with Value=%s"), *this->GetName(), *Value.Get<FVector2D>().ToString());
	if (!bIsInputModeUI)
	{
		if (bIsSecondPlayerShiftPressed) return;
		if (ASnake* PawnByKeyboard = GetPawnByKeyboard(1))
		{
			PawnByKeyboard->SetDirection(EDirectionState::Up);
		}
	}
}

void AS_PlayerController::SecondPlayerMoveRight(const FInputActionValue& Value)
{
	//UE_LOG(AS_PlayerControllerLog, Display, TEXT("SecondPlayerMoveRight %s fired with Value=%s"), *this->GetName(), *Value.Get<FVector2D>().ToString());
	if (!bIsInputModeUI)
	{
		if (bIsSecondPlayerShiftPressed) return;
		if (ASnake* PawnByKeyboard = GetPawnByKeyboard(1))
		{
			PawnByKeyboard->SetDirection(EDirectionState::Right);
		}
	}
}

void AS_PlayerController::SecondPlayerMoveDown(const FInputActionValue& Value)
{
	//UE_LOG(AS_PlayerControllerLog, Display, TEXT("SecondPlayerMoveDown %s fired with Value=%s"), *this->GetName(), *Value.Get<FVector2D>().ToString());
	if (!bIsInputModeUI)
	{
		if (bIsSecondPlayerShiftPressed) return;
		if (ASnake* PawnByKeyboard = GetPawnByKeyboard(1))
		{
			PawnByKeyboard->SetDirection(EDirectionState::Down);
		}
	}
}

void AS_PlayerController::SecondPlayerMoveLeft(const FInputActionValue& Value)
{
	//UE_LOG(AS_PlayerControllerLog, Display, TEXT("SecondPlayerMoveLeft %s fired with Value=%s"), *this->GetName(), *Value.Get<FVector2D>().ToString());
	if (!bIsInputModeUI)
	{
		if (bIsSecondPlayerShiftPressed) return;
		/*if (IsInputKeyDown(EKeys::LeftShift) || IsInputKeyDown(EKeys::RightShift))
			return;*/
		if (ASnake* PawnByKeyboard = GetPawnByKeyboard(1))
		{
			PawnByKeyboard->SetDirection(EDirectionState::Left);
		}
	}
}

void AS_PlayerController::SecondPlayerShiftPressed(const FInputActionValue& Value)
{
	//bIsSecondPlayerShiftPressed = Value.Get<bool>();
	bIsSecondPlayerShiftPressed = true;
	UE_LOG(AS_PlayerControllerLog, Display, TEXT("%s SecondPlayerShiftPressed: %s"), *GetName(), bIsSecondPlayerShiftPressed ? TEXT("TRUE") : TEXT("FALSE"));
}

void AS_PlayerController::SecondPlayerShiftReleased(const FInputActionValue& Value)
{
	bIsSecondPlayerShiftPressed = false;
	UE_LOG(AS_PlayerControllerLog, Display, TEXT("%s SecondPlayerShiftReleased: %s"), *GetName(), bIsSecondPlayerShiftPressed ? TEXT("TRUE") : TEXT("FALSE"));
}

void AS_PlayerController::SecondPlayerLookUp(const FInputActionValue& Value)
{
	//UE_LOG(AS_PlayerControllerLog, Display, TEXT("SecondPlayerLookUp %s fired with Value=%s"), *this->GetName(), *Value.Get<FVector2D>().ToString());
	if (!bIsInputModeUI)
	{
		if (ASnake* PawnByKeyboard = GetPawnByKeyboard(1))
		{
			PawnByKeyboard->RotateCamera(EDirectionState::Up);
		}
	}
}

void AS_PlayerController::SecondPlayerLookRight(const FInputActionValue& Value)
{
	//UE_LOG(AS_PlayerControllerLog, Display, TEXT("SecondPlayerLookRight %s fired with Value=%s"), *this->GetName(), *Value.Get<FVector2D>().ToString());
	if (!bIsInputModeUI)
	{
		if (ASnake* PawnByKeyboard = GetPawnByKeyboard(1))
		{
			PawnByKeyboard->RotateCamera(EDirectionState::Right);
		}
	}
}

void AS_PlayerController::SecondPlayerLookDown(const FInputActionValue& Value)
{
	//UE_LOG(AS_PlayerControllerLog, Display, TEXT("SecondPlayerLookDown %s fired with Value=%s"), *this->GetName(), *Value.Get<FVector2D>().ToString());
	if (!bIsInputModeUI)
	{
		if (ASnake* PawnByKeyboard = GetPawnByKeyboard(1))
		{
			PawnByKeyboard->RotateCamera(EDirectionState::Down);
		}
	}
}

void AS_PlayerController::SecondPlayerLookLeft(const FInputActionValue& Value)
{
	//UE_LOG(AS_PlayerControllerLog, Display, TEXT("SecondPlayerLookLeft %s fired with Value=%s"), *this->GetName(), *Value.Get<FVector2D>().ToString());
	if (!bIsInputModeUI)
	{
		if (ASnake* PawnByKeyboard = GetPawnByKeyboard(1))
		{
			PawnByKeyboard->RotateCamera(EDirectionState::Left);
		}
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
			TArray<AController*> Controllers = GameMode->GetPlayerControllers();

			for (AController* Controller : Controllers)
			{
				if (AS_PlayerController* PC = Cast<AS_PlayerController>(Controller))
				{
					EGamePlayerControllerType Type = PC->SnakePlayerState->GetControllerType();
					if ((Type == EGamePlayerControllerType::Keyboard1 && KeyboardID == static_cast<uint8>(Type)) ||
						(Type == EGamePlayerControllerType::Keyboard2 && KeyboardID == static_cast<uint8>(Type)))
					{
						return PC->Snake;
					}
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
		bIsInputModeUI = false;
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(FInputModeGameAndUI()); //FInputModeUIOnly
		bIsInputModeUI = true;
		bShowMouseCursor = true;
	}
}

void AS_PlayerController::OnDead(AController* FallenController)
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

	const auto GameMode = Cast<AS_GameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->Dead(FallenController);
	}
}

void AS_PlayerController::ResetPlayerStateData()
{
	SnakePlayerState->ResetAppleCount();
	Snake->ClearBody();
}

void AS_PlayerController::ApplyPlayerStateData()
{
	if (UMaterialInterface** FoundMat = SnakeMaterials.Find(SnakePlayerState->GetControllerType()))
	{
		Snake->ChangeMeshMaterial(*FoundMat);
	}
	else
	{
		UE_LOG(AS_PlayerControllerLog, Warning, TEXT("Key not found in SnakeMaterials"));
	}

	switch (SnakePlayerState->GetControllerType())
	{
		case EGamePlayerControllerType::Keyboard1:
			Snake->SetMaterialParameterNames(FName("FirstPlayerLocation"), FName("FirstCameraLocation"));
			break;
		case EGamePlayerControllerType::Keyboard2:
			Snake->SetMaterialParameterNames(FName("SecondPlayerLocation"), FName("SecondCameraLocation"));
			break;
		case EGamePlayerControllerType::AI:
			Snake->SetMaterialParameterNames(FName("SecondPlayerLocation"), FName("SecondCameraLocation"));
			break;
	}
}

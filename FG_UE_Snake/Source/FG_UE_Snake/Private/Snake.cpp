// Fill out your copyright notice in the Description page of Project Settings.


#include "Snake.h"

#include "Camera/CameraComponent.h"

#include "Components/InputComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"

#include "FG_UE_CoreTypes.h"
#include "Kismet/GameplayStatics.h"
#include "S_GameInstance.h"
#include "SnakeBody.h"

DEFINE_LOG_CATEGORY_STATIC(SnakeLog, All, All);

ASnake::ASnake()
{
	PrimaryActorTick.bCanEverTick = true;

    bIsFirstPlayer = true;

    HeadCollisionComponent = CreateDefaultSubobject<UBoxComponent>("HeadCollisionComponent");
    HeadCollisionComponent->SetupAttachment(GetRootComponent());
    RootComponent = HeadCollisionComponent;

    HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>("HeadMesh");
    HeadMesh->SetupAttachment(HeadCollisionComponent);

    RightEyeMesh = CreateDefaultSubobject<UStaticMeshComponent>("RightEyeMesh");
    RightEyeMesh->SetupAttachment(HeadMesh);
    RightEyeMesh->SetRelativeLocation(FVector(30.f, 30.f, 50.f));
    RightEyeMesh->SetRelativeScale3D(FVector(0.2f));

    LeftEyeMesh = CreateDefaultSubobject<UStaticMeshComponent>("LeftEyeMesh");
    LeftEyeMesh->SetupAttachment(HeadMesh);
    LeftEyeMesh->SetRelativeLocation(FVector(30.f, -30.f, 50.f));
    LeftEyeMesh->SetRelativeScale3D(FVector(0.2f));

    /*InstancedTailMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("InstancedTailMesh");
    InstancedTailMesh->SetupAttachment(HeadCollisionComponent);*/

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(HeadCollisionComponent);
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->bInheritRoll = false;
    SpringArmComponent->TargetArmLength = 350.0f;
    SpringArmComponent->SocketOffset = FVector(-20.0f, 0.0f, 200.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraComponent->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));

    CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
    CameraCollisionComponent->SetupAttachment(CameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);


    MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
    MoveScale = 1.f;

    MovementInterpolationTimer = 0.f;
    bIsMoving = false;

    StartLocation = GetActorLocation();
    TargetLocation = StartLocation;

    StartRotation = HeadMesh->GetRelativeRotation();
    TargetRotation = StartRotation;

    UE_LOG(SnakeLog, Display, TEXT("ASnake"));
}

//void ASnake::MoveUp(const FInputActionValue& Value)
//{
//    SetDirection(EDirectionState::Up);
//}
//
//void ASnake::MoveRight(const FInputActionValue& Value)
//{
//    SetDirection(EDirectionState::Right);
//}
//
//void ASnake::MoveDown(const FInputActionValue& Value)
//{
//    SetDirection(EDirectionState::Down);
//}
//
//void ASnake::MoveLeft(const FInputActionValue& Value)
//{
//    SetDirection(EDirectionState::Left);
//}
//
//void ASnake::Look(const FInputActionValue& Value)
//{
//	
//}

void ASnake::SetDirection(EDirectionState Direction)
{
    if(DirectionQueueCount >= 10)
    {
        UE_LOG(SnakeLog, Display, TEXT("DirectionQueueCount :  %d"), DirectionQueueCount);
        return;
    }

    if (!DirectionQueue.IsEmpty() && LastQueuedDirection == Direction)
    {
        return;
    }
    DirectionQueue.Enqueue(Direction);
    LastQueuedDirection = Direction;
    DirectionQueueCount++;

    /*if(Direction == CurrentDirection) return;
    CurrentDirection = Direction;*/
}

void ASnake::BeginPlay()
{
	Super::BeginPlay();
	
    /*if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                Subsystem->ClearAllMappings();
                if (bIsFirstPlayer && WASDMappingContext)
                {
                    Subsystem->AddMappingContext(WASDMappingContext, 0);
                }
                else if (!bIsFirstPlayer && ArrowsMappingContext)
                {
                    Subsystem->AddMappingContext(ArrowsMappingContext, 0);
                }
            }
        }
    }*/


    if (!GetWorld()) return;

    auto GameInstance = Cast<US_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (!GameInstance) return;

    const FGameSettings GameSettings = GameInstance->GetGameSettings();
    if (GameSettings.GridWidth > 0)
    {
        TileSize = GameSettings.TileSize;
    }
    else
    {
        return;
    }

    HeadCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASnake::OnCollisionBeginOverlap);
    HeadCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ASnake::OnCollisionEndOverlap);
}

void ASnake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bIsMoving)
    {
        MovementInterpolationTimer += DeltaTime;
        float Alpha = FMath::Clamp(MovementInterpolationTimer / StepInterval, 0.f, 1.f);

        FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
        SetActorLocation(NewLocation);

        FRotator NewRotation = FMath::Lerp(StartRotation, TargetRotation, Alpha);
        HeadMesh->SetRelativeRotation(NewRotation);

        if (MovementInterpolationTimer >= StepInterval)
        {
            bIsMoving = false;
            StartLocation = TargetLocation;
            StartRotation = TargetRotation;

            if (PendingGrowth > 0)
            {
                GrowSnakeBody();
                /*if (bIsAppleEatenRightNow)
                {
                    bIsAppleEatenRightNow = !bIsAppleEatenRightNow;
                }
                else
                {
                    GrowSnakeBody();
                }*/
            }
        }
    }
    else
    {
        AccumulatedTime += DeltaTime;
        if (AccumulatedTime >= StepInterval)
        {
            AccumulatedTime = 0.f;
            StartLocation = GetActorLocation();
            TargetLocation = StartLocation;

            StartRotation = HeadMesh->GetRelativeRotation();
            TargetRotation = StartRotation;

            switch (GetNextDirection())
            {
                case EDirectionState::Up:
                    TargetLocation += FVector(TileSize, 0.f, 0.f);
                    TargetRotation = FRotator(0.f);
                    break;
                case EDirectionState::Right:
                    TargetLocation += FVector(0.f, TileSize, 0.f);
                    TargetRotation = FRotator(0.f, 90.f, 0.f);
                    break;
                case EDirectionState::Down:
                    TargetLocation -= FVector(TileSize, 0.f, 0.f);
                    TargetRotation = FRotator(0.f, 180.f, 0.f);
                    break;
                case EDirectionState::Left:
                    TargetLocation -= FVector(0.f, TileSize, 0.f);
                    TargetRotation = FRotator(0.f, -90.f, 0.f);
                    break;
            }
            if (IsValid(ChildSnakeBody))
            {
                ChildSnakeBody->SetNextLocation(StartLocation);
                ChildSnakeBody->SetIsMoving();
            }
            MovementInterpolationTimer = 0.f;
            bIsMoving = true;
        }
    }
}

//void ASnake::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
//	{
//		EnhancedInputComponent->BindAction(MoveUpAction, ETriggerEvent::Triggered, this, &ASnake::MoveUp);
//		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ASnake::MoveRight);
//		EnhancedInputComponent->BindAction(MoveDownAction, ETriggerEvent::Triggered, this, &ASnake::MoveDown);
//		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Triggered, this, &ASnake::MoveLeft);
//
//		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASnake::Look);
//	}
//}

void ASnake::EatApple()
{
    UE_LOG(SnakeLog, Display, TEXT("EatApple"));

    PendingGrowth++;
    bIsAppleEatenRightNow = true;
    
}

void ASnake::HandleDeath()
{
    OnDead.Broadcast(this);
    if (IsValid(ChildSnakeBody))
    {
        ChildSnakeBody->HandleDestroy();
    }
    Destroy();
}

void ASnake::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        UE_LOG(SnakeLog, Display, TEXT("OnCollisionBeginOverlap with %s"), *OtherActor->GetName());
        ASnakeBody* SnakeBody = Cast<ASnakeBody>(OtherActor);
        if (SnakeBody && SnakeBody != ChildSnakeBody)
        {
            UE_LOG(SnakeLog, Display, TEXT("OnCollision with %s"), *SnakeBody->GetName());
            // END THE GAME

            HandleDeath();
        }
    }
}

void ASnake::OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ASnake::GrowSnakeBody()
{
    if (!GetWorld()) return;
    if (!SnakeBodyClass) return;

    FVector MovementVector = FVector::ZeroVector;
    switch (CurrentDirection)
    {
    case EDirectionState::Up:
        MovementVector = FVector(1.f, 0.f, 0.f);
        break;
    case EDirectionState::Right:
        MovementVector = FVector(0.f, 1.f, 0.f);
        break;
    case EDirectionState::Down:
        MovementVector = FVector(-1.f, 0.f, 0.f);
        break;
    case EDirectionState::Left:
        MovementVector = FVector(0.f, -1.f, 0.f);
        break;
    }

    FVector SnakeBodyLocation = FVector::ZeroVector;

    if (IsValid(ChildSnakeBody))
    {
        SnakeBodyLocation = ChildSnakeBody->GetLastLocation();
    }
    else
    {
        SnakeBodyLocation = GetActorLocation();
    }

    //SnakeBodyLocation = SnakeBodyLocation - MovementVector * TileSize;

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.Owner = this;

    ASnakeBody* SnakeBody = GetWorld()->SpawnActor<ASnakeBody>(SnakeBodyClass, SnakeBodyLocation, FRotator::ZeroRotator, SpawnParameters);
    
    if (SnakeBody)
    {
        PendingGrowth--;
        UE_LOG(SnakeLog, Display, TEXT("SpawnActor %s"), *SnakeBody->GetName());
    }

    if (IsValid(ChildSnakeBody))
    {
        ChildSnakeBody->AddChildSnakeBody(SnakeBody);
    }
    else
    {
        ChildSnakeBody = SnakeBody;
    }
}

void ASnake::HandleDirectionQueue()
{
    if (!DirectionQueue.IsEmpty())
    {
        EDirectionState NextDirection;
        DirectionQueue.Dequeue(NextDirection);
        DirectionQueueCount--;

        if ((CurrentDirection == EDirectionState::Down && NextDirection == EDirectionState::Up) ||
            (CurrentDirection == EDirectionState::Left && NextDirection == EDirectionState::Right) ||
            (CurrentDirection == EDirectionState::Right && NextDirection == EDirectionState::Left) ||
            (CurrentDirection == EDirectionState::Up && NextDirection == EDirectionState::Down))
        {
            return;
        }
        CurrentDirection = NextDirection;
    }
}

EDirectionState ASnake::GetNextDirection()
{
    HandleDirectionQueue();

    return CurrentDirection;
}


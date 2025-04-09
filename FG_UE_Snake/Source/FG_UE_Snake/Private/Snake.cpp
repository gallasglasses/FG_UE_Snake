// Fill out your copyright notice in the Description page of Project Settings.


#include "Snake.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ASnake::ASnake()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    bIsFirstPlayer = true;

    /*SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SceneComponent->SetupAttachment(GetRootComponent());*/

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->bInheritRoll = false;
    SpringArmComponent->TargetArmLength = 350.0f;
    SpringArmComponent->SocketOffset = FVector(-20.0f, 0.0f, 200.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraComponent->SetRelativeRotation(FRotator(0.0f, -20.0f, 0.0f));

    CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
    CameraCollisionComponent->SetupAttachment(CameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    HeadCollisionComponent = CreateDefaultSubobject<UBoxComponent>("HeadCollisionComponent");
    HeadCollisionComponent->SetupAttachment(GetRootComponent());

    HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>("HeadMesh");
    HeadMesh->SetupAttachment(HeadCollisionComponent);

    InstancedTailMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("InstancedTailMesh");
    InstancedTailMesh->SetupAttachment(HeadMesh);
}

void ASnake::Move(const FInputActionValue& Value)
{
	
}

void ASnake::Look(const FInputActionValue& Value)
{
	
}

// Called when the game starts or when spawned
void ASnake::BeginPlay()
{
	Super::BeginPlay();
	
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
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
    }
}

// Called every frame
void ASnake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASnake::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASnake::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASnake::Look);
	}

}


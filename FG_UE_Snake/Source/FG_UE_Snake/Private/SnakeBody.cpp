// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBody.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(SnakeBodyLog, All, All);

ASnakeBody::ASnakeBody()
{
	PrimaryActorTick.bCanEverTick = true;

	BodyCollisionComponent = CreateDefaultSubobject<UBoxComponent>("BodyCollisionComponent");
	BodyCollisionComponent->SetupAttachment(GetRootComponent());
	/*BodyCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BodyCollisionComponent->SetGenerateOverlapEvents(false);*/
	RootComponent = BodyCollisionComponent;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("BodyMesh");
	BodyMesh->SetupAttachment(BodyCollisionComponent);

	MovementInterpolationTimer = 0.f;
	bIsMoving = false;
	StepInterval = 0.4f;
}

void ASnakeBody::BeginPlay()
{
	Super::BeginPlay();

	BodyCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BodyCollisionComponent->SetGenerateOverlapEvents(false);

	CurrentLocation = GetActorLocation();
	TargetLocation = CurrentLocation;
}

void ASnakeBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(SnakeBodyLog, Display, TEXT("bIsMoving : %s"), bIsMoving ? TEXT("TRUE") : TEXT("FALSE"));
	if (bIsMoving)
	{
		MovementInterpolationTimer += DeltaTime;
		float Alpha = FMath::Clamp(MovementInterpolationTimer / StepInterval, 0.f, 1.f);

		FVector NewLocation = FMath::Lerp(CurrentLocation, TargetLocation, Alpha);
		SetActorLocation(NewLocation);

		if (MovementInterpolationTimer >= StepInterval)
		{
			bIsMoving = false;
			CurrentLocation = TargetLocation;
			EnableCollision();
		}
	}
}

const FVector& ASnakeBody::GetLastLocation()
{
	if (IsValid(ChildSnakeBody))
	{
		return ChildSnakeBody->GetLastLocation();
	}
	return CurrentLocation;
}

void ASnakeBody::SetNextLocation(const FVector& Location, EDirectionState Direction)
{
	if (Direction == EDirectionState::None) return;

	if (IsValid(ChildSnakeBody))
	{
		ChildSnakeBody->SetNextLocation(TargetLocation, Direction);
	}

	CurrentLocation = GetActorLocation();
	TargetLocation = Location;
}

void ASnakeBody::AddChildSnakeBody(ASnakeBody* SnakeBody)
{
	if (IsValid(ChildSnakeBody))
	{
		ChildSnakeBody->AddChildSnakeBody(SnakeBody);
	}
	else
	{
		ChildSnakeBody = SnakeBody;
	}
}

void ASnakeBody::SetIsMoving(bool IsMoving)
{
	if (IsValid(ChildSnakeBody))
	{
		ChildSnakeBody->SetIsMoving(IsMoving);
	}
	MovementInterpolationTimer = 0.f;
	bIsMoving = IsMoving;
}

void ASnakeBody::HandleDestroy()
{
	if (IsValid(ChildSnakeBody))
	{
		ChildSnakeBody->HandleDestroy();
	}
	Destroy();
}

void ASnakeBody::ChangeMeshMaterial(UMaterialInterface* Material)
{
	BodyMesh->SetMaterial(0, Material);
}

void ASnakeBody::EnableCollision()
{
	BodyCollisionComponent->SetGenerateOverlapEvents(true);
	BodyCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BodyCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}


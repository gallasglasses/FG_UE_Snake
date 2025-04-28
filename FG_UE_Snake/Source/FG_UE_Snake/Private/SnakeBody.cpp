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

			//UE_LOG(SnakeBodyLog, Display, TEXT("EnableCollision"));
			EnableCollision();

			/*FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ASnakeBody::EnableCollision, 1.f, false);*/

			/*if (BodyCollisionComponent->GetCollisionResponseToChannels() == ECollisionResponse::ECR_Ignore)
				BodyCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);*/
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

void ASnakeBody::SetNextLocation(const FVector& Location)
{
	if (IsValid(ChildSnakeBody))
	{
		ChildSnakeBody->SetNextLocation(TargetLocation);
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

void ASnakeBody::SetIsMoving()
{
	if (IsValid(ChildSnakeBody))
	{
		ChildSnakeBody->SetIsMoving();
	}
	MovementInterpolationTimer = 0.f;
	bIsMoving = true;
}

void ASnakeBody::HandleDestroy()
{
	if (IsValid(ChildSnakeBody))
	{
		ChildSnakeBody->HandleDestroy();
	}
	Destroy();
}

void ASnakeBody::EnableCollision()
{
	BodyCollisionComponent->SetGenerateOverlapEvents(true);
	BodyCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BodyCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}


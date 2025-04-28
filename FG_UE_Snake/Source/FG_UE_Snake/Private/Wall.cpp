// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall.h"
#include "Snake.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(WallLog, All, All);

// Sets default values
AWall::AWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WallCollisionComponent = CreateDefaultSubobject<UBoxComponent>("WallCollisionComponent");
	WallCollisionComponent->SetupAttachment(GetRootComponent());
	RootComponent = WallCollisionComponent;

	WallCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AWall::OnCollisionBeginOverlap);

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>("WallMesh");
	WallMesh->SetupAttachment(WallCollisionComponent);
}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWall::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor)
	{
		UE_LOG(WallLog, Display, TEXT("On Collision the Wall by %s"), *OtherActor->GetName());
		ASnake* Snake = Cast<ASnake>(OtherActor);
		if (Snake)
		{
			UE_LOG(WallLog, Display, TEXT("Snake collides with the wall"));
			Snake->HandleDeath();
		}
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Apple.h"
#include "Snake.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(AppleLog, All, All);

// Sets default values
AApple::AApple()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AppleCollisionComponent = CreateDefaultSubobject<UBoxComponent>("AppleCollisionComponent");
	AppleCollisionComponent->SetupAttachment(GetRootComponent());
	RootComponent = AppleCollisionComponent;

	AppleCollisionComponent->InitBoxExtent(FVector(50.f, 50.f, 50.f));

	AppleCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AApple::OnCollisionBeginOverlap);
	AppleCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AApple::OnCollisionEndOverlap);

	AppleMesh = CreateDefaultSubobject<UStaticMeshComponent>("AppleMesh");
	AppleMesh->SetupAttachment(AppleCollisionComponent);

}

// Called when the game starts or when spawned
void AApple::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AApple::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AApple::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UE_LOG(AppleLog, Display, TEXT("EatApple OtherActor"));
		ASnake* Snake = Cast<ASnake>(OtherActor);
		if (Snake)
		{
			UE_LOG(AppleLog, Display, TEXT("EatApple ASnake"));
			Snake->EatApple();


			OnAppleDestroyed.Broadcast();
			OnAppleDestroyed.Clear();
			Destroy();
		}
	}
}

void AApple::OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


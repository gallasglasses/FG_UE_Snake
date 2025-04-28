// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(TileLog, All, All);

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileCollisionComponent = CreateDefaultSubobject<UBoxComponent>("TileCollisionComponent");
	TileCollisionComponent->SetupAttachment(GetRootComponent());
	RootComponent = TileCollisionComponent;

	TileCollisionComponent->InitBoxExtent(FVector(50.f, 50.f, 100.f));

	TileCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATile::OnCollisionBeginOverlap);
	TileCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ATile::OnCollisionEndOverlap);

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>("TileMesh");
	TileMesh->SetupAttachment(TileCollisionComponent);

	Neighbours.Add(EDirectionState::Up, nullptr);
	Neighbours.Add(EDirectionState::Right, nullptr);
	Neighbours.Add(EDirectionState::Down, nullptr);
	Neighbours.Add(EDirectionState::Left, nullptr);

	TileCollisionComponent->UpdateOverlaps();
	//UE_LOG(TileLog, Display, TEXT("Position :  %s"), *Position.ToString());
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::SetPosition(const int32 x, const int32 y)
{
	Position = FVector2D(x,y);
	//UE_LOG(TileLog, Display, TEXT("SetPosition :  %s"), *Position.ToString());
}

void ATile::SetNeighbour(EDirectionState Direction, ATile* Neighbour)
{
	if(!Neighbours.Find(Direction)) return;

	Neighbours[Direction] = Neighbour;
}

void ATile::RegisterOccupant(AActor* Actor)
{
	if (Actor && Actor != this && !OccupiedBy.Contains(Actor))
	{
		OccupiedBy.Add(Actor);
		UE_LOG(TileLog, Display, TEXT("%s now occupied by %s"), *GetName(), *Actor->GetName());
	}
}

void ATile::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	RegisterOccupant(OtherActor);

	//if (OtherActor && OtherActor != this && !OccupiedBy.Contains(OtherActor))
	//{
	//	OccupiedBy.Add(OtherActor);
	//	//UE_LOG(TileLog, Display, TEXT("Tile %s OccupiedBy :  %s"), *GetName(), *OtherActor->GetName());
	//}

	// +++++++++ Just for log
	TArray<FString> Names;
	Names.Reserve(OccupiedBy.Num());
	for (AActor* Actor : OccupiedBy)
	{
		Names.Add(Actor ? Actor->GetName() : TEXT("nullptr"));
	}
	const FString Joined = FString::Join(Names, TEXT(", "));
	UE_LOG(TileLog, Display, TEXT("Tile %s OccupiedBy (%d): %s"), *GetName(), OccupiedBy.Num(), *Joined);
	// +++++++++ Just for log
}

void ATile::OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && OccupiedBy.Contains(OtherActor))
	{
		OccupiedBy.Remove(OtherActor);
		UE_LOG(TileLog, Display, TEXT("Tile %s ReleasedBy :  %s"), *GetName(), *OtherActor->GetName());
	}
}

int32 ATile::GetTileBounds() const
{
	return TileCollisionComponent->GetScaledBoxExtent().X;
}

const TArray<AActor*>& ATile::GetOccupiedBy() const
{
	return OccupiedBy;
}


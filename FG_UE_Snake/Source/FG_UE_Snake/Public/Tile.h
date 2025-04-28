// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FG_UE_CoreTypes.h"
#include "Tile.generated.h"

class UBoxComponent;

UCLASS()
class FG_UE_SNAKE_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tile")
	UBoxComponent* TileCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tile")
	UStaticMeshComponent* TileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	FVector2D Position = FVector2D(-1, -1);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	TArray<AActor*> OccupiedBy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	TMap<EDirectionState, ATile*> Neighbours;


	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	void SetPosition(const int32 x, const int32 y);
	void SetNeighbour(EDirectionState Direction, ATile* Neighbour);
	void RegisterOccupant(AActor* Actor);

	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	int32 GetTileBounds() const;

	const TArray<AActor*>& GetOccupiedBy() const;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FG_UE_CoreTypes.h"
#include "GameFramework/Actor.h"
#include "S_GridManager.generated.h"

class ATile;
class AWall;
class AApple;
class ANavMeshBoundsVolume;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpawnedAppleSignature, AActor*, Apple);

UCLASS()
class FG_UE_SNAKE_API AS_GridManager : public AActor
{
	GENERATED_BODY()
	
public:	

	AS_GridManager();

	UPROPERTY(BlueprintAssignable, Category = "GridSettings")
	FOnSpawnedAppleSignature OnSpawnedApple;

protected:

	virtual void BeginPlay() override;

public:

	UPROPERTY(EditDefaultsOnly, Category = "GridSettings")
	TSubclassOf<ATile> TileClass;

	UPROPERTY(EditDefaultsOnly, Category = "GridSettings")
	TSubclassOf<AWall> WallClass;

	UPROPERTY(EditDefaultsOnly, Category = "GridSettings")
	TSubclassOf<AApple> AppleClass;

	UPROPERTY(EditAnywhere, Category = "Navigation")
	ANavMeshBoundsVolume* NavMeshVolume;


	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "GridPathfinding")
	bool FindPath(const FVector2D& Start, const FVector2D& Goal, TArray<ATile*>& OutPathTiles);

	UFUNCTION(BlueprintPure, Category = "GridPathfinding")
	FVector2D WorldLocationToGridIndex(const FVector& WorldLoc) const;

	UFUNCTION(BlueprintPure, Category = "GridPathfinding")
	FVector GridIndexToWorldLocation(const FVector2D& GridIdx) const;

	AActor* GetSpawnedApple() const;

private:

	TArray<ATile*> Tiles;
	TArray<AWall*> Walls;
	int32 GridWidth;
	int32 GridHeight;
	int32 TileSize;

	UPROPERTY()
	AApple* SpawnedApple;

	void GenerateGrid();
	void SetNeighbours();
	void GenerateWalls(const TArray<int32>& WallLayout);
	void SpawnApple();

	bool LoadWallLayout(TArray<int32>& OutWallLayout, EGameLevel Level, const FString& JsonFilePath);

	UFUNCTION()
	void CalculateAppleCount(float PercentToEat, int32 NumPlayers);

	void RebuildNavMesh();
};

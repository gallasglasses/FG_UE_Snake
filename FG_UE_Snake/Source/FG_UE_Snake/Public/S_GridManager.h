// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FG_UE_CoreTypes.h"
#include "GameFramework/Actor.h"
#include "S_GridManager.generated.h"

class ATile;
class AWall;
class AApple;


UCLASS()
class FG_UE_SNAKE_API AS_GridManager : public AActor
{
	GENERATED_BODY()
	
public:	

	AS_GridManager();

protected:

	virtual void BeginPlay() override;

public:

	UPROPERTY(EditDefaultsOnly, Category = "GridSettings")
	TSubclassOf<ATile> TileClass;

	UPROPERTY(EditDefaultsOnly, Category = "GridSettings")
	TSubclassOf<AWall> WallClass;

	UPROPERTY(EditDefaultsOnly, Category = "GridSettings")
	TSubclassOf<AApple> AppleClass;

	virtual void Tick(float DeltaTime) override;

	

private:

	TArray<ATile*> Tiles;
	int32 GridWidth;
	int32 GridHeight;
	int32 TileSize;

	void GenerateGrid();
	void SetNeighbours();
	void GenerateWalls(const TArray<int32>& WallLayout);
	void SpawnApple();

	bool LoadWallLayout(TArray<int32>& OutWallLayout, EGameLevel Level, const FString& JsonFilePath);


};

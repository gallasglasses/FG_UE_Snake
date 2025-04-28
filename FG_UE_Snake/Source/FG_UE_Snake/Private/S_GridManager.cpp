// Fill out your copyright notice in the Description page of Project Settings.


#include "S_GridManager.h"
#include "Tile.h"
#include "Wall.h"
#include "Apple.h"
#include "Kismet/GameplayStatics.h"
#include "S_GameInstance.h"
#include "JsonManager.h"

DEFINE_LOG_CATEGORY_STATIC(GridManagerLog, All, All);

AS_GridManager::AS_GridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AS_GridManager::BeginPlay()
{
	Super::BeginPlay();

	if (!GetWorld()) return;

	auto GameInstance = Cast<US_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if(!GameInstance) return;

	const FGameSettings GameSettings = GameInstance->GetGameSettings();
	if (GameSettings.GridWidth > 0)
	{
		GridWidth = GameSettings.GridWidth;
		GridHeight = GameSettings.GridHeight;
		TileSize = GameSettings.TileSize;
		FVector2D StartPos = GameSettings.FirstPlayerPosition;

		UE_LOG(GridManagerLog, Display, TEXT("Grid Width: %d, Height: %d, TileSize: %d, SnakeStart: %s"),
			GridWidth, GridHeight, TileSize, *StartPos.ToString());
	}
	else
	{
		UE_LOG(GridManagerLog, Warning, TEXT("DefaultRow is not found"));
		return;
	}

	GenerateGrid();
	SetNeighbours();
	
	TArray<int32> WallLayout;
	if (LoadWallLayout(WallLayout, GameInstance->GetNextLevel(), GameSettings.LevelLayoutFile))
	{
		GenerateWalls(WallLayout);
	}
	else
	{
		UE_LOG(GridManagerLog, Warning, TEXT("Failed to load WallLayout from JSON"));
	}

	SpawnApple();
}

// Called every frame
void AS_GridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AS_GridManager::GenerateGrid()
{
	if (!GetWorld()) return;
	if(!TileClass) return;

	Tiles.SetNum(GridHeight * GridWidth);

	for (int32 y = 0; y < GridHeight; y++)
	{
		for (int32 x = 0; x < GridWidth; x++)
		{
			FVector TileLocation = GetActorLocation() + FVector(TileSize * x, TileSize * y, 0.f);
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = this;

			ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileClass, TileLocation, FRotator::ZeroRotator, SpawnParameters);
			
			if (NewTile)
			{
				NewTile->SetPosition(x,y);
				Tiles[y * GridWidth + x] = NewTile;
			}
		}
	}
}

void AS_GridManager::SetNeighbours()
{
	for (int32 y = 0; y < GridHeight; y++)
	{
		for (int32 x = 0; x < GridWidth; x++)
		{
			ATile* Tile = Tiles[y * GridWidth + x];
			
			ATile* UpNeighbour = (y > 0) ? Tiles[(y - 1) * GridWidth + x] : nullptr;
			Tile->SetNeighbour(EDirectionState::Up, UpNeighbour);
			
			ATile* RightNeighbour = (x < GridWidth - 1) ? Tiles[y * GridWidth + x + 1] : nullptr;
			Tile->SetNeighbour(EDirectionState::Right, RightNeighbour);
			
			ATile* DownNeighbour = (y < GridHeight - 1) ? Tiles[(y + 1) * GridWidth + x] : nullptr;
			Tile->SetNeighbour(EDirectionState::Down, DownNeighbour);
			
			ATile* LeftNeighbour = (x > 0) ? Tiles[y * GridWidth + x - 1] : nullptr;
			Tile->SetNeighbour(EDirectionState::Left, LeftNeighbour);
		}
	}
}

void AS_GridManager::GenerateWalls(const TArray<int32>& WallLayout)
{
	if (!GetWorld()) return;
	if (!WallClass) return;
	
	for (int32 y = 0; y < GridHeight; y++)
	{
		for (int32 x = 0; x < GridWidth; x++)
		{
			if (WallLayout[y * GridWidth + x] == 1)
			{
				FVector WallLocation = GetActorLocation() + FVector(TileSize * x, TileSize * y, TileSize);
				FActorSpawnParameters SpawnParameters;
				SpawnParameters.Owner = this;

				AWall* NewWall = GetWorld()->SpawnActor<AWall>(WallClass, WallLocation, FRotator::ZeroRotator, SpawnParameters);

				if (NewWall)
				{
					const int32 TileIndex = y * GridWidth + x;
					if (Tiles.IsValidIndex(TileIndex) && Tiles[TileIndex])
					{
						Tiles[TileIndex]->RegisterOccupant(NewWall);
					}
				}
			}
		}
	}
}

void AS_GridManager::SpawnApple()
{
	bool bIsNoSpawnPoint = true;
	int32 IndexY = -1;
	int32 IndexX = -1;

	while (bIsNoSpawnPoint)
	{
		IndexY = FMath::RandRange(1, GridHeight - 2);
		IndexX = FMath::RandRange(1, GridWidth - 2);
		const int32 TileIndex = IndexY * GridWidth + IndexX;
		if (Tiles.IsValidIndex(TileIndex) && Tiles[TileIndex])
		{
			const TArray<AActor*> Occupiers = Tiles[TileIndex]->GetOccupiedBy();
			if (!Occupiers.IsEmpty() /*Occupiers.FindItemByClass<AWall>()*/)
			{
				continue;
			}
			else
			{
				bIsNoSpawnPoint = false;
			}
		}
	}

	FVector AppleLocation = GetActorLocation() + FVector(TileSize * IndexX, TileSize * IndexY, TileSize);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;

	AApple* Apple = GetWorld()->SpawnActor<AApple>(AppleClass, AppleLocation, FRotator::ZeroRotator, SpawnParameters);

	if (Apple)
	{
		const int32 TileIndex = IndexY * GridWidth + IndexX;
		if (Tiles.IsValidIndex(TileIndex) && Tiles[TileIndex])
		{
			Tiles[TileIndex]->RegisterOccupant(Apple);
			Apple->OnAppleDestroyed.AddUObject(this, &AS_GridManager::SpawnApple);
		}
	}
}

bool AS_GridManager::LoadWallLayout(TArray<int32>& OutWallLayout, EGameLevel Level, const FString& JsonFilePath)
{
	// JsonFilePath = "/Data/Levels/Level"
	// "Content/Data/Levels/Level1.json", "Content/Data/Levels/Level2.json"

	if (JsonFilePath.IsEmpty())
	{
		return false;
	}

	int32 ID = 0;
	switch (Level)
	{
		case EGameLevel::Level1:
			ID = 1;
			break;
		case EGameLevel::Level2:
			ID = 2;
			break;
		case EGameLevel::Level3:
			ID = 3;
			break;
		case EGameLevel::Level4:
			ID = 4;
			break;
		case EGameLevel::Level5:
			ID = 5;
			break;
	}

	if(ID == 0)
	{
		UE_LOG(GridManagerLog, Error, TEXT("ID == %d"), ID);
		return false;
	}

	FString FullPath = FPaths::ProjectContentDir() / (JsonFilePath + FString::FromInt(ID) + TEXT(".json"));

	bool bOutSuccess = false;
	FString OutInfoMessage;
	TSharedPtr<FJsonObject> JsonObject = UJsonManager::ReadJson(FullPath, bOutSuccess, OutInfoMessage);

	if (!bOutSuccess || !JsonObject.IsValid())
	{
		UE_LOG(GridManagerLog, Error, TEXT("Failed to load layout %s: %s"), *FullPath, *OutInfoMessage);
		return false;
	}

	const TArray<TSharedPtr<FJsonValue>>* LayoutArray = nullptr;
	if (!JsonObject->TryGetArrayField(TEXT("layout"), LayoutArray))
	{
		UE_LOG(GridManagerLog, Error, TEXT("There is no field 'layout' %s"), *FullPath);
		return false;
	}

	OutWallLayout.Empty();

	for (const TSharedPtr<FJsonValue>& RowValue : *LayoutArray)
	{
		const TArray<TSharedPtr<FJsonValue>>* RowArray = nullptr;
		if (!RowValue->TryGetArray(RowArray))
		{
			UE_LOG(LogTemp, Warning, TEXT("String is not an array"));
			continue;
		}

		for (const TSharedPtr<FJsonValue>& WallTileValue : *RowArray)
		{
			OutWallLayout.Add(WallTileValue->AsNumber());
		}
	}

	if (OutWallLayout.Num() != GridWidth * GridHeight)
	{
		UE_LOG(GridManagerLog, Error, TEXT("JSON data (number of rows and columns) does not match grid dimensions"));
		return false;
	}

	return true;
}


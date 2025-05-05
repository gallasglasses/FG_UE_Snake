#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FG_UE_CoreTypes.generated.h"

class UInputMappingContext;


UENUM(BlueprintType)
enum class EGameState : uint8
{
	Init = 0,
	Menu,
	InProgress,
	Pause,
	Death,
	GameOver
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameStateChangedSignature, EGameState, PreviousState, EGameState, NextState);

UENUM(BlueprintType)
enum class EDirectionState : uint8
{
	None,
	Up,
	Right,
	Down,
	Left
};

UENUM(BlueprintType)
enum class EGameMode : uint8
{
	Singleplayer = 0,
	Cooperative,
	Competitive
};

UENUM(BlueprintType)
enum class EGamePlayer : uint8
{
	Real = 0,
	AI
};

UENUM(BlueprintType)
enum class EGamePlayerControllerType : uint8
{
	Keyboard1 = 0,
	Keyboard2,
	AI
};

UENUM(BlueprintType)
enum class EGameLevel : uint8
{
	PersistentLevel,
	Level1,
	Level2,
	Level3,
	Level4,
	Level5,
	InitLevel,
	MenuLevel
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNextLevelChangedSignature, EGameLevel, NextLevel);

USTRUCT(BlueprintType)
struct FGameSettings : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameSettings")
	int32 GridWidth = 25;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameSettings")
	int32 GridHeight = 25;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameSettings")
	int32 TileSize = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameSettings")
	FVector2D FirstPlayerPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameSettings")
	FVector2D SecondPlayerPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameSettings")
	UInputMappingContext* FirstPlayerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameSettings")
	UInputMappingContext* SecondPlayerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameSettings")
	FString LevelLayoutFile;

};

USTRUCT(BlueprintType)
struct FGameData
{
	GENERATED_BODY()

	UPROPERTY()
	EGameMode GameMode;

	UPROPERTY()
	EGamePlayer GamePlayer;

	UPROPERTY()
	int32 NumberOfApplesToEat;

};

class S_Utils
{
public:

	static FText TextFromInt(int32 Number) { return FText::FromString(FString::FromInt(Number)); }
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FG_UE_CoreTypes.h"
#include "S_GameModeBase.generated.h"

class AS_PlayerController;
class US_GameInstance;
class US_GameInstanceSubsystem;
class AS_GameDataState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDiedSignature, AController*, DeadController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerWonSignature, AController*, WinningController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCalculateApplesSignature, float, PercentApplesToEat, int32, NumPlayersToPlay);

class AAIController;
class AS_GridManager;

UCLASS()
class FG_UE_SNAKE_API AS_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AS_GameModeBase();

	UPROPERTY(BlueprintAssignable, Category = "GameData")
	FOnPlayerDiedSignature OnPlayerDied;

	UPROPERTY(BlueprintAssignable, Category = "GameData")
	FOnPlayerWonSignature OnPlayerWon;

	UPROPERTY(BlueprintAssignable, Category = "GameData")
	FOnCalculateApplesSignature OnCalculateApples;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void SetNumberOfApplesToFinishLevel(int32 Number);

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void StartPlay() override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
	virtual bool ClearPause() override;

	AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void StartGame();
	void StartNextLevel();
	void OnChooseMainMenu();

	UFUNCTION()
	void Dead(AController* DeadController);

	UFUNCTION(BlueprintCallable)
	void GameOver(AController* Winner);

	TArray<AController*> GetPlayerControllers();

	void SetGridManager(AS_GridManager* Manager);
	AS_GridManager* GetGridManager() const;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameData")
	EGameLevel NextLevel = EGameLevel::Level2;

	UPROPERTY(EditDefaultsOnly, Category = "GameData")
	TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "GameData")
	TSubclassOf<APawn> AIPawnClass;

private:

	int32 NumberOfApplesToFinishLevel = -1;

	TArray<AController*> PlayerControllers;

	AS_GridManager* GridManager;

	UFUNCTION()
	void HandleApplesCountChanged(int32 NewCount);

	void AddSecondPlayer(EGamePlayer Player, FString& ErrorMessage);
	void ResetPlayer();
	void SpawnAISnake();

	US_GameInstanceSubsystem* GetThisGameInstanceSubsystem() const;
	US_GameInstance* GetThisGameInstance() const;
	AS_GameDataState* GetThisGameDataState() const;

};

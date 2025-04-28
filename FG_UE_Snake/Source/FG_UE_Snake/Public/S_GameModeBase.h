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

UCLASS()
class FG_UE_SNAKE_API AS_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AS_GameModeBase();

	UPROPERTY(BlueprintAssignable, Category = "GameData")
	FOnNextLevelChangedSignature OnNextLevelChanged;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void StartPlay() override;
	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
	virtual bool ClearPause() override;

	AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void StartGame();
	void Dead();

	UFUNCTION(BlueprintCallable)
	void GameOver();

	TArray<AS_PlayerController*> GetPlayerControllers();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameData")
	EGameLevel NextLevel = EGameLevel::Level2;

private:

	TArray<AS_PlayerController*> PlayerControllers;

	void AddSecondPlayer(EGamePlayer Player, FString& ErrorMessage);
	US_GameInstanceSubsystem* GetThisGameInstanceSubsystem() const;
	US_GameInstance* GetThisGameInstance() const;
	AS_GameDataState* GetThisGameDataState() const;
};

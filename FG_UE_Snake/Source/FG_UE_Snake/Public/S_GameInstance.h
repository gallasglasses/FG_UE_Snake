// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FG_UE_CoreTypes.h"
#include "S_GameInstance.generated.h"


UCLASS()
class FG_UE_SNAKE_API US_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameData")
	UDataTable* LevelSettingsTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameData")
	EGameLevel CurrentLevel = EGameLevel::MenuLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameData")
	EGameLevel NextLevel = EGameLevel::PersistentLevel;

	/*UPROPERTY(EditDefaultsOnly, Transient, BlueprintReadOnly, Category = "GameData")
	TMap<EGameLevel, TSoftObjectPtr<UWorld>> Levels;*/

	UPROPERTY(EditDefaultsOnly, Transient, BlueprintReadOnly, Category = "GameData")
	TMap<EGameLevel, FName> Levels;

public:
	UPROPERTY(BlueprintAssignable, Category = "GameData")
	FOnNextLevelChangedSignature OnNextLevelChanged;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	FORCEINLINE EGameLevel GetCurrentLevel() const { return CurrentLevel; }

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void SetCurrentLevel(EGameLevel Level);

	UFUNCTION(BlueprintCallable, Category = "GameData")
	FORCEINLINE EGameLevel GetNextLevel() const { return NextLevel; }

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void SetNextLevel(EGameLevel InNextLevel);

	UFUNCTION(BlueprintCallable, Category = "GameData")
	FGameSettings GetGameSettings(FName RowName = "DefaultRow") const;

	/*UFUNCTION(BlueprintCallable, Category = "GameData")
	TSoftObjectPtr<UWorld> GetLevel(EGameLevel NewLevel) const;*/

	UFUNCTION(BlueprintCallable, Category = "GameData")
	FName GetLevel(EGameLevel NewLevel) const;
};

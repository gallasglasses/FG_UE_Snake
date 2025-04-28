// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FG_UE_CoreTypes.h"
#include "S_GameDataSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class FG_UE_SNAKE_API US_GameDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	
	UPROPERTY()
	FGameData GameData;

public:

	UFUNCTION(BlueprintCallable)
	void SaveGameData();

	UFUNCTION(BlueprintCallable)
	void LoadGameData();
	
};

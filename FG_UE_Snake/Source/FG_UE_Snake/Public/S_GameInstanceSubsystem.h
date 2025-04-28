// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FG_UE_CoreTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "S_GameInstanceSubsystem.generated.h"



UCLASS()
class FG_UE_SNAKE_API US_GameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	
	UPROPERTY()
	EGameState GameState = EGameState::Init;

public:

	UPROPERTY(BlueprintAssignable, Category = "GameState")
	FOnGameStateChangedSignature OnGameStateChanged;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EGameState GetGameState() const { return GameState; }

	UFUNCTION(BlueprintCallable)
	void SetGameState(EGameState State);
};

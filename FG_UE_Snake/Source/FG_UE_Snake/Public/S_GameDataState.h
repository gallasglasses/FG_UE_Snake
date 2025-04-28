// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FG_UE_CoreTypes.h"
#include "S_GameDataState.generated.h"



UCLASS()
class FG_UE_SNAKE_API AS_GameDataState : public AGameStateBase
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameData")
	EGameMode GameMode = EGameMode::Singleplayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameData")
	EGamePlayer GamePlayer = EGamePlayer::Real;
	

public:

	UFUNCTION(BlueprintCallable, Category = "GameData")
	FORCEINLINE EGameMode GetGameMode() const { return GameMode; }

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void SetGameMode(EGameMode Mode);

	UFUNCTION(BlueprintCallable, Category = "GameData")
	FORCEINLINE EGamePlayer GetGamePlayer() const { return GamePlayer; }

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void SetGamePlayer(EGamePlayer Player);
};

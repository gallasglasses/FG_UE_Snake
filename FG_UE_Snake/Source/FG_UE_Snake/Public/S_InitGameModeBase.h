// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "S_InitGameModeBase.generated.h"

class US_GameInstanceSubsystem;

UCLASS()
class FG_UE_SNAKE_API AS_InitGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	AS_InitGameModeBase();

	virtual void StartPlay() override;

private:

	void StartGame();
	US_GameInstanceSubsystem* GetGameInstanceSubsystem() const;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "S_InitGameModeBase.h"
#include "S_GameInstance.h"
#include "S_GameInstanceSubsystem.h"
#include "FG_UE_CoreTypes.h"
#include "S_InitPlayerController.h"
#include "InitHUD.h"

AS_InitGameModeBase::AS_InitGameModeBase()
{
	PlayerControllerClass = AS_InitPlayerController::StaticClass();
	HUDClass = AInitHUD::StaticClass();
}

void AS_InitGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetGameInstanceSubsystem()->SetGameState(EGameState::Init);

	StartGame();
}

void AS_InitGameModeBase::StartGame()
{
	
}

US_GameInstanceSubsystem* AS_InitGameModeBase::GetGameInstanceSubsystem() const
{
	US_GameInstance* GameInstance = Cast<US_GameInstance>(GetGameInstance());
	if (!GameInstance) return nullptr;

	US_GameInstanceSubsystem* GameInstanceSubsystem = GameInstance->GetSubsystem<US_GameInstanceSubsystem>();
	if (!GameInstanceSubsystem) return nullptr;

	return GameInstanceSubsystem;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "S_GameDataSubsystem.h"
#include "S_GameDataState.h"

DEFINE_LOG_CATEGORY_STATIC(GameDataSubsystemLog, All, All);

void US_GameDataSubsystem::SaveGameData()
{
	if (!GetWorld()) return;

	AS_GameDataState* GameDataState = GetWorld()->GetGameState<AS_GameDataState>();
	if (!GameDataState) return;

	GameData.GameMode = GameDataState->GetGameMode();
	GameData.GamePlayer = GameDataState->GetGamePlayer();
	GameData.NumberOfApplesToEat = GameDataState->GetNumberOfApplesToEat();
	UE_LOG(GameDataSubsystemLog, Display, TEXT("SaveGameData"));
}

void US_GameDataSubsystem::LoadGameData()
{
	if (!GetWorld()) return;

	AS_GameDataState* GameDataState = GetWorld()->GetGameState<AS_GameDataState>();
	if (!GameDataState)
	{
		UE_LOG(GameDataSubsystemLog, Display, TEXT("GameDataState not set"));
		return;
	}

	GameDataState->SetGameMode(GameData.GameMode);
	GameDataState->SetGamePlayer(GameData.GamePlayer);
	GameDataState->SetNumberOfApplesToEat(GameData.NumberOfApplesToEat);
	UE_LOG(GameDataSubsystemLog, Display, TEXT("LoadGameData"));
}

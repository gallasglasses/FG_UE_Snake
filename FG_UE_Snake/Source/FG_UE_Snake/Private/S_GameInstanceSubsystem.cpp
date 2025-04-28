// Fill out your copyright notice in the Description page of Project Settings.


#include "S_GameInstanceSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(US_GameInstanceSubsystemLog, All, All);

void US_GameInstanceSubsystem::SetGameState(EGameState State)
{
	if (GameState == State) return;

	EGameState PreviousState = GameState;
	GameState = State;

	UE_LOG(US_GameInstanceSubsystemLog, Display, TEXT("GameState changed from %s to %s"),
		*StaticEnum<EGameState>()->GetNameStringByValue((int64)PreviousState),
		*StaticEnum<EGameState>()->GetNameStringByValue((int64)GameState)
	);

	OnGameStateChanged.Broadcast(PreviousState, GameState);
}

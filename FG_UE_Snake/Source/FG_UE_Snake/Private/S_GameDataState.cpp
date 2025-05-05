// Fill out your copyright notice in the Description page of Project Settings.


#include "S_GameDataState.h"


void AS_GameDataState::SetGameMode(EGameMode Mode)
{
    if (GameMode == Mode) return;

    GameMode = Mode;
}

void AS_GameDataState::SetGamePlayer(EGamePlayer Player)
{
    if (GamePlayer == Player) return;

    GamePlayer = Player;
}

void AS_GameDataState::SetNumberOfApplesToEat(int32 NumberApples)
{
    NumberOfApplesToEat = NumberApples;
}

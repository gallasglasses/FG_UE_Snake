// Fill out your copyright notice in the Description page of Project Settings.


#include "S_GameInstance.h"

void US_GameInstance::SetCurrentLevel(EGameLevel Level)
{
    if (CurrentLevel == Level) return;

    CurrentLevel = Level;
}

void US_GameInstance::SetNextLevel(EGameLevel InNextLevel)
{
    if (NextLevel == InNextLevel) return;

    NextLevel = InNextLevel;
}

FGameSettings US_GameInstance::GetGameSettings(FName RowName) const
{
    if (LevelSettingsTable)
    {
        const FString ContextString(TEXT("GetGameSettings"));
        if (const FGameSettings* Settings = LevelSettingsTable->FindRow<FGameSettings>(RowName, ContextString, true))
        {
            return *Settings;
        }
    }

    return FGameSettings();
}

FName US_GameInstance::GetLevel(EGameLevel NewLevel) const
{
    if (!Levels.Find(NewLevel)) return FName();
    
    return Levels[NewLevel];
}

//TSoftObjectPtr<UWorld> US_GameInstance::GetLevel(EGameLevel NewLevel) const
//{
//    if (!Levels.Find(NewLevel)) return nullptr;
//
//    return Levels[NewLevel];
//}

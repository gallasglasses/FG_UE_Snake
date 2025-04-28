// Fill out your copyright notice in the Description page of Project Settings.


#include "S_MenuGameModeBase.h"
#include "S_MenuPlayerController.h"
#include "S_MenuHUD.h"
#include "S_GameDataState.h"

AS_MenuGameModeBase::AS_MenuGameModeBase()
{
	PlayerControllerClass = AS_MenuPlayerController::StaticClass();
	HUDClass = AS_MenuHUD::StaticClass();
	GameStateClass = AS_GameDataState::StaticClass();
}

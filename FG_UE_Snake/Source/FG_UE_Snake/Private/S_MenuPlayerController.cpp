// Fill out your copyright notice in the Description page of Project Settings.


#include "S_MenuPlayerController.h"

void AS_MenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}

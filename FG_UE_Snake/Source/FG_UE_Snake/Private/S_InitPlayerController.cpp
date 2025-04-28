// Fill out your copyright notice in the Description page of Project Settings.


#include "S_InitPlayerController.h"

void AS_InitPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}

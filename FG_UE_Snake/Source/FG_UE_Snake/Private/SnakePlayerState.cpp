// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePlayerState.h"

void ASnakePlayerState::SetControllerType(EGamePlayerControllerType Type)
{
	ControllerType = Type;
}

void ASnakePlayerState::OnAppleCountChanged(AController* EatenBy)
{
	ApplesEaten++;
	OnApplesEatenChanged.Broadcast(ApplesEaten);
}

void ASnakePlayerState::ResetAppleCount()
{
	ApplesEaten = 0;
}

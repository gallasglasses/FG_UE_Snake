// Fill out your copyright notice in the Description page of Project Settings.


#include "AISnake.h"
#include "S_AIController.h"

AAISnake::AAISnake()
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	//AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; bug - spawns second BP_AISnakeController
	AIControllerClass = AS_AIController::StaticClass();
}

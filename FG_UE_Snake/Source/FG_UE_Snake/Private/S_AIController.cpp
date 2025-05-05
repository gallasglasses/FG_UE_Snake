// Fill out your copyright notice in the Description page of Project Settings.


#include "S_AIController.h"
#include "AISnake.h"

DEFINE_LOG_CATEGORY_STATIC(AS_AIControllerLog, All, All);

void AS_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const auto AICharacter = Cast<AAISnake>(InPawn);
	if (AICharacter)
	{
		UE_LOG(AS_AIControllerLog, Display, TEXT("%s OnPossess : %s"), *GetName(), *InPawn->GetName());
		RunBehaviorTree(AICharacter->BehaviorTree);
	}
}

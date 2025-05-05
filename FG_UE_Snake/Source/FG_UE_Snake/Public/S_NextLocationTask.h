// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "S_NextLocationTask.generated.h"

/**
 * 
 */
UCLASS()
class FG_UE_SNAKE_API US_NextLocationTask : public UBTTaskNode
{
	GENERATED_BODY()

public:

	US_NextLocationTask();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector AimLocationKey;
};

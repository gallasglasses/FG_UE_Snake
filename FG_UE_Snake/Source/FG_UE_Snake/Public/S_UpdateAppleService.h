// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "S_UpdateAppleService.generated.h"


UCLASS()
class FG_UE_SNAKE_API US_UpdateAppleService : public UBTService
{
	GENERATED_BODY()

public:

    US_UpdateAppleService();

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};

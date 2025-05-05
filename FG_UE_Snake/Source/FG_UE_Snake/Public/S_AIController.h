// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "S_AIController.generated.h"

/**
 * 
 */
UCLASS()
class FG_UE_SNAKE_API AS_AIController : public AAIController
{
	GENERATED_BODY()

protected:
    virtual void OnPossess(APawn* InPawn) override;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USSPAIPerceptionComponent* AIPerceptionComponent;*/
	
};

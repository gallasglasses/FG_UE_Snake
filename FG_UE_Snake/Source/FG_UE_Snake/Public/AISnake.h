// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Snake.h"
#include "AISnake.generated.h"

class UBehaviorTree;

// should be parent class ABaseSnakePawn: HandleDeath(), EatApple(), ClearBody(), MoveChildSnakeBody()?


UCLASS()
class FG_UE_SNAKE_API AAISnake : public ASnake
{
	GENERATED_BODY()

public:

	AAISnake();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;
	
};

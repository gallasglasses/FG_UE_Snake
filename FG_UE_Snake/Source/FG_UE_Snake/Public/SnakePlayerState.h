// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FG_UE_CoreTypes.h"
#include "SnakePlayerState.generated.h"


UCLASS()
class FG_UE_SNAKE_API ASnakePlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller", meta = (AllowPrivateAccess = "true"))
	EGamePlayerControllerType ControllerType;

public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE EGamePlayerControllerType GetControllerType() const { return ControllerType; }

	UFUNCTION(BlueprintCallable)
	void SetControllerType(EGamePlayerControllerType Type);
};

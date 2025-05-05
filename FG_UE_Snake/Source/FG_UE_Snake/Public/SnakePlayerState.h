// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FG_UE_CoreTypes.h"
#include "SnakePlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnApplesEatenChangedSignature, int32, NewCount);

UCLASS()
class FG_UE_SNAKE_API ASnakePlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller", meta = (AllowPrivateAccess = "true"))
	EGamePlayerControllerType ControllerType;

	UPROPERTY(BlueprintReadOnly, Category = "PlayerData")
	int32 ApplesEaten = 0;

public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE EGamePlayerControllerType GetControllerType() const { return ControllerType; }

	UFUNCTION(BlueprintCallable)
	void SetControllerType(EGamePlayerControllerType Type);

	UPROPERTY(BlueprintAssignable, Category = "PlayerData")
	FOnApplesEatenChangedSignature OnApplesEatenChanged;

	UFUNCTION()
	void OnAppleCountChanged(AController* EatenBy);

	UFUNCTION(BlueprintCallable)
	void ResetAppleCount();

	int32 GetApplesEatenCount() const { return ApplesEaten; }

private:

};

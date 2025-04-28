// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "FG_UE_CoreTypes.h"
#include "S_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ASnake;
class ASnakePlayerState;

UCLASS()
class FG_UE_SNAKE_API AS_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputMappingContext> FirstPlayerMappingContext;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputMappingContext> SecondPlayerMappingContext;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> FirstPlayerMoveUpAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> FirstPlayerMoveRightAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> FirstPlayerMoveDownAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> FirstPlayerMoveLeftAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> FirstPlayerLookAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> SecondPlayerMoveUpAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> SecondPlayerMoveRightAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> SecondPlayerMoveDownAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> SecondPlayerMoveLeftAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> SecondPlayerLookAction;

	void FirstPlayerMoveUp(const FInputActionValue& Value);
	void FirstPlayerMoveRight(const FInputActionValue& Value);
	void FirstPlayerMoveDown(const FInputActionValue& Value);
	void FirstPlayerMoveLeft(const FInputActionValue& Value);
	void FirstPlayerLook(const FInputActionValue& Value);

	void SecondPlayerMoveUp(const FInputActionValue& Value);
	void SecondPlayerMoveRight(const FInputActionValue& Value);
	void SecondPlayerMoveDown(const FInputActionValue& Value);
	void SecondPlayerMoveLeft(const FInputActionValue& Value);
	void SecondPlayerLook(const FInputActionValue& Value);

protected:

	void OnPauseGame();

private:

	UPROPERTY()
	TObjectPtr<ASnake> Snake;

	UPROPERTY()
	TObjectPtr<ASnakePlayerState> SnakePlayerState;

	UFUNCTION()
	ASnake* GetPawnByKeyboard(int32 KeyboardID);

	virtual bool SetPause(bool bPause, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
	
	UFUNCTION()
	void OnGameStateChanged(EGameState PreviousState, EGameState NextState);

	UFUNCTION()
	void OnDead(ASnake* DeadSnake);
};

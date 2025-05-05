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

	UPROPERTY(EditDefaultsOnly, Transient, BlueprintReadOnly, Category = "Snake")
	TMap<EGamePlayerControllerType, UMaterialInterface*> SnakeMaterials;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputMappingContext> FirstPlayerMappingContext;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputMappingContext> SecondPlayerMappingContext;

	//++++++++++++++++++++++++++++++++++++++
	// FirstPlayerMove
	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> FirstPlayerMoveUpAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> FirstPlayerMoveRightAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> FirstPlayerMoveDownAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> FirstPlayerMoveLeftAction;

	void FirstPlayerMoveUp(const FInputActionValue& Value);
	void FirstPlayerMoveRight(const FInputActionValue& Value);
	void FirstPlayerMoveDown(const FInputActionValue& Value);
	void FirstPlayerMoveLeft(const FInputActionValue& Value);
	//++++++++++++++++++++++++++++++++++++++


	//++++++++++++++++++++++++++++++++++++++
	// FirstPlayerLook
	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> FirstPlayerShiftAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> FirstPlayerLookUpAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> FirstPlayerLookRightAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> FirstPlayerLookDownAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> FirstPlayerLookLeftAction;

	void FirstPlayerShiftPressed(const FInputActionValue& Value);
	void FirstPlayerShiftReleased(const FInputActionValue& Value);
	void FirstPlayerLookUp(const FInputActionValue& Value);
	void FirstPlayerLookRight(const FInputActionValue& Value);
	void FirstPlayerLookDown(const FInputActionValue& Value);
	void FirstPlayerLookLeft(const FInputActionValue& Value);
	//++++++++++++++++++++++++++++++++++++++


	//++++++++++++++++++++++++++++++++++++++
	// SecondPlayerMove
	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> SecondPlayerMoveUpAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> SecondPlayerMoveRightAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> SecondPlayerMoveDownAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> SecondPlayerMoveLeftAction;

	void SecondPlayerMoveUp(const FInputActionValue& Value);
	void SecondPlayerMoveRight(const FInputActionValue& Value);
	void SecondPlayerMoveDown(const FInputActionValue& Value);
	void SecondPlayerMoveLeft(const FInputActionValue& Value);
	//++++++++++++++++++++++++++++++++++++++


	//++++++++++++++++++++++++++++++++++++++
	// SecondPlayerLook
	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> SecondPlayerShiftAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> SecondPlayerLookUpAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> SecondPlayerLookRightAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> SecondPlayerLookDownAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
	TObjectPtr<UInputAction> SecondPlayerLookLeftAction;

	void SecondPlayerShiftPressed(const FInputActionValue& Value);
	void SecondPlayerShiftReleased(const FInputActionValue& Value);
	void SecondPlayerLookUp(const FInputActionValue& Value);
	void SecondPlayerLookRight(const FInputActionValue& Value);
	void SecondPlayerLookDown(const FInputActionValue& Value);
	void SecondPlayerLookLeft(const FInputActionValue& Value);
	//++++++++++++++++++++++++++++++++++++++

protected:

	void OnPauseGame();

private:

	bool bIsInputModeUI = false;

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
	void OnDead(AController* FallenController);

	bool bIsFirstPlayerShiftPressed = false;
	bool bIsSecondPlayerShiftPressed = false;

public:

	UFUNCTION()
	void ResetPlayerStateData();

	void ApplyPlayerStateData();
};

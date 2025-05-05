// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "S_MenuWidget.generated.h"

class UTextBlock;
class UButton;
class UWidgetSwitcher;
class USlider;
//class USoundCue;
class US_GameInstance;
class UBackToMenuWidget;
class AS_GameDataState;

UCLASS()
class FG_UE_SNAKE_API US_MenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

		void ShowOpening();

protected:

	virtual void NativeOnInitialized() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MenuText;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UButton* NewGameButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NewGameText;

	UPROPERTY(meta = (BindWidget))
	UButton* SingleplayerButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SingleplayerText;

	UPROPERTY(meta = (BindWidget))
	UButton* MultiplayerButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MultiplayerText;

	UPROPERTY(meta = (BindWidget))
	UButton* CoopButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CoopText;

	UPROPERTY(meta = (BindWidget))
	UButton* BattleButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BattleText;

	UPROPERTY(meta = (BindWidget))
	UButton* RealPlayerButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RealPlayerText;

	UPROPERTY(meta = (BindWidget))
	UButton* AIPlayerButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AIPlayerText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PercentApplesToEatText;

	UPROPERTY(meta = (BindWidget))
	USlider* PercentApplesToEatSlider;

	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StartGameText;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* QuitText;

	UPROPERTY(meta = (BindWidget))
	UBackToMenuWidget* BackToMenuWidget;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* OpeningAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* EndingAnimation;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* MenuSound;*/

private:

	UFUNCTION()
	void OnStartNewGame();

	UFUNCTION()
	void OnChooseSingleplayerGame();

	UFUNCTION()
	void OnChooseMultiplayerGame();

	UFUNCTION()
	void OnChooseCoopGame();

	UFUNCTION()
	void OnChooseBattleGame();

	UFUNCTION()
	void OnChooseRealPlayer();

	UFUNCTION()
	void OnChooseAIPlayer();

	UFUNCTION()
	void OnSliderValueChanged(float Value);

	UFUNCTION()
	void OnStartGame();

	UFUNCTION(BlueprintCallable)
	void OnBackToMenu();

	UFUNCTION()
	void OnQuitGame();

	US_GameInstance* GetThisGameInstance() const;
	AS_GameDataState* GetThisGameDataState() const;

	void LevelFinishedLoad(ULevel* Level, UWorld* World);
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InitWidget.generated.h"

class UTextBlock;
class UButton;
class UImage;
class UWidgetSwitcher;


UCLASS()
class FG_UE_SNAKE_API UInitWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void FadeInAnimation();

protected:

	virtual void NativeOnInitialized() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LoadingText;

	UPROPERTY(meta = (BindWidget))
	UButton* ContinueButton;

	UPROPERTY(meta = (BindWidget))
	UImage* LoadingImage;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* LoadingSwitcher;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeInAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeOutAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* LoadingAnim;

private:

	UFUNCTION()
	void OnStartGame();

	UFUNCTION()
	void LoadLevel();

	void LevelFinishedLoad(ULevel* Level, UWorld* World);
};

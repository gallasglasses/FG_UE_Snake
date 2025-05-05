// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "S_DeathWidget.generated.h"

class UTextBlock;
class UButton;
class UVerticalBox;

UCLASS()
class FG_UE_SNAKE_API US_DeathWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DeathText;

	UPROPERTY(meta = (BindWidget))
	UButton* RetryButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RetryText;

	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MainMenuText;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PlayersStatBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerStatRowWidgetClass;

private:

	UFUNCTION()
	void OnRetryLevel();

	UFUNCTION()
	void OnChooseMainMenu();

	void UpdatePlayersStat();

	UFUNCTION()
	void HandleDeadPlayer(AController* DeadController);
	
};

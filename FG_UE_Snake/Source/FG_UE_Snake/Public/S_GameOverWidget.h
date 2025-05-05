// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FG_UE_CoreTypes.h"
#include "S_GameOverWidget.generated.h"

class UTextBlock;
class UButton;
class UWidgetSwitcher;
class UVerticalBox;

UCLASS()
class FG_UE_SNAKE_API US_GameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual bool Initialize() override;

protected:

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelCompletedText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* FinishedGameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WinnerText;

	UPROPERTY(meta = (BindWidget))
	UButton* NextLevelButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NextLevelText;

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
	void OnChangeWidgetSwitcherIndex(EGameLevel NextLevel);

	UFUNCTION()
	void OnStartNextLevel();

	UFUNCTION()
	void OnChooseMainMenu();

	UFUNCTION()
	void HandleWinningPlayer(AController* Winner);

	void UpdatePlayersStat();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FG_UE_CoreTypes.h"
#include "S_GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class FG_UE_SNAKE_API AS_GameHUD : public AHUD
{
	GENERATED_BODY()

protected:

	UUserWidget* CreateWidgetByClass(const TSubclassOf<UUserWidget> GameWidgetClass, const EGameState State, const int32 ZOrder = 0);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TMap<EGameState, TSubclassOf<UUserWidget>> GameWidgets;

	virtual void BeginPlay() override;

	UPROPERTY()
	UUserWidget* CurrentWidget;

public:

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetCurrentWidget() const { return CurrentWidget; };

	UFUNCTION(BlueprintCallable)
	UUserWidget* ShowWidget(const EGameState WidgetState, const int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable)
	void HideWidget();

private:

	UFUNCTION()
	void OnGameStateChanged(EGameState PreviousState, EGameState NextState);
	
	void ShowInitWidget();
};

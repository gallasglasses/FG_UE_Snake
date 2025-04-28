// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BackToMenuWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBackToMenuClickedSignature);

UCLASS()
class FG_UE_SNAKE_API UBackToMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "BackToMenu")
	FOnBackToMenuClickedSignature OnBackToMenuClicked;

protected:

	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	UButton* BackToMenuButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BackToMenuText;

private:

	UFUNCTION()
	void HandleButtonClicked();
};

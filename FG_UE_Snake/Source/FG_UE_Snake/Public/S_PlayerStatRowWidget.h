// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "S_PlayerStatRowWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class FG_UE_SNAKE_API US_PlayerStatRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativePreConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (ClampMin = "0", ClampMax = "1"))
	int32 PlayerColorID = 0;

	void SetPlayerIndicatorVisibility(bool IsVisible);
	void SetPlayerIndicatorColor(int32 PlayerID);
	void SetPlayerName(const FText& Text);
	void SetAppleCount(const FText& Text);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FLinearColor DefaultColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FLinearColor Player1Color;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FLinearColor Player2Color;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AppleEatenCountText;

	UPROPERTY(meta = (BindWidget))
	UImage* PlayerIndicatorImage;
};

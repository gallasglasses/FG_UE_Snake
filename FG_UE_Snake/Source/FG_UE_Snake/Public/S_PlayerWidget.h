// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "S_PlayerWidget.generated.h"

class UTextBlock;

UCLASS()
class FG_UE_SNAKE_API US_PlayerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AppleCountText;

private:

	UFUNCTION()
	void HandleApplesCountChanged(int32 NewCount);
};

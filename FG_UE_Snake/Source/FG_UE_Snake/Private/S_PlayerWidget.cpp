// Fill out your copyright notice in the Description page of Project Settings.


#include "S_PlayerWidget.h"
#include "Components/TextBlock.h"
#include "Snake.h"
#include "SnakePlayerState.h"

void US_PlayerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

    if (APlayerController* PC = GetOwningPlayer())
    {
        if (ASnakePlayerState* PS = PC->GetPlayerState<ASnakePlayerState>())
        {
            HandleApplesCountChanged(0);
            PS->OnApplesEatenChanged.AddDynamic(this, &US_PlayerWidget::HandleApplesCountChanged);
        }
    }
}

void US_PlayerWidget::HandleApplesCountChanged(int32 NewCount)
{
    if (AppleCountText)
    {
        AppleCountText->SetText(FText::AsNumber(NewCount));
    }
}

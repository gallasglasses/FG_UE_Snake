// Fill out your copyright notice in the Description page of Project Settings.


#include "S_PlayerStatRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

DEFINE_LOG_CATEGORY_STATIC(US_PlayerStatRowWidgetLog, All, All);

void US_PlayerStatRowWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!IsValid(PlayerIndicatorImage)) return;
	//PlayerIndicatorImage->SetColorAndOpacity(DefaultColor);
	PlayerIndicatorImage->SetBrushTintColor(FSlateColor(DefaultColor));
}

void US_PlayerStatRowWidget::SetPlayerIndicatorVisibility(bool IsVisible)
{
	UE_LOG(US_PlayerStatRowWidgetLog, Display, TEXT("SetPlayerIndicatorVisibility"));
	if (!IsValid(PlayerIndicatorImage)) return;
	PlayerIndicatorImage->SetVisibility(IsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	UE_LOG(US_PlayerStatRowWidgetLog, Display, TEXT("%s IsVisible: %s"), *GetName(), IsVisible ? TEXT("TRUE") : TEXT("FALSE"));
}

void US_PlayerStatRowWidget::SetPlayerIndicatorColor(int32 PlayerID)
{
	UE_LOG(US_PlayerStatRowWidgetLog, Display, TEXT("SetPlayerIndicatorColor"));
	if (!IsValid(PlayerIndicatorImage))
	{
		UE_LOG(US_PlayerStatRowWidgetLog, Display, TEXT("!IsValid(PlayerIndicatorImage)"));
		return;
	}

	FLinearColor NewColor = (PlayerID == 0) ? Player1Color : Player2Color;
	//PlayerIndicatorImage->SetColorAndOpacity(NewColor);
	PlayerIndicatorImage->SetBrushTintColor(FSlateColor(NewColor));
	//PlayerIndicatorImage->SynchronizeProperties();

	UE_LOG(US_PlayerStatRowWidgetLog, Display, TEXT("PlayerID : %d"), PlayerID);
	UE_LOG(US_PlayerStatRowWidgetLog, Display, TEXT("SetPlayerIndicatorColor: %s"), *NewColor.ToString());
}

void US_PlayerStatRowWidget::SetPlayerName(const FText& Text)
{
	UE_LOG(US_PlayerStatRowWidgetLog, Display, TEXT("SetPlayerName"));
	if (!IsValid(PlayerNameText)) return;
	PlayerNameText->SetText(Text);
}

void US_PlayerStatRowWidget::SetAppleCount(const FText& Text)
{
	UE_LOG(US_PlayerStatRowWidgetLog, Display, TEXT("SetAppleCount"));
	if (!IsValid(AppleEatenCountText)) return;
	AppleEatenCountText->SetText(Text);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BackToMenuWidget.h"
#include "Components/Button.h"

void UBackToMenuWidget::NativeOnInitialized()
{
	if (BackToMenuButton)
	{
		BackToMenuButton->OnClicked.AddDynamic(this, &UBackToMenuWidget::HandleButtonClicked);
	}
}

void UBackToMenuWidget::HandleButtonClicked()
{
	OnBackToMenuClicked.Broadcast();
}

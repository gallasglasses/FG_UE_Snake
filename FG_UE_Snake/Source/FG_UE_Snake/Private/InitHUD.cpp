// Fill out your copyright notice in the Description page of Project Settings.


#include "InitHUD.h"
#include "InitWidget.h"

void AInitHUD::BeginPlay()
{
	Super::BeginPlay();

	if (InitWidgetClass)
	{
		InitWidget = CreateWidget<UInitWidget>(GetWorld(), InitWidgetClass);
		if (InitWidget)
		{
			InitWidget->AddToViewport();
			InitWidget->FadeInAnimation();
		}
	}
}

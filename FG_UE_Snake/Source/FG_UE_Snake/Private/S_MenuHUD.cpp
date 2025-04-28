// Fill out your copyright notice in the Description page of Project Settings.


#include "S_MenuHUD.h"
#include "S_MenuWidget.h"

void AS_MenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MenuWidgetClass)
	{
		const auto MenuWidget = CreateWidget<US_MenuWidget>(GetWorld(), MenuWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->AddToViewport();
			MenuWidget->ShowOpening();
		}
	}
}

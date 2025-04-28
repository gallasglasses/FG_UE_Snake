// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InitHUD.generated.h"

class UInitWidget;

UCLASS()
class FG_UE_SNAKE_API AInitHUD : public AHUD
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> InitWidgetClass;

	virtual void BeginPlay() override;

private:

	UInitWidget* InitWidget;
};

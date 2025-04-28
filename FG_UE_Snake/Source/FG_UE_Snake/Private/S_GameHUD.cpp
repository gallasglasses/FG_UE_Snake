// Fill out your copyright notice in the Description page of Project Settings.


#include "S_GameHUD.h"
#include "Blueprint/UserWidget.h"
#include "InitWidget.h"
#include "S_GameInstance.h"
#include "S_GameInstanceSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(AS_GameHUDLog, All, All);

UUserWidget* AS_GameHUD::CreateWidgetByClass(const TSubclassOf<UUserWidget> GameWidgetClass, const EGameState State, const int32 ZOrder)
{
	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), GameWidgetClass);
	if (CurrentWidget)
	{
		CurrentWidget->AddToViewport(ZOrder);
		if (State == EGameState::Init)
		{
			const auto LoadingWidget = Cast<UInitWidget>(CurrentWidget);
			if (!LoadingWidget) return nullptr;
			LoadingWidget->FadeInAnimation();
		}
	}
	return CurrentWidget;
}

void AS_GameHUD::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<UUserWidget>* GameWidgetClass = GameWidgets.Find(EGameState::Init);
	if (GameWidgetClass && *GameWidgetClass)
	{
		CreateWidgetByClass(*GameWidgetClass, EGameState::Init);
	}

	if (!GetWorld()) return;

	const US_GameInstance* GameInstance = GetWorld()->GetGameInstance<US_GameInstance>();
	if (!GameInstance) return;

	US_GameInstanceSubsystem* GameInstanceSubsystem = GameInstance->GetSubsystem<US_GameInstanceSubsystem>();
	if (!GameInstanceSubsystem) return;

	GameInstanceSubsystem->OnGameStateChanged.AddDynamic(this, &AS_GameHUD::OnGameStateChanged);
}

UUserWidget* AS_GameHUD::ShowWidget(const EGameState WidgetState, const int32 ZOrder)
{
	TSubclassOf<UUserWidget>* MenuWidgetClass = GameWidgets.Find(WidgetState);
	if (MenuWidgetClass && *MenuWidgetClass)
	{
		CreateWidgetByClass(*MenuWidgetClass, WidgetState, ZOrder);
	}
	return CurrentWidget;
}

void AS_GameHUD::HideWidget()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}
}

void AS_GameHUD::OnGameStateChanged(EGameState PreviousState, EGameState NextState)
{
	UE_LOG(AS_GameHUDLog, Display, TEXT("OnGameStateChanged"));
	HideWidget();
	ShowWidget(NextState);
}

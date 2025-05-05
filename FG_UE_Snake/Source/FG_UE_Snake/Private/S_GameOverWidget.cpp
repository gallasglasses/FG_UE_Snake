// Fill out your copyright notice in the Description page of Project Settings.


#include "S_GameOverWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "S_GameModeBase.h"
#include "S_GameInstance.h"
#include "SnakePlayerState.h"
#include "S_PlayerStatRowWidget.h"
#include "S_PlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(US_GameOverWidgetLog, All, All);

bool US_GameOverWidget::Initialize()
{
	if (!GetWorld()) return Super::Initialize();
	US_GameInstance* GameInstance = GetWorld()->GetGameInstance<US_GameInstance>();
	if (!GameInstance) return Super::Initialize();

	GameInstance->OnNextLevelChanged.AddDynamic(this, &US_GameOverWidget::OnChangeWidgetSwitcherIndex);

	return Super::Initialize();
}

void US_GameOverWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!IsValid(GetWorld())) return;

	const auto GameMode = Cast<AS_GameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->OnPlayerWon.AddDynamic(this, &US_GameOverWidget::HandleWinningPlayer);
	}

	if (NextLevelButton)
	{
		NextLevelButton->OnClicked.AddDynamic(this, &US_GameOverWidget::OnStartNextLevel);
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &US_GameOverWidget::OnChooseMainMenu);
	}
}

void US_GameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UpdatePlayersStat();
}

void US_GameOverWidget::OnStartNextLevel()
{
	if (!GetWorld()) return;
	const auto GameMode = Cast<AS_GameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->StartNextLevel();
	}
}

void US_GameOverWidget::OnChooseMainMenu()
{
	if (!GetWorld()) return;
	const auto GameMode = Cast<AS_GameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->StartNextLevel();
	}
}

void US_GameOverWidget::HandleWinningPlayer(AController* Winner)
{
	if (!IsValid(Winner)) return;

	UE_LOG(US_GameOverWidgetLog, Display, TEXT("Winner : %s"), *Winner->GetName());

	auto PS = Cast<ASnakePlayerState>(Winner->PlayerState);
	if (!PS) return;

	FString Name = TEXT("WINNER ");
	switch (PS->GetControllerType())
	{
	case EGamePlayerControllerType::Keyboard1:
		Name += TEXT("PLAYER 1");
		break;
	case EGamePlayerControllerType::Keyboard2:
		Name += TEXT("PLAYER 2");
		break;
	case EGamePlayerControllerType::AI:
		Name += TEXT("AI PLAYER");
		break;
	}

	if (WinnerText)
		WinnerText->SetText(FText::FromString(Name));
}

void US_GameOverWidget::UpdatePlayersStat()
{
	if (!IsValid(GetWorld()) || !IsValid(PlayersStatBox)) return;

	PlayersStatBox->ClearChildren();

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if (!IsValid(Controller)) continue;

		AS_PlayerController* PC = Cast<AS_PlayerController>(Controller);
		if (!IsValid(PC)) continue;

		const auto PS = Cast<ASnakePlayerState>(PC->PlayerState);
		if (!IsValid(PS)) continue;

		const auto PlayerStatRowWidget = CreateWidget<US_PlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
		if (!PlayerStatRowWidget) continue;

		//PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
		FString Name = "";
		switch (PS->GetControllerType())
		{
		case EGamePlayerControllerType::Keyboard1:
			Name = "PLAYER 1";
			break;
		case EGamePlayerControllerType::Keyboard2:
			Name = "PLAYER 2";
			break;
		case EGamePlayerControllerType::AI:
			Name = "AI PLAYER";
			break;
		}

		PlayerStatRowWidget->SetPlayerName(FText::FromString(Name));
		PlayerStatRowWidget->SetAppleCount(S_Utils::TextFromInt(PS->GetApplesEatenCount()));
		PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());

		UE_LOG(US_GameOverWidgetLog, Display, TEXT("PC->GetLocalPlayer()->GetLocalPlayerIndex() : %d"), PC->GetLocalPlayer()->GetLocalPlayerIndex());

		PlayersStatBox->AddChild(PlayerStatRowWidget);

		PlayerStatRowWidget->SetPlayerIndicatorColor(PC->GetLocalPlayer()->GetLocalPlayerIndex());
	}
}

void US_GameOverWidget::OnChangeWidgetSwitcherIndex(EGameLevel NextLevel)
{
	if (NextLevel == EGameLevel::MenuLevel)
	{
		if (!IsValid(LevelCompletedText)) return;
		FString Name = TEXT("YOU FINISHED THE GAME");

		LevelCompletedText->SetText(FText::FromString(Name));
		
		WidgetSwitcher->SetActiveWidgetIndex(1);
	}
	else
	{

		if (!IsValid(GetWorld()) || !IsValid(LevelCompletedText)) return;

		US_GameInstance* GameInstance = GetWorld()->GetGameInstance<US_GameInstance>();
		if (!GameInstance) return;

		FString Name = TEXT("LEVEL ");
		Name += FString::FromInt(static_cast<uint8>(GameInstance->GetCurrentLevel()));
		Name += TEXT(" COMPLETED");
		
		LevelCompletedText->SetText(FText::FromString(Name));

		WidgetSwitcher->SetActiveWidgetIndex(0);
	}
}

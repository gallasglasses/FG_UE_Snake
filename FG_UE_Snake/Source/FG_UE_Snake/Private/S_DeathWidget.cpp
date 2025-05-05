// Fill out your copyright notice in the Description page of Project Settings.


#include "S_DeathWidget.h"
#include "Components/Button.h"
#include "S_GameModeBase.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "SnakePlayerState.h"
#include "S_PlayerStatRowWidget.h"
#include "FG_UE_CoreTypes.h"
#include "S_PlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(US_DeathWidgetLog, All, All);

void US_DeathWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!IsValid(GetWorld())) return;

	const auto GameMode = Cast<AS_GameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->OnPlayerDied.AddDynamic(this, &US_DeathWidget::HandleDeadPlayer);
	}

	if (RetryButton)
	{
		RetryButton->OnClicked.AddDynamic(this, &US_DeathWidget::OnRetryLevel);
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &US_DeathWidget::OnChooseMainMenu);
	}
}

void US_DeathWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UpdatePlayersStat();
}

void US_DeathWidget::OnRetryLevel()
{
	if (!IsValid(GetWorld())) return;

	const auto GameMode = Cast<AS_GameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->StartNextLevel();
	}
}

void US_DeathWidget::OnChooseMainMenu()
{
	if (!IsValid(GetWorld())) return;
	const auto GameMode = Cast<AS_GameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->OnChooseMainMenu();
		GameMode->StartNextLevel();
	}
}

void US_DeathWidget::UpdatePlayersStat()
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

		UE_LOG(US_DeathWidgetLog, Display, TEXT("PC->GetLocalPlayer()->GetLocalPlayerIndex() : %d"), PC->GetLocalPlayer()->GetLocalPlayerIndex());

		PlayersStatBox->AddChild(PlayerStatRowWidget);
		
		PlayerStatRowWidget->SetPlayerIndicatorColor(PC->GetLocalPlayer()->GetLocalPlayerIndex());
    }
}

void US_DeathWidget::HandleDeadPlayer(AController* DeadController)
{
	if (!IsValid(DeadController)) return;

	auto PS = Cast<ASnakePlayerState>(DeadController->PlayerState);
	if (!PS) return;

	FString Name;
	switch (PS->GetControllerType())
	{
		case EGamePlayerControllerType::Keyboard1:
			Name = TEXT("PLAYER 1");
			break;
		case EGamePlayerControllerType::Keyboard2:
			Name = TEXT("PLAYER 2");
			break;
		case EGamePlayerControllerType::AI:
			Name = TEXT("AI PLAYER");
			break;
	}
	Name += TEXT(" DIED");

	if (DeathText)
		DeathText->SetText(FText::FromString(Name));
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "S_MenuWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "S_GameInstance.h"
#include "FG_UE_CoreTypes.h"
#include "BackToMenuWidget.h"
#include "S_MenuGameModeBase.h"
#include "S_GameDataState.h"
#include "S_GameDataSubsystem.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

DEFINE_LOG_CATEGORY_STATIC(MenuWidgetLog, All, All);

void US_MenuWidget::ShowOpening()
{
	PlayAnimation(OpeningAnimation);
	//UGameplayStatics::PlaySound2D(GetWorld(), MenuSound);
}

void US_MenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(NewGameButton))
	{
		NewGameButton->OnClicked.AddDynamic(this, &US_MenuWidget::OnStartNewGame);
	}

	if (IsValid(SingleplayerButton))
	{
		SingleplayerButton->OnClicked.AddDynamic(this, &US_MenuWidget::OnChooseSingleplayerGame);
	}

	if (IsValid(MultiplayerButton))
	{
		MultiplayerButton->OnClicked.AddDynamic(this, &US_MenuWidget::OnChooseMultiplayerGame);
	}

	if (IsValid(CoopButton))
	{
		CoopButton->OnClicked.AddDynamic(this, &US_MenuWidget::OnChooseCoopGame);
	}

	if (IsValid(BattleButton))
	{
		BattleButton->OnClicked.AddDynamic(this, &US_MenuWidget::OnChooseBattleGame);
	}

	if (IsValid(RealPlayerButton))
	{
		RealPlayerButton->OnClicked.AddDynamic(this, &US_MenuWidget::OnChooseRealPlayer);
	}

	if (IsValid(AIPlayerButton))
	{
		AIPlayerButton->OnClicked.AddDynamic(this, &US_MenuWidget::OnChooseAIPlayer);
	}

	if (IsValid(PercentApplesToEatSlider))
	{
		PercentApplesToEatSlider->OnValueChanged.AddDynamic(this, &US_MenuWidget::OnSliderValueChanged);
		PercentApplesToEatSlider->SetMinValue(0);
		PercentApplesToEatSlider->SetMaxValue(100);
		PercentApplesToEatSlider->SetStepSize(1);
		PercentApplesToEatSlider->SetValue(0);
	}

	if (IsValid(StartGameButton))
	{
		StartGameButton->OnClicked.AddDynamic(this, &US_MenuWidget::OnStartGame);
	}

	if (IsValid(QuitButton))
	{
		QuitButton->OnClicked.AddDynamic(this, &US_MenuWidget::OnQuitGame);
	}
	
	if (IsValid(BackToMenuWidget))
	{
		BackToMenuWidget->OnBackToMenuClicked.AddDynamic(this, &US_MenuWidget::OnBackToMenu);
	}
}

void US_MenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (Animation != EndingAnimation) return;

	if (!GetWorld()) return;

	if (!GetThisGameInstance()) return;

	if (GetThisGameInstance()->GetLevel(EGameLevel::PersistentLevel).IsNone())  //.IsNull() for TSoftObjectPtr
	{
		UE_LOG(MenuWidgetLog, Error, TEXT("Level name is NONE"));
		return;
	}

	//UGameplayStatics::OpenLevel(this, FName(GetThisGameInstance()->GetLevel(EGameLevel::InitLevel).ToSoftObjectPath().GetAssetName()));

	if (!GetThisGameInstance()) return;

	US_GameDataSubsystem* GameDataSubsystem = GetThisGameInstance()->GetSubsystem<US_GameDataSubsystem>();
	if (!GameDataSubsystem) return;

	GameDataSubsystem->SaveGameData();

	UGameplayStatics::OpenLevel(this, GetThisGameInstance()->GetLevel(EGameLevel::PersistentLevel));


	//FLatentActionInfo LatentInfo;
	//LatentInfo.UUID = 1;
	//LatentInfo.Linkage = LatentInfo.UUID;
	////UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(), GameInstance->GetLevel(GameInstance->GetNextLevel()), true, false, LatentInfo);
	//UGameplayStatics::LoadStreamLevel(GetWorld(), GetThisGameInstance()->GetLevel(EGameLevel::InitLevel), true, false, LatentInfo); //GameInstance->GetNextLevel()).ToSoftObjectPath().GetLongPackageName().RightChop(1)
	//FWorldDelegates::LevelAddedToWorld.AddUObject(this, &US_MenuWidget::LevelFinishedLoad);


	/*FLatentActionInfo LatentInfo;
	UGameplayStatics::UnloadStreamLevel(GetWorld(), FName("MenuLevel"), LatentInfo, false);*/
}

void US_MenuWidget::OnStartNewGame()
{
	MenuSwitcher->SetActiveWidgetIndex(1);
	BackToMenuWidget->SetVisibility(ESlateVisibility::Visible);
}

void US_MenuWidget::OnChooseSingleplayerGame()
{
	// OnChooseSingleplayerGame

	if(!GetThisGameDataState()) return;
	GetThisGameDataState()->SetGameMode(EGameMode::Singleplayer);

	/*if (!GetThisGameInstance()) return;
	GetThisGameInstance()->SetGameMode(EGameMode::Singleplayer);*/

	MenuSwitcher->SetActiveWidgetIndex(4);
	//OnStartGame();
}

void US_MenuWidget::OnChooseMultiplayerGame()
{
	MenuSwitcher->SetActiveWidgetIndex(2);
}

void US_MenuWidget::OnChooseCoopGame()
{
	// OnChooseCoopGame
	if (!GetThisGameDataState()) return;
	GetThisGameDataState()->SetGameMode(EGameMode::Cooperative);

	/*if (!GetThisGameInstance()) return;
	GetThisGameInstance()->SetGameMode(EGameMode::Cooperative);*/

	MenuSwitcher->SetActiveWidgetIndex(3);
}

void US_MenuWidget::OnChooseBattleGame()
{
	// OnChooseBattleGame
	if (!GetThisGameDataState()) return;
	GetThisGameDataState()->SetGameMode(EGameMode::Competitive);

	/*if (!GetThisGameInstance()) return;
	GetThisGameInstance()->SetGameMode(EGameMode::Competitive);*/

	MenuSwitcher->SetActiveWidgetIndex(3);
}

void US_MenuWidget::OnChooseRealPlayer()
{
	// ChooseRealPlayer
	if (!GetThisGameDataState()) return;
	GetThisGameDataState()->SetGamePlayer(EGamePlayer::Real);

	/*if (!GetThisGameInstance()) return;
	GetThisGameInstance()->SetGamePlayer(EGamePlayer::Real);*/

	MenuSwitcher->SetActiveWidgetIndex(4);
	//OnStartGame();
}

void US_MenuWidget::OnChooseAIPlayer()
{
	// ChooseAIPlayer
	if (!GetThisGameDataState()) return;
	GetThisGameDataState()->SetGamePlayer(EGamePlayer::AI);

	/*if (!GetThisGameInstance()) return;
	GetThisGameInstance()->SetGamePlayer(EGamePlayer::AI);*/

	MenuSwitcher->SetActiveWidgetIndex(4);
	//OnStartGame();
}

void US_MenuWidget::OnSliderValueChanged(float Value)
{
	if (IsValid(PercentApplesToEatText))
	{
		PercentApplesToEatText->SetText(S_Utils::TextFromInt(FMath::RoundToInt(Value)));
		if (!GetThisGameDataState()) return;
		GetThisGameDataState()->SetNumberOfApplesToEat(FMath::RoundToInt(Value));
		UE_LOG(MenuWidgetLog, Display, TEXT("SetNumberOfApplesToEat : %d"), FMath::RoundToInt(Value));
	}
}

void US_MenuWidget::OnStartGame()
{
	BackToMenuWidget->SetVisibility(ESlateVisibility::Hidden);

	if (!GetThisGameInstance()) return;
	GetThisGameInstance()->SetNextLevel(EGameLevel::Level1);

	PlayAnimation(EndingAnimation);
}

void US_MenuWidget::OnBackToMenu()
{
	BackToMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	MenuSwitcher->SetActiveWidgetIndex(0);
}

void US_MenuWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

US_GameInstance* US_MenuWidget::GetThisGameInstance() const
{
	US_GameInstance* GameInstance = Cast<US_GameInstance>(GetGameInstance());
	if (!GameInstance) return nullptr;

	return GameInstance;
}

AS_GameDataState* US_MenuWidget::GetThisGameDataState() const
{
	if (!GetWorld()) return nullptr;

	AS_GameDataState* GameDataState = GetWorld()->GetGameState<AS_GameDataState>();
	if (!GameDataState) return nullptr;

	return GameDataState;
}

void US_MenuWidget::LevelFinishedLoad(ULevel* Level, UWorld* World)
{
	FLatentActionInfo LatentInfo;
	UGameplayStatics::UnloadStreamLevel(GetWorld(), FName("MenuLevel"), LatentInfo, false);
}

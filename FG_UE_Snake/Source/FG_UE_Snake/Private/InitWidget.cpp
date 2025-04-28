// Fill out your copyright notice in the Description page of Project Settings.


#include "InitWidget.h"
#include "S_GameModeBase.h"
#include "S_GameInstance.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "FG_UE_CoreTypes.h"

DEFINE_LOG_CATEGORY_STATIC(InitWidgetLog, All, All);

void UInitWidget::FadeInAnimation()
{
	PlayAnimation(FadeInAnim); 
	PlayAnimation(LoadingAnim, 0.f, 0, EUMGSequencePlayMode::Forward, 1.f);

	if (!GetWorld()) return;

	US_GameInstance* GameInstance = GetWorld()->GetGameInstance<US_GameInstance>();
	if (!GameInstance) return;

	GameInstance->SetCurrentLevel(GameInstance->GetNextLevel());

	LoadLevel();
}

void UInitWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &UInitWidget::OnStartGame);
	}
}

void UInitWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (Animation != FadeOutAnim) return;
	UE_LOG(InitWidgetLog, Display, TEXT("OnAnimationFinished_Implementation"));
	if (!GetWorld()) return;

	const US_GameInstance* GameInstance = GetWorld()->GetGameInstance<US_GameInstance>();
	if (!GameInstance) return;

	///*if (GameInstance->GetNextLevel() == EGameLevel::MenuLevel)
	//{

	//}
	//else */
	//if (GameInstance->GetNextLevel() != EGameLevel::MenuLevel && GameInstance->GetNextLevel() != EGameLevel::InitLevel)
	//{
	//}
	
	const auto GameMode = Cast<AS_GameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		UE_LOG(InitWidgetLog, Display, TEXT("GameMode StartGame"));
		GameMode->StartGame();
	}

	/*FLatentActionInfo LatentInfo;
	UGameplayStatics::UnloadStreamLevel(GetWorld(), FName("InitLevel"), LatentInfo, false);*/
}

void UInitWidget::OnStartGame()
{
	UE_LOG(InitWidgetLog, Display, TEXT("PlayAnimation(FadeOutAnim)"));
	PlayAnimation(FadeOutAnim);
}

void UInitWidget::LoadLevel()
{
	if (!GetWorld()) return;
	const US_GameInstance* GameInstance = GetWorld()->GetGameInstance<US_GameInstance>();
	if (!GameInstance) return;

	if (GameInstance->GetLevel(GameInstance->GetNextLevel()).IsNone()) //.IsNull() for TSoftObjectPtr
	{
		UE_LOG(InitWidgetLog, Error, TEXT("Level name is NONE"));
		return;
	}

	FLatentActionInfo LatentInfo;
	LatentInfo.UUID = 1;
	LatentInfo.Linkage = LatentInfo.UUID;
	//UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(), GameInstance->GetLevel(GameInstance->GetNextLevel()), true, false, LatentInfo);
	UGameplayStatics::LoadStreamLevel(GetWorld(), GameInstance->GetLevel(GameInstance->GetNextLevel()), true, false, LatentInfo); //GameInstance->GetNextLevel()).ToSoftObjectPath().GetLongPackageName().RightChop(1)
	FWorldDelegates::LevelAddedToWorld.AddUObject(this, &UInitWidget::LevelFinishedLoad);
}

void UInitWidget::LevelFinishedLoad(ULevel* Level, UWorld* World)
{
	UE_LOG(InitWidgetLog, Display, TEXT("LevelFinishedLoad"));
	LoadingSwitcher->SetActiveWidgetIndex(1);
	StopAnimation(LoadingAnim);
	LoadingImage->RenderTransform.Angle = 0;
}

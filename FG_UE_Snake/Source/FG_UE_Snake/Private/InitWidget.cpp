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

	if (GameInstance->GetLevel(GameInstance->GetNextLevel()).IsNone()) //.IsNull() for TSoftObjectPtr
	{
		UE_LOG(InitWidgetLog, Error, TEXT("Level name is NONE"));
		return;
	}

	if (GameInstance->GetNextLevel() == EGameLevel::MenuLevel)
	{
		UE_LOG(InitWidgetLog, Error, TEXT("OpenLevel"));
		UGameplayStatics::OpenLevel(this, GameInstance->GetLevel(GameInstance->GetNextLevel()));
	}
	if (GameInstance->GetCurrentLevel() == EGameLevel::MenuLevel)
	{
		UE_LOG(InitWidgetLog, Error, TEXT("LoadGameLevel New Level"));
		LoadGameLevel(GameInstance->GetLevel(GameInstance->GetNextLevel()));
	}
	else
	{
		UE_LOG(InitWidgetLog, Error, TEXT("UnloadGameLevel"));
		UnloadGameLevel();
	}
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

void UInitWidget::LoadGameLevel(const FName NextLevel)
{
	if (!GetWorld()) return;
	//const US_GameInstance* GameInstance = GetWorld()->GetGameInstance<US_GameInstance>();
	//if (!GameInstance) return;

	//if (GameInstance->GetLevel(GameInstance->GetNextLevel()).IsNone()) //.IsNull() for TSoftObjectPtr
	//{
	//	UE_LOG(InitWidgetLog, Error, TEXT("Level name is NONE"));
	//	return;
	//}

	UE_LOG(InitWidgetLog, Display, TEXT("LoadGameLevel : %s"), *NextLevel.ToString());
	/*FLatentActionInfo LatentInfo;
	LatentInfo.UUID = NextLatentUUID;
	LatentInfo.Linkage = LatentInfo.UUID;
	++NextLatentUUID;*/

	FLatentActionInfo LoadInfo(NextLatentUUID, NextLatentUUID, TEXT("LevelFinishedLoad"), this);
	++NextLatentUUID;

	//UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(), GameInstance->GetLevel(GameInstance->GetNextLevel()), true, false, LatentInfo);
	UGameplayStatics::LoadStreamLevel(GetWorld(), NextLevel, true, false, LoadInfo); //GameInstance->GetNextLevel()).ToSoftObjectPath().GetLongPackageName().RightChop(1)
	//FWorldDelegates::LevelAddedToWorld.AddUObject(this, &UInitWidget::LevelFinishedLoad);
}

void UInitWidget::UnloadGameLevel()
{
	if (!GetWorld()) return;

	US_GameInstance* GameInstance = GetWorld()->GetGameInstance<US_GameInstance>();
	if (!GameInstance) return;

	if (GameInstance->GetLevel(GameInstance->GetCurrentLevel()).IsNone()) //.IsNull() for TSoftObjectPtr
	{
		UE_LOG(InitWidgetLog, Error, TEXT("Level name is NONE"));
		return;
	}
	if (GameInstance->GetCurrentLevel() == EGameLevel::MenuLevel)
	{
		UE_LOG(InitWidgetLog, Error, TEXT("UInitWidget::UnloadGameLevel GetCurrentLevel() == EGameLevel::MenuLevel"));
		return;
	}

	UE_LOG(InitWidgetLog, Display, TEXT("UnloadGameLevel : %s"), *GameInstance->GetLevel(GameInstance->GetCurrentLevel()).ToString());
	/*FLatentActionInfo LatentInfo;
	LatentInfo.UUID = NextLatentUUID;
	LatentInfo.Linkage = LatentInfo.UUID;
	++NextLatentUUID;*/

	FLatentActionInfo UnloadInfo(NextLatentUUID, NextLatentUUID, TEXT("LevelFinishedUnload"), this);
	++NextLatentUUID;
	UGameplayStatics::UnloadStreamLevel(GetWorld(), GameInstance->GetLevel(GameInstance->GetCurrentLevel()), UnloadInfo, false);
	//FWorldDelegates::LevelRemovedFromWorld.AddUObject(this, &UInitWidget::LevelFinishedUnload);
}

void UInitWidget::LevelFinishedLoad(ULevel* Level, UWorld* World)
{
	//UnloadGameLevel();

	if (!GetWorld()) return;

	US_GameInstance* GameInstance = GetWorld()->GetGameInstance<US_GameInstance>();
	if (!GameInstance) return;

	GameInstance->SetCurrentLevel(GameInstance->GetNextLevel());
	UE_LOG(InitWidgetLog, Display, TEXT("CurrentLevel : %s"),
		*StaticEnum<EGameLevel>()->GetNameStringByValue((int64)GameInstance->GetCurrentLevel()));

	UE_LOG(InitWidgetLog, Display, TEXT("LevelFinishedLoad"));
	LoadingSwitcher->SetActiveWidgetIndex(1);
	StopAnimation(LoadingAnim);
	LoadingImage->RenderTransform.Angle = 0;
}

void UInitWidget::LevelFinishedUnload(ULevel* Level, UWorld* World)
{
	if (!GetWorld()) return;

	US_GameInstance* GameInstance = GetWorld()->GetGameInstance<US_GameInstance>();
	if (!GameInstance) return;

	UE_LOG(InitWidgetLog, Display, TEXT("LevelFinishedUnload"));
	LoadGameLevel(GameInstance->GetLevel(GameInstance->GetNextLevel()));
}

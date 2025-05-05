// Fill out your copyright notice in the Description page of Project Settings.


#include "S_UpdateAppleService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "S_GameModeBase.h"
#include "S_GridManager.h"
#include "Kismet/GameplayStatics.h"

US_UpdateAppleService::US_UpdateAppleService()
{
    NodeName = TEXT("Update Apple");
    Interval = 1.0f;
}

void US_UpdateAppleService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    UWorld* World = OwnerComp.GetAIOwner()->GetWorld();
    if (!Blackboard || !World) return;

    AS_GameModeBase* GameMode = Cast<AS_GameModeBase>(UGameplayStatics::GetGameMode(World));
    if (!GameMode) return;

    AS_GridManager* GridManager = GameMode->GetGridManager();
    if (!GridManager) return;

    AActor* Apple = GridManager->GetSpawnedApple();
    if (Apple)
    {
        Blackboard->SetValueAsObject(FName("AppleActor"), Apple);
    }
}
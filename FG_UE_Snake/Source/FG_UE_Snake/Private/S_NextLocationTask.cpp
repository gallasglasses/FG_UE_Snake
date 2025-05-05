// Fill out your copyright notice in the Description page of Project Settings.


#include "S_NextLocationTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "S_GridManager.h"
#include "Tile.h"

#include "Kismet/GameplayStatics.h"


US_NextLocationTask::US_NextLocationTask()
{
	NodeName = TEXT("Next Location");
}

EBTNodeResult::Type US_NextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	AActor* Apple = Cast<AActor>(Blackboard->GetValueAsObject(TEXT("AppleActor")));

	if (!Controller || !Blackboard || !Apple) return EBTNodeResult::Failed;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	/*const auto NavSys = UNavigationSystemV1::GetCurrent(Pawn);
	if (!NavSys) return EBTNodeResult::Failed;

	FNavLocation NavLocation;
	auto PawnLocation = Pawn->GetActorLocation();*/

    AS_GridManager* GM = Cast<AS_GridManager>(UGameplayStatics::GetActorOfClass(Pawn->GetWorld(), AS_GridManager::StaticClass()));
    if (!GM) return EBTNodeResult::Failed;

    FVector2D StartIdx = GM->WorldLocationToGridIndex(Pawn->GetActorLocation());
    FVector2D GoalIdx = GM->WorldLocationToGridIndex(Apple->GetActorLocation());

    TArray<ATile*> PathTiles;
    if (!GM->FindPath(StartIdx, GoalIdx, PathTiles) || PathTiles.Num() < 2)
        return EBTNodeResult::Failed;

    ATile* NextTile = PathTiles[1];
    FVector  NextLoc = NextTile->GetActorLocation();
	NextLoc.Z = 100.f;
	Blackboard->SetValueAsVector(TEXT("TargetLocation"), NextLoc);

    return EBTNodeResult::Succeeded;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FG_UE_CoreTypes.h"
#include "SnakeBody.generated.h"

class UBoxComponent;

UCLASS()
class FG_UE_SNAKE_API ASnakeBody : public AActor
{
	GENERATED_BODY()
	
public:	

	ASnakeBody();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SnakeComponents")
	UBoxComponent* BodyCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SnakeComponents")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY()
	ASnakeBody* ChildSnakeBody = nullptr;

public:	

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	const FVector& GetLastLocation();

	UFUNCTION()
	void SetNextLocation(const FVector& Location, EDirectionState Direction);

	UFUNCTION()
	void AddChildSnakeBody(ASnakeBody* SnakeBody);

	UFUNCTION()
	void SetIsMoving(bool IsMoving);

	UFUNCTION()
	void HandleDestroy();

	void ChangeMeshMaterial(UMaterialInterface* Material);

private:

	bool bIsMoving;
	float MovementInterpolationTimer;
	float StepInterval;

	FVector CurrentLocation;
	FVector TargetLocation;

	void EnableCollision();
};

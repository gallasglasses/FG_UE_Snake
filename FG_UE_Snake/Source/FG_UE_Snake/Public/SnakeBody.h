// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	void SetNextLocation(const FVector& Location);

	UFUNCTION()
	void AddChildSnakeBody(ASnakeBody* SnakeBody);

	UFUNCTION()
	void SetIsMoving();

	UFUNCTION()
	void HandleDestroy();

private:

	bool bIsMoving;
	float MovementInterpolationTimer;
	float StepInterval;

	FVector CurrentLocation;
	FVector TargetLocation;

	void EnableCollision();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FG_UE_CoreTypes.h"
#include "Containers/Queue.h"

#include "Snake.generated.h"

class UBoxComponent;
class UCameraComponent;
class UInstancedStaticMeshComponent;
class UFloatingPawnMovement;
class USceneComponent;
class USphereComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class ASnakeBody;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeadSignature, ASnake*, DeadSnake);

UCLASS()
class FG_UE_SNAKE_API ASnake : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASnake();

protected:

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SnakeComponents")
		USceneComponent* SceneComponent;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SnakeComponents")
		UBoxComponent* HeadCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SnakeComponents")
		UStaticMeshComponent* HeadMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SnakeComponents")
		UStaticMeshComponent* RightEyeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SnakeComponents")
		UStaticMeshComponent* LeftEyeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SnakeComponents")
		UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SnakeComponents")
		USphereComponent* CameraCollisionComponent;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SnakeComponents")
		USpringArmComponent* SpringArmComponent;
		
	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SnakeComponents")
		UInstancedStaticMeshComponent* InstancedTailMesh;*/
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SnakeComponents")
		UFloatingPawnMovement* MovementComponent;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SnakeComponents")
		EDirectionState CurrentDirection;

	UPROPERTY(EditDefaultsOnly, Category = "SnakeSettings")
		TSubclassOf<ASnakeBody> SnakeBodyClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SnakeSettings")
		float MoveScale;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SnakeSettings")
		float StepInterval = 0.4f;

	UPROPERTY()
		ASnakeBody* ChildSnakeBody = nullptr;


	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintAssignable)
	FOnDeadSignature OnDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SnakeSettings")
	bool bIsFirstPlayer;

	virtual void Tick(float DeltaTime) override;

	void EatApple();
	void HandleDeath();
	void SetDirection(EDirectionState Direction);

	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	bool bIsMoving;
	bool bIsAppleEatenRightNow = false;

	int32 TileSize = -1;
	int32 PendingGrowth = 0;
	int32 DirectionQueueCount = 0;

	float AccumulatedTime;
	float MovementInterpolationTimer;

	FVector StartLocation;
	FVector TargetLocation;

	FRotator StartRotation;
	FRotator TargetRotation;

	TQueue<EDirectionState> DirectionQueue;
	EDirectionState LastQueuedDirection;

	void GrowSnakeBody();
	void HandleDirectionQueue();
	EDirectionState GetNextDirection();
};

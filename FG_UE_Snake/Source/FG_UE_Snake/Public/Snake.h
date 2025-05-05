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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeadSignature, AController*, DeadController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAppleEatenSignature, AController*, Instigator);

UCLASS()
class FG_UE_SNAKE_API ASnake : public APawn
{
	GENERATED_BODY()

public:

	ASnake();

protected:

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
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SnakeComponents")
		UFloatingPawnMovement* MovementComponent;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SnakeComponents")
		EDirectionState CurrentDirection;

	UPROPERTY(EditDefaultsOnly, Category = "SnakeSettings")
		TSubclassOf<ASnakeBody> SnakeBodyClass;

	UPROPERTY(EditAnywhere, Category = "SnakeSettings")
	UMaterialParameterCollection* MPC_Occlusion;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SnakeSettings")
		float MoveScale;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SnakeSettings")
		float StepInterval = 0.4f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SnakeSettings")
	FName MaterialActorParameterName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SnakeSettings")
	FName MaterialCameraParameterName;

	UPROPERTY()
		ASnakeBody* ChildSnakeBody = nullptr;


	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetMaterialParameters();

public:

	UPROPERTY(BlueprintAssignable)
	FOnDeadSignature OnDead;

	UPROPERTY(BlueprintAssignable)
	FOnAppleEatenSignature OnAppleEaten;

	virtual void Tick(float DeltaTime) override;

	void EatApple();
	void HandleDeath();
	void ClearBody();
	void SetDirection(EDirectionState Direction);
	void RotateCamera(EDirectionState Direction);
	void ChangeMeshMaterial(UMaterialInterface* Material);
	void SetMaterialParameterNames(FName ActorParameter, FName CameraParameter);

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
	TSet<AActor*> OccludingObjects;

	void GrowSnakeBody();
	void HandleDirectionQueue();

	EDirectionState GetNextDirection();
};

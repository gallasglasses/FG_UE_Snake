// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"

#include "Snake.generated.h"

class UBoxComponent;
class UCameraComponent;
class UInstancedStaticMeshComponent;
class USceneComponent;
class USphereComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;

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
		UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SnakeComponents")
		USphereComponent* CameraCollisionComponent;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SnakeComponents")
		USpringArmComponent* SpringArmComponent;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SnakeComponents")
		UInstancedStaticMeshComponent* InstancedTailMesh;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
		UInputMappingContext* WASDMappingContext;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
		UInputMappingContext* ArrowsMappingContext;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
		UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = "SnakeInput")
		UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SnakePlayer")
		bool bIsFirstPlayer;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

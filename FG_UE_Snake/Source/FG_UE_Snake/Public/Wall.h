// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wall.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class FG_UE_SNAKE_API AWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWall();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Wall")
	UBoxComponent* WallCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Wall")
	UStaticMeshComponent* WallMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};

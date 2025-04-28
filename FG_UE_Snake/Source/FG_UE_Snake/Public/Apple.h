// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Apple.generated.h"

class UBoxComponent;

DECLARE_MULTICAST_DELEGATE(FOnAppleDestroyedSignature);

UCLASS()
class FG_UE_SNAKE_API AApple : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AApple();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Apple")
	UBoxComponent* AppleCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Apple")
	UStaticMeshComponent* AppleMesh;

	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	FOnAppleDestroyedSignature OnAppleDestroyed;

};

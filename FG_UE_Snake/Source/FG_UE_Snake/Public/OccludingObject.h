#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OccludingObject.generated.h"


UINTERFACE(MinimalAPI, Blueprintable)
class UOccludingObject : public UInterface
{
	GENERATED_BODY()
};


class FG_UE_SNAKE_API IOccludingObject
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be 
	//inherited to implement this interface.

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Occlusion")
	void ApplyMaskOccludingObject(float MaskParameter);
	
};
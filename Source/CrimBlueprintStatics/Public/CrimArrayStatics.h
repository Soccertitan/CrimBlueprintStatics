// Copyright Soccertitan

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CrimArrayStatics.generated.h"

/**
 * 
 */
UCLASS()
class CRIMBLUEPRINTSTATICS_API UCrimArrayStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Makes a copy of the array that only includes objects of the specified Class.
	 * @param Objects The array of objects to filter.
	 * @param Class The class the objects must be to be added to the output.
	 * @return A copy of the array with the transformed objects.
	 */
	UFUNCTION(BlueprintCallable, Category = "Crim Blueprint Statics|Array", DisplayName = TransformArrayIf, meta = (DeterminesOutputType = Class))
	static TArray<UObject*> K2_TransformArrayIf(const TArray<UObject*>& Objects, TSubclassOf<UObject> Class);
};

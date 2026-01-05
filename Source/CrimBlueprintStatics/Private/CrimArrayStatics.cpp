// Copyright Soccertitan 2025


#include "CrimArrayStatics.h"

TArray<UObject*> UCrimArrayStatics::K2_TransformArrayIf(const TArray<UObject*>& Objects, TSubclassOf<UObject> Class)
{
	TArray<UObject*> Result;

	for (int32 idx = Objects.Num() - 1; idx >= 0; idx--)
	{
		if (Objects[idx] && Objects[idx]->IsA(Class))
		{
			Result.Add(Objects[idx]);
		}
	}

	return Result;
}

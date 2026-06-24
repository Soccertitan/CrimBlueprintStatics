// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SkeletalMergingLibrary.h"
#include "SkeletalMeshMerge.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CrimSkeletalMergingLibrary.generated.h"

/** 
 * Maps a skeletal mesh to a tagged slot.
 */
USTRUCT(BlueprintType)
struct CRIMBLUEPRINTSTATICS_API FCrimSkeletalMeshMap
{
	GENERATED_BODY()

	FCrimSkeletalMeshMap()
	{
		Slot = FGameplayTag();
		Mesh = nullptr;
	}

	/** The slot to map this mesh to. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Slot;

	/** The mesh to merge to the skeleton. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh;
};

/**
* Struct that merge meshes based on a base and dynamic set  of meshes.
*/
USTRUCT(BlueprintType)
struct CRIMBLUEPRINTSTATICS_API FCrimSkeletalMeshMergeParams
{
	GENERATED_BODY()

	FCrimSkeletalMeshMergeParams()
	{
		BaseMeshesToMerge = TArray<FCrimSkeletalMeshMap>();
		DynamicMeshesToMerge = TArray<FCrimSkeletalMeshMap>();
	}

	// The default meshes to merge.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCrimSkeletalMeshMap> BaseMeshesToMerge;

	// Prioritizes entries in DynamicMeshesToMerge if the Slot Tags are equal to a BaseMesh entry.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCrimSkeletalMeshMap> DynamicMeshesToMerge;
	
	// An optional array to transform the UVs in each mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSkeletalMeshMergeParams SkeletalMeshMergeParams;
};

/**
 * 
 */
UCLASS()
class CRIMBLUEPRINTSTATICS_API UCrimSkeletalMergingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Updates the Params with the union of Base and Dynamic meshes and then merges the meshes. */
	UFUNCTION(BlueprintCallable, Category = "Mesh Merge")
	static USkeletalMesh* MergeMeshes(FCrimSkeletalMeshMergeParams& Params);
	
	/**
	 * Adds a new mesh to the DynamicMeshesToMerge variable. If an entry with the same tag already exists it will replace it.
	 */
	UFUNCTION(BlueprintCallable, Category = "Mesh Merge")
	static void AddDynamicMesh(FCrimSkeletalMeshMergeParams& Params, const FCrimSkeletalMeshMap& Map);

	/**
	 * Removes the entry in the DynamicMeshesToMerge that matches the tag.
	 */
	UFUNCTION(BlueprintCallable, Category = "Mesh Merge")
	static void RemoveDynamicMesh(FCrimSkeletalMeshMergeParams& Params, FGameplayTag Tag);
};

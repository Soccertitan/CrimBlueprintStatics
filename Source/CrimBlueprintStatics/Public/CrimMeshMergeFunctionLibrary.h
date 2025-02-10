// Copyright Soccertitan

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SkeletalMeshMerge.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CrimMeshMergeFunctionLibrary.generated.h"

USTRUCT(BlueprintType)
struct CRIMBLUEPRINTSTATICS_API FSkeletalMeshMap
{
	GENERATED_BODY()

	FSkeletalMeshMap()
	{
		Slot = FGameplayTag();
		Mesh = nullptr;
	}

	/**
	 * The slot to map this mesh to.
	 */
	UPROPERTY(EditAnywhere)
	FGameplayTag Slot;

	/**
	 * The mesh to merge to the skeleton.
	 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> Mesh;
};

/**
* Struct containing all parameters used to perform a Skeletal Mesh merge.
*/
USTRUCT(BlueprintType)
struct CRIMBLUEPRINTSTATICS_API FSkeletalMeshMergeParams
{
	GENERATED_BODY()

	FSkeletalMeshMergeParams()
	{
		StripTopLODS = 0;
		bNeedsCpuAccess = false;
		bSkeletonBefore = false;
		Skeleton = nullptr;
	}

	// The default meshes to merge.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, NotReplicated)
	TArray<FSkeletalMeshMap> BaseMeshesToMerge;

	// Prioritizes entries in DynamicMeshesToMerge if the Slot Tags are equal to a BaseMesh entry.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSkeletalMeshMap> DynamicMeshesToMerge;
	
	// An optional array to transform the UVs in each mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NotReplicated)
	TArray<FSkelMeshMergeUVTransformMapping> UVTransformsPerMesh;
	
	// The number of high LODs to remove from input meshes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NotReplicated)
	int32 StripTopLODS;

	// Whether or not the resulting mesh needs to be accessed by the CPU for any reason (e.g. for spawning particle effects).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NotReplicated)
	uint32 bNeedsCpuAccess : 1;

	/**
	 * Update skeleton before merge. Otherwise, update after.
	 * Skeleton must also be provided.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NotReplicated)
	uint32 bSkeletonBefore : 1;

	/**
	 * Skeleton that will be used for the merged mesh.
	 * Leave empty if the generated skeleton is OK.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, NotReplicated)
	TObjectPtr<USkeleton> Skeleton;
};

/**
 * 
 */
UCLASS()
class CRIMBLUEPRINTSTATICS_API UCrimMeshMergeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	/**
	* Merges the given meshes into a single mesh.
	* @return The merged mesh (will be invalid if the merge failed).
	*/
	UFUNCTION(BlueprintCallable, Category = "Mesh Merge", meta = (UnsafeDuringActorConstruction = "true"))
	static USkeletalMesh* MergeMeshes(const FSkeletalMeshMergeParams& Params);

	/**
	 * Adds a new mesh to the DynamicMeshesToMerge variable. If an entry with the same tag already exists it will replace it.
	 */
	UFUNCTION(BlueprintCallable, Category = "Mesh Merge")
	static void AddDynamicMesh(FSkeletalMeshMergeParams& Params, const FSkeletalMeshMap& Map);

	/**
	 * Removes the entry in the DynamicMeshesToMerge that matches the tag.
	 */
	UFUNCTION(BlueprintCallable, Category = "Mesh Merge")
	static void RemoveDynamicMesh(FSkeletalMeshMergeParams& Params, FGameplayTag Tag);
};

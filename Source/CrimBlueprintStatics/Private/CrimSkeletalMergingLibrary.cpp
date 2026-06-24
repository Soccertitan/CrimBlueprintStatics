// Copyright Soccertitan 2025


#include "CrimSkeletalMergingLibrary.h"


USkeletalMesh* UCrimSkeletalMergingLibrary::MergeMeshes(FCrimSkeletalMeshMergeParams& Params)
{
	TArray<FCrimSkeletalMeshMap> MeshesToMergeCopy;
	for (const FCrimSkeletalMeshMap& Entry : Params.BaseMeshesToMerge)
	{
		if (Entry.Mesh)
		{
			MeshesToMergeCopy.Add(Entry);
		}
	}
	for (const FCrimSkeletalMeshMap& Entry : Params.DynamicMeshesToMerge)
	{
		if (Entry.Mesh == nullptr)
		{
			continue;
		}
		
		bool bFoundDefault = false;
		for (FCrimSkeletalMeshMap& Item : MeshesToMergeCopy)
		{
			if (Entry.Slot.MatchesTag(Item.Slot))
			{
				Item.Mesh = Entry.Mesh;
				bFoundDefault = true;
				break;
			}
		}
		if (!bFoundDefault)
		{
			MeshesToMergeCopy.Add(Entry);
		}
	}

	Params.SkeletalMeshMergeParams.MeshesToMerge.Empty(MeshesToMergeCopy.Num());
	for (FCrimSkeletalMeshMap& Entry : MeshesToMergeCopy)
	{
		Params.SkeletalMeshMergeParams.MeshesToMerge.Add(Entry.Mesh);
	}
	
	return USkeletalMergingLibrary::MergeMeshes(Params.SkeletalMeshMergeParams);
}

void UCrimSkeletalMergingLibrary::AddDynamicMesh(FCrimSkeletalMeshMergeParams& Params, const FCrimSkeletalMeshMap& Map)
{
	for (FCrimSkeletalMeshMap& Entry : Params.DynamicMeshesToMerge)
	{
		if (Entry.Slot.MatchesTag(Map.Slot))
		{
			Entry.Mesh = Map.Mesh;
			return;
		}
	}
	Params.DynamicMeshesToMerge.Add(Map);
}

void UCrimSkeletalMergingLibrary::RemoveDynamicMesh(FCrimSkeletalMeshMergeParams& Params, FGameplayTag Tag)
{
	if (Params.DynamicMeshesToMerge.IsEmpty())
	{
		return;
	}
	
	for (int32 i = Params.DynamicMeshesToMerge.Num() - 1; i >= 0; i--)
	{
		if (Params.DynamicMeshesToMerge[i].Slot.MatchesTag(Tag))
		{
			Params.DynamicMeshesToMerge.RemoveAt(i);
		}
	}
}

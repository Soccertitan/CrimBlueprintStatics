// Copyright Soccertitan


#include "CrimMeshMergeFunctionLibrary.h"

#include "SkeletalMeshMerge.h"
#include "Engine/SkeletalMeshSocket.h"

USkeletalMesh* UCrimMeshMergeFunctionLibrary::MergeMeshes(const FSkeletalMeshMergeParams& Params)
{
	TArray<FSkeletalMeshMap> MeshesToMergeCopy;
	for (const FSkeletalMeshMap& Entry : Params.BaseMeshesToMerge)
	{
		if (Entry.Mesh)
		{
			MeshesToMergeCopy.Add(Entry);
		}
	}
	for (const FSkeletalMeshMap& Entry : Params.DynamicMeshesToMerge)
	{
		if (Entry.Mesh == nullptr)
		{
			continue;
		}
		
		bool bFoundDefault = false;
		for (auto& Item : MeshesToMergeCopy)
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

	TArray<USkeletalMesh*> MeshesToMerge;
	for (FSkeletalMeshMap& Entry : MeshesToMergeCopy)
	{
		MeshesToMerge.Add(Entry.Mesh);
	}
	if (MeshesToMerge.Num() <= 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Must provide multiple valid Skeletal Meshes in order to perform a merge."));
		return nullptr;
	}

	EMeshBufferAccess BufferAccess = Params.bNeedsCpuAccess ? EMeshBufferAccess::ForceCPUAndGPU : EMeshBufferAccess::Default;
	bool bRunDuplicateCheck = false;
	USkeletalMesh* BaseMesh = NewObject<USkeletalMesh>();
	if (Params.Skeleton && Params.bSkeletonBefore)
	{
		BaseMesh->SetSkeleton(Params.Skeleton);
		bRunDuplicateCheck = true;
		for (USkeletalMeshSocket* Socket : BaseMesh->GetMeshOnlySocketList())
		{
			if (Socket)
			{
				UE_LOG(LogTemp, Warning, TEXT("SkelMeshSocket: %s"), *(Socket->SocketName.ToString()));
			}
		}

		for (USkeletalMeshSocket* Socket : BaseMesh->GetSkeleton()->Sockets)
		{
			if (Socket)
			{
				UE_LOG(LogTemp, Warning, TEXT("SkelSocket: %s"), *(Socket->SocketName.ToString()));
			}
		}
	}
	TArray<FSkelMeshMergeSectionMapping> SectionMappings;
	FSkeletalMeshMerge Merger(BaseMesh, MeshesToMerge, SectionMappings, Params.StripTopLODS, BufferAccess, Params.UVTransformsPerMesh.GetData());
	if (!Merger.DoMerge())
	{
		UE_LOG(LogTemp, Warning, TEXT("Merge failed!"));
		return nullptr;
	}

	if (Params.Skeleton && !Params.bSkeletonBefore)
	{
		BaseMesh->SetSkeleton(Params.Skeleton);
	}

	if (bRunDuplicateCheck)
	{
		TArray<FName> SkelMeshSockets;
		TArray<FName> SkelSockets;
		for (USkeletalMeshSocket* Socket : BaseMesh->GetMeshOnlySocketList())
		{
			if (Socket)
			{
				SkelMeshSockets.Add(Socket->GetFName());
				UE_LOG(LogTemp, Warning, TEXT("SkelMeshSocket: %s"), *(Socket->SocketName.ToString()));
			}
		}

		for (USkeletalMeshSocket* Socket : BaseMesh->GetSkeleton()->Sockets)
		{
			if (Socket)
			{
				SkelSockets.Add(Socket->GetFName());
				UE_LOG(LogTemp, Warning, TEXT("SkelSocket: %s"), *(Socket->SocketName.ToString()));
			}
		}
	    
		TSet<FName> UniqueSkelMeshSockets;
		TSet<FName> UniqueSkelSockets;
		UniqueSkelMeshSockets.Append(SkelMeshSockets);
		UniqueSkelSockets.Append(SkelSockets);
		int32 Total = SkelSockets.Num() + SkelMeshSockets.Num();
		int32 UniqueTotal = UniqueSkelMeshSockets.Num() + UniqueSkelSockets.Num();
		UE_LOG(LogTemp, Warning, TEXT("SkelMeshSocketCount: %d | SkelSocketCount: %d | Combined: %d"), SkelMeshSockets.Num(), SkelSockets.Num(), Total);
		UE_LOG(LogTemp, Warning, TEXT("SkelMeshSocketCount: %d | SkelSocketCount: %d | Combined: %d"), UniqueSkelMeshSockets.Num(), UniqueSkelSockets.Num(), UniqueTotal);
		UE_LOG(LogTemp, Warning, TEXT("Found Duplicates: %s"), *((Total != UniqueTotal) ? FString("True") : FString("False")));
	}

	return BaseMesh;
}

void UCrimMeshMergeFunctionLibrary::AddDynamicMesh(FSkeletalMeshMergeParams& Params, const FSkeletalMeshMap& Map)
{
	for (FSkeletalMeshMap& Entry : Params.DynamicMeshesToMerge)
	{
		if (Entry.Slot.MatchesTag(Map.Slot))
		{
			Entry.Mesh = Map.Mesh;
			return;
		}
	}
	Params.DynamicMeshesToMerge.Add(Map);
}

void UCrimMeshMergeFunctionLibrary::RemoveDynamicMesh(FSkeletalMeshMergeParams& Params, FGameplayTag Tag)
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

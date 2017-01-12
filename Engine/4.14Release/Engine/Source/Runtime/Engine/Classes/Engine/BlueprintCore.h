// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BlueprintCore.generated.h"


UCLASS()
class ENGINE_API UBlueprintCore
	: public UObject
{
	GENERATED_UCLASS_BODY()

	/** Pointer to the skeleton class; this is regenerated any time a member variable or function is added but  
	is usually incomplete (no code or hidden autogenerated variables are added to it) */
	UPROPERTY(nontransactional, transient)
	TSubclassOf<class UObject>  SkeletonGeneratedClass;

	/** Pointer to the 'most recent' fully generated class */
	UPROPERTY(nontransactional)
	TSubclassOf<class UObject>	GeneratedClass;

	/** BackCompat:  Whether or not we need to purge references in this blueprint to the skeleton generated during compile-on-load  */
	UPROPERTY()
	bool bLegacyNeedToPurgeSkelRefs;

private:

	/** BackCompat: Whether or not this blueprint's authoritative CDO data has been migrated from the SkeletonGeneratedClass CDO to the GeneratedClass CDO */
	UPROPERTY()
	bool bLegacyGeneratedClassIsAuthoritative;

	/** Blueprint Guid */
	UPROPERTY()
	FGuid BlueprintGuid;

public:

	void SetLegacyGeneratedClassIsAuthoritative()
	{
		bLegacyGeneratedClassIsAuthoritative = true;
	}

	bool IsGeneratedClassAuthoritative()
	{
		return bLegacyGeneratedClassIsAuthoritative;
	}

	virtual void Serialize( FArchive& Ar ) override;

#if WITH_EDITORONLY_DATA
	/** Gets asset registry tags */
	virtual void GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const override;
#endif

	/** Generates a new blueprint Guid, used when creating new blueprints */
	void GenerateNewGuid()
	{
		BlueprintGuid = FGuid::NewGuid();
	}

	/** Gets the Blueprint Guid */
	const FGuid& GetBlueprintGuid() const { return BlueprintGuid; }

private:
	
	/** Generates a new deterministic guid based on blueprint properties */
	void GenerateDeterministicGuid();
};

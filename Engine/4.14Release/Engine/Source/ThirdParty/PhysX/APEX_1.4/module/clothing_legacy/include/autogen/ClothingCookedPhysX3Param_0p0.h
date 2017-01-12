// This code contains NVIDIA Confidential Information and is disclosed to you
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and
// any modifications thereto. Any use, reproduction, disclosure, or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA Corporation is strictly prohibited.
//
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2008-2015 NVIDIA Corporation. All rights reserved.

// This file was generated by NvParameterized/scripts/GenParameterized.pl


#ifndef HEADER_ClothingCookedPhysX3Param_0p0_h
#define HEADER_ClothingCookedPhysX3Param_0p0_h

#include "NvParametersTypes.h"

#ifndef NV_PARAMETERIZED_ONLY_LAYOUTS
#include "nvparameterized/NvParameterized.h"
#include "nvparameterized/NvParameterizedTraits.h"
#include "NvParameters.h"
#include "NvTraitsInternal.h"
#endif

namespace nvidia
{
namespace parameterized
{

#if PX_VC
#pragma warning(push)
#pragma warning(disable: 4324) // structure was padded due to __declspec(align())
#endif

namespace ClothingCookedPhysX3Param_0p0NS
{

struct PhaseDesc_Type;
struct SetDesc_Type;

struct F32_DynamicArray1D_Type
{
	float* buf;
	bool isAllocated;
	int32_t elementSize;
	int32_t arraySizes[1];
};

struct U32_DynamicArray1D_Type
{
	uint32_t* buf;
	bool isAllocated;
	int32_t elementSize;
	int32_t arraySizes[1];
};

struct SetDesc_DynamicArray1D_Type
{
	SetDesc_Type* buf;
	bool isAllocated;
	int32_t elementSize;
	int32_t arraySizes[1];
};

struct PhaseDesc_DynamicArray1D_Type
{
	PhaseDesc_Type* buf;
	bool isAllocated;
	int32_t elementSize;
	int32_t arraySizes[1];
};

struct SetDesc_Type
{
	uint32_t fiberEnd;
	uint32_t longestFiber;
	uint32_t shortestFiber;
	uint32_t numEdges;
	float avgEdgeLength;
	uint32_t avgFiberLength;
};
struct PhaseDesc_Type
{
	uint32_t phaseType;
	uint32_t setIndex;
	uint32_t restValueOffset;
};

struct ParametersStruct
{

	uint32_t physicalMeshId;
	uint32_t physicalSubMeshId;
	uint32_t numVertices;
	F32_DynamicArray1D_Type deformableRestLengths;
	U32_DynamicArray1D_Type deformableIndices;
	U32_DynamicArray1D_Type deformableFibers;
	SetDesc_DynamicArray1D_Type deformableSets;
	PhaseDesc_DynamicArray1D_Type deformablePhaseDescs;
	U32_DynamicArray1D_Type tetherAnchors;
	F32_DynamicArray1D_Type tetherLengths;
	F32_DynamicArray1D_Type deformableInvVertexWeights;
	U32_DynamicArray1D_Type virtualParticleIndices;
	F32_DynamicArray1D_Type virtualParticleWeights;
	uint32_t cookedDataVersion;
	void* fabricCPU;
	void* fabricGPU;
	NvParameterized::Interface* nextCookedData;

};

static const uint32_t checksum[] = { 0x22beeaa3, 0xe15f304d, 0xe1ac2daf, 0x3f6ec0e9, };

} // namespace ClothingCookedPhysX3Param_0p0NS

#ifndef NV_PARAMETERIZED_ONLY_LAYOUTS
class ClothingCookedPhysX3Param_0p0 : public NvParameterized::NvParameters, public ClothingCookedPhysX3Param_0p0NS::ParametersStruct
{
public:
	ClothingCookedPhysX3Param_0p0(NvParameterized::Traits* traits, void* buf = 0, int32_t* refCount = 0);

	virtual ~ClothingCookedPhysX3Param_0p0();

	virtual void destroy();

	static const char* staticClassName(void)
	{
		return("ClothingCookedPhysX3Param");
	}

	const char* className(void) const
	{
		return(staticClassName());
	}

	static const uint32_t ClassVersion = ((uint32_t)0 << 16) + (uint32_t)0;

	static uint32_t staticVersion(void)
	{
		return ClassVersion;
	}

	uint32_t version(void) const
	{
		return(staticVersion());
	}

	static const uint32_t ClassAlignment = 8;

	static const uint32_t* staticChecksum(uint32_t& bits)
	{
		bits = 8 * sizeof(ClothingCookedPhysX3Param_0p0NS::checksum);
		return ClothingCookedPhysX3Param_0p0NS::checksum;
	}

	static void freeParameterDefinitionTable(NvParameterized::Traits* traits);

	const uint32_t* checksum(uint32_t& bits) const
	{
		return staticChecksum(bits);
	}

	const ClothingCookedPhysX3Param_0p0NS::ParametersStruct& parameters(void) const
	{
		ClothingCookedPhysX3Param_0p0* tmpThis = const_cast<ClothingCookedPhysX3Param_0p0*>(this);
		return *(static_cast<ClothingCookedPhysX3Param_0p0NS::ParametersStruct*>(tmpThis));
	}

	ClothingCookedPhysX3Param_0p0NS::ParametersStruct& parameters(void)
	{
		return *(static_cast<ClothingCookedPhysX3Param_0p0NS::ParametersStruct*>(this));
	}

	virtual NvParameterized::ErrorType getParameterHandle(const char* long_name, NvParameterized::Handle& handle) const;
	virtual NvParameterized::ErrorType getParameterHandle(const char* long_name, NvParameterized::Handle& handle);

	void initDefaults(void);

protected:

	virtual const NvParameterized::DefinitionImpl* getParameterDefinitionTree(void);
	virtual const NvParameterized::DefinitionImpl* getParameterDefinitionTree(void) const;


	virtual void getVarPtr(const NvParameterized::Handle& handle, void*& ptr, size_t& offset) const;

private:

	void buildTree(void);
	void initDynamicArrays(void);
	void initStrings(void);
	void initReferences(void);
	void freeDynamicArrays(void);
	void freeStrings(void);
	void freeReferences(void);

	static bool mBuiltFlag;
	static NvParameterized::MutexType mBuiltFlagMutex;
};

class ClothingCookedPhysX3Param_0p0Factory : public NvParameterized::Factory
{
	static const char* const vptr;

public:

	virtual void freeParameterDefinitionTable(NvParameterized::Traits* traits)
	{
		ClothingCookedPhysX3Param_0p0::freeParameterDefinitionTable(traits);
	}

	virtual NvParameterized::Interface* create(NvParameterized::Traits* paramTraits)
	{
		// placement new on this class using mParameterizedTraits

		void* newPtr = paramTraits->alloc(sizeof(ClothingCookedPhysX3Param_0p0), ClothingCookedPhysX3Param_0p0::ClassAlignment);
		if (!NvParameterized::IsAligned(newPtr, ClothingCookedPhysX3Param_0p0::ClassAlignment))
		{
			NV_PARAM_TRAITS_WARNING(paramTraits, "Unaligned memory allocation for class ClothingCookedPhysX3Param_0p0");
			paramTraits->free(newPtr);
			return 0;
		}

		memset(newPtr, 0, sizeof(ClothingCookedPhysX3Param_0p0)); // always initialize memory allocated to zero for default values
		return NV_PARAM_PLACEMENT_NEW(newPtr, ClothingCookedPhysX3Param_0p0)(paramTraits);
	}

	virtual NvParameterized::Interface* finish(NvParameterized::Traits* paramTraits, void* bufObj, void* bufStart, int32_t* refCount)
	{
		if (!NvParameterized::IsAligned(bufObj, ClothingCookedPhysX3Param_0p0::ClassAlignment)
		        || !NvParameterized::IsAligned(bufStart, ClothingCookedPhysX3Param_0p0::ClassAlignment))
		{
			NV_PARAM_TRAITS_WARNING(paramTraits, "Unaligned memory allocation for class ClothingCookedPhysX3Param_0p0");
			return 0;
		}

		// Init NvParameters-part
		// We used to call empty constructor of ClothingCookedPhysX3Param_0p0 here
		// but it may call default constructors of members and spoil the data
		NV_PARAM_PLACEMENT_NEW(bufObj, NvParameterized::NvParameters)(paramTraits, bufStart, refCount);

		// Init vtable (everything else is already initialized)
		*(const char**)bufObj = vptr;

		return (ClothingCookedPhysX3Param_0p0*)bufObj;
	}

	virtual const char* getClassName()
	{
		return (ClothingCookedPhysX3Param_0p0::staticClassName());
	}

	virtual uint32_t getVersion()
	{
		return (ClothingCookedPhysX3Param_0p0::staticVersion());
	}

	virtual uint32_t getAlignment()
	{
		return (ClothingCookedPhysX3Param_0p0::ClassAlignment);
	}

	virtual const uint32_t* getChecksum(uint32_t& bits)
	{
		return (ClothingCookedPhysX3Param_0p0::staticChecksum(bits));
	}
};
#endif // NV_PARAMETERIZED_ONLY_LAYOUTS

} // namespace parameterized
} // namespace nvidia

#if PX_VC
#pragma warning(pop)
#endif

#endif
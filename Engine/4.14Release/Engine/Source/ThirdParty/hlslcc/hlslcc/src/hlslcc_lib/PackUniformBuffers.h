// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

// See bottom of the file for a brief explanation of Packing UB methods/options

#ifndef PACKUNIFORMBUFFERS_H
#define PACKUNIFORMBUFFERS_H

//@todo-rco: Remove STL!
// THIRD_PARTY* are macros that aid static analysis of the engine, they won't be defined for hlslcc project
#ifdef __UNREAL__
THIRD_PARTY_INCLUDES_START
#endif // __UNREAL__
	#include <map>
	#include <string>
	#include <set>
#ifdef __UNREAL__
THIRD_PARTY_INCLUDES_END
#endif // __UNREAL__

#include "CustomStdAllocator.h"

typedef std::map<FCustomStdString, ir_variable*> TStringIRVarMap;
typedef std::map<FCustomStdString, TStringIRVarMap> TStringStringIRVarMap;
typedef std::list<ir_variable*> TIRVarList;


struct SUniformVarEntry
{
	ir_variable*	UniformArrayVar;
	int				Vec4Start;
	int				Components;
	int				NumRows;
};

typedef std::map<ir_variable*, SUniformVarEntry> TVarVarMap;

// Remove typeA(typeB(typeA(x)) to typeA(x)
void FixRedundantCasts(exec_list* IR);

// After packing, you might get code like this
//		vec4 t9[4];
//		t9[0] = _pu[4];
//		t9[1] = _pu[5];
//		t9[2] = _pu[6];
//		t9[3] = _pu[7];
//		[...]
//		vec4 z = t9[i] + t9[2];
// Ideally we want to convert it to:
//		vec4 z = _pu[4 + i] + _pu[6];
// This requires the UniformMap generated by PackUniforms()
void RemovePackedUniformBufferReferences( exec_list* Instructions, _mesa_glsl_parse_state* ParseState, TVarVarMap& UniformMap );

// Flattens structures inside a uniform buffer into uniform variables; from:
//		cbuffer CB
//		{
//			float4 Value0;
//			struct
//			{
//				float4 Member0;
//				float3 Member1;
//			} S;
//			float4 Value1;
//		};
//	to:
//		cbuffer CB
//		{
//			float4 Value;
//			float4 S_Member0;
//			float3 S_Member1;
//			float4 Value1;
//		};
void FlattenUniformBufferStructures(exec_list* Instructions, _mesa_glsl_parse_state* ParseState);


/**
 * Pack uniforms in to typed arrays.
 * @param Instructions - The IR for which to pack uniforms.
 * @param ParseState - Parse state.
 * @param bFlattenStructure - True if we want all structs flattened into members (FlattenUniformBufferStructures() was called)
 * @param bGroupFlattenedUBs - 
 * @param OutUniformMap - Mapping table used during backend code gen for cross referencing source/packed uniforms
 */
void PackUniforms(exec_list* Instructions, _mesa_glsl_parse_state* ParseState, bool bFlattenStructure, bool bGroupFlattenedUBs, bool bPackGlobalArraysIntoUniformBuffers, TVarVarMap& OutUniformMap);

// Expand any full assignments (a = b) to per element (a[0] = b[0]; a[1] = b[1]; etc) so the array can be split
bool ExpandArrayAssignments(exec_list* ir, _mesa_glsl_parse_state* State);

// Removes the SamplerStates from hlsl while making a map for the textures it references
bool ExtractSamplerStatesNameInformation(exec_list* Instructions, _mesa_glsl_parse_state* ParseState);

// Expands matrices to arrays to help on some cases (e.g., non-square matrices on ES 2)
bool ExpandMatricesIntoArrays(exec_list* Instructions, _mesa_glsl_parse_state* ParseState);

// Find all variables that are at any point used by an atomic instruction
void FindAtomicVariables(exec_list* ir, TIRVarSet& OutAtomicVariables);

// Find all variables that are at any point used by an atomic instruction
void FixAtomicReferences(exec_list* ir, _mesa_glsl_parse_state* State, TIRVarSet& AtomicVariables);

#endif


/////////////////////////////////////////////////
// Packing/grouping uniform flags explanation
/////////////////////////////////////////////////
#if 0
/////////////////////////////////////////////////
// Sample shader. 3 Globals, 1 regular CBuffer, one CBuffer with a struct member.
/////////////////////////////////////////////////
float Global0;
float Global1;
float Global2;

cbuffer CBuffer
{
	float CBMember0;
	float CBMember1;
}

cbuffer CBufferStruct
{
	struct
	{
		float SMember0;
	} S;
}

float4 Main() : SV_Target0
{
	return Global0 * Global1 + Global2 * CBMember0 + CBMember1 / S.SMember0;
}

/////////////////////////////////////////////////
/// Method 0: No compile flags at all
/////////////////////////////////////////////////
// @Outputs: f4:out_Target0
// @UniformBlocks: CBuffer(0),CBufferStruct(1)
// @Uniforms: f1:Global2,f1:Global1,f1:Global0
#version 150
struct anon_struct_0000
{
	float SMember0;
};

layout(std140) uniform CBuffer
{
	float CBMember0;
	float CBMember1;
};

layout(std140) uniform CBufferStruct
{
	anon_struct_0000 S;
};

uniform float Global2;
uniform float Global1;
uniform float Global0;
out vec4 out_Target0;
void main()
{
	out_Target0.xyzw = vec4((((Global0*Global1)+(Global2*CBMember0))+(CBMember1/S.SMember0)));
}

/////////////////////////////////////////////////
/// Method 1: Pack Uniforms (HLSLCC_PackUniforms).
//		All global variables will be packed into an array based off precision (high/float, medium/half, low/fixed).
/////////////////////////////////////////////////
// @Outputs: f4:out_Target0
// @UniformBlocks: CBuffer(0),CBufferStruct(1)
// @PackedGlobals: Global0(h:0,1),Global1(h:4,1),Global2(h:8,1)
struct anon_struct_0000
{
	float SMember0;
};

layout(std140) uniform pb0
{
	float CBMember0;
	float CBMember1;
};

layout(std140) uniform pb1
{
	anon_struct_0000 S;
};

uniform vec4 pu_h[3];
out vec4 out_Target0;
void main()
{
	out_Target0.xyzw = vec4((((pu_h[0].x*pu_h[1].x)+(pu_h[2].x*CBMember0))+(CBMember1/S.SMember0)));
}

/////////////////////////////////////////////////
/// Method 2: HLSLCC_FlattenUniformBufferStructures (implies HLSLCC_PackUniforms)
//		All constant buffers with members being structures are flattened to be in the packed arrays by precision
/////////////////////////////////////////////////
// @Outputs: f4:out_Target0
// @UniformBlocks: CBuffer(0)
// @PackedGlobals: Global0(h:0,1),Global1(h:4,1),Global2(h:8,1)
#version 150
layout(std140) uniform pb0
{
	float CBMember0;
	float CBMember1;
};

uniform vec4 pu_h[4];
out vec4 out_Target0;
void main()
{
	out_Target0.xyzw = vec4((((pu_h[0].x*pu_h[1].x) + (pu_h[2].x*CBMember0)) + (CBMember1 / pu_h[3].x)));
}

/////////////////////////////////////////////////
/// Method 3: HLSLCC_FlattenUniformBuffers (implies HLSLCC_FlattenUniformBufferStructures)
//		All constant buffers with any kind of members are flattened into the packed arrays by precision
/////////////////////////////////////////////////
// @Outputs: f4:out_Target0
// @PackedGlobals: Global0(h:0,1),Global1(h:4,1),Global2(h:8,1)
// @PackedUB: CBuffer(0): CBMember0(0,1),CBMember1(1,1)
// @PackedUB: CBufferStruct(1): S_SMember0(0,1)
// @PackedUBGlobalCopies: 0:0-h:12:1,0:1-h:16:1,1:0-h:20:1
uniform vec4 pu_h[6];
out vec4 out_Target0;
void main()
{
	out_Target0.xyzw = vec4((((pu_h[0].x*pu_h[1].x) + (pu_h[2].x*pu_h[3].x)) + (pu_h[4].x / pu_h[5].x)));
}

/////////////////////////////////////////////////
/// Method 4: HLSLCC_GroupFlattenedUniformBuffers (implies HLSLCC_FlattenUniformBuffers)
//		Each constant buffer with any kind of members are flattened into global arrays per constant buffer/precision
/////////////////////////////////////////////////
// @Outputs: f4:out_Target0
// @PackedGlobals: Global0(h:0,1),Global1(h:4,1),Global2(h:8,1)
// @PackedUB: CBuffer(0): CBMember0(0,1),CBMember1(1,1)
// @PackedUB: CBufferStruct(1): S_SMember0(0,1)
// @PackedUBCopies: 0:0-0:h:0:1,0:1-0:h:4:1,1:0-1:h:0:1
uniform vec4 pc1_h[1];
uniform vec4 pc0_h[2];
uniform vec4 pu_h[3];
out vec4 out_Target0;
void main()
{
	out_Target0.xyzw = vec4((((pu_h[0].x*pu_h[1].x) + (pu_h[2].x*pc0_h[0].x)) + (pc0_h[1].x / pc1_h[0].x)));
}
#endif

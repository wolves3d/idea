//-----------------------------------------------------------------------------
// File: HLSLwithoutEffects.vsh
//
// Desc: The vertex shader file for the HLSLWithoutFX sample.  It contains a vertex 
//		 shader which animates the vertices.
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
cbuffer cb0
{
	row_major float4x4 mMVP;  // World * View * Projection transformation
	float4 vColorMod;
	float2 vOffset;
};

/*cbuffer cb1
{
	float4 vV;
};*/

//-----------------------------------------------------------------------------
// Vertex shader output structure
//-----------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position : SV_Position;	// vertex position 
    float2 vUV 		: TEXCOORD0;
	float4 vColor	: COLOR;
};


struct VS_INPUT
{
    float2 Position : POSITION;
    float2 vUV		: TEXCOORD;
};


//-----------------------------------------------------------------------------
// Name: Ripple
// Type: Vertex shader                                      
// Desc: This shader ripples the vertices
//-----------------------------------------------------------------------------
VS_OUTPUT Ripple( in VS_INPUT v )
{
    VS_OUTPUT Output;
    
	Output.Position = mul( float4( v.Position.x, v.Position.y, 0.5f, 1.0f ), mMVP );
	//Output.Position = float4( v.Position.x + vOffset.x, v.Position.y + vOffset.y, 0.5f, 1.0f );
	Output.vUV = v.vUV;// + vV.xy;
	Output.vColor = vColorMod;
    
    return Output;
}



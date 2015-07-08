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
    float4 Position	: SV_Position;
    float4 vColor	: TEXCOORD0;
};


struct VS_INPUT
{
    float3 vPos 	: POSITION;
    float4 vColor	: COLOR;
};


//-----------------------------------------------------------------------------
// Name: Ripple
// Type: Vertex shader                                      
// Desc: This shader ripples the vertices
//-----------------------------------------------------------------------------
VS_OUTPUT Ripple( in VS_INPUT vertex )
{
    VS_OUTPUT Output;
    
	Output.vColor = vertex.vColor;
	
	Output.Position = mul( float4( vertex.vPos.x, vertex.vPos.y, vertex.vPos.z, 1.0f ), mMVP );
//	Output.Position = mul( vertex.vPos, mMVP );
    
    return Output;
}




cbuffer MatrixBuffer : register(b0)
{
	matrix ModelToWorldMatrix;
	matrix WorldToViewMatrix;
	matrix ProjectionMatrix;
};




struct VSIn
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
	float2 TexCoord : TEX;
};

struct PSIn
{
	float4 Pos  : SV_Position;
	float3 Normal : NORMAL;
	float2 TexCoord : TEX;

	float3 worldpos : WORLD_POS;
};

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSIn VS_main(VSIn input)
{
	PSIn output = (PSIn)0;
	

	// model-to-view
	matrix MV = mul(ModelToWorldMatrix, WorldToViewMatrix);
	// model-to-projection
	matrix MVP = mul(MV, ProjectionMatrix);
	
	output.Pos = mul(float4(input.Pos, 1), MVP);
	output.Normal = mul(input.Normal, ModelToWorldMatrix);
	output.TexCoord = input.TexCoord;
	output.worldpos = mul(float4(input.Pos, 1), ModelToWorldMatrix);
	
	return output;
}
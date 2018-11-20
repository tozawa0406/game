cbuffer scene : register(b0)
{
	matrix View;
	matrix Proj;
};

cbuffer object : register(b1)
{
	matrix World;
	float4 Texcoord;
	float4 Diffuse;
};

Texture2D    DiffuseTexture : register(t0);   
SamplerState DiffuseSampler : register(s0);

struct IN_VS
{
	float4 position : POSITION0;
	float4 normal   : NORMAL0;
	float4 tangent	: TANGENT0;
	float4 color    : COLOR0;
	float2 texcoord : TEXCOORD0;
	float4 boneIndex: TEXCOORD1;
	float4 weight   : TEXCOORD2;
};

struct OUT_VS
{
	float4 position : SV_POSITION;
	float4 color    : COLOR0;
	float2 texcoord : TEXCOORD0;
};

//バーテックスシェーダー
OUT_VS VS_MeshMain(IN_VS In)
{   
	OUT_VS Out = (OUT_VS)0;

	Out.position = mul(In.position , transpose(World));
	Out.position = mul(Out.position, transpose(View));
	Out.position = mul(Out.position, transpose(Proj));

	Out.color    = Diffuse;
	Out.texcoord = In.texcoord;

    return Out;
}

OUT_VS VS_SpriteMain(IN_VS In)
{
	OUT_VS Out = (OUT_VS)0;

	Out.position = mul(In.position , transpose(World));
	Out.position = mul(Out.position, transpose(View));
	Out.position = mul(Out.position, transpose(Proj));

	Out.color    = Diffuse;
	Out.texcoord = In.texcoord * float2(Texcoord.z, Texcoord.w) + float2(Texcoord.x, Texcoord.y);

	return Out;
}

//ピクセルシェーダー
float4 PS_Main(OUT_VS In) : SV_Target
{
	return In.color * DiffuseTexture.Sample(DiffuseSampler, In.texcoord);
}

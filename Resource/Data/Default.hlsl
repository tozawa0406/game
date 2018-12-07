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
	float4 worldPosition : TEXCOORD1;
	float4 worldNormal   : TEXCOORD2;
};

struct OUT_PS
{
	float4 target0 : SV_Target0;
	float4 target1 : SV_Target1;
	float4 target2 : SV_Target2;
};

//バーテックスシェーダー
OUT_VS VS_MeshMain(IN_VS In)
{   
	OUT_VS Out = (OUT_VS)0;

	Out.position = mul(In.position , transpose(World));
	Out.worldPosition = Out.position;
	Out.position = mul(Out.position, transpose(View));
	Out.position = mul(Out.position, transpose(Proj));

	Out.worldNormal = mul(In.normal, transpose(World));
	Out.color    = Diffuse;
	Out.texcoord = In.texcoord;

    return Out;
}

OUT_VS VS_SpriteMain(IN_VS In)
{
	OUT_VS Out = (OUT_VS)0;

	Out.position = mul(In.position , transpose(World));
	Out.worldPosition = Out.position;
	Out.position = mul(Out.position, transpose(View));
	Out.position = mul(Out.position, transpose(Proj));

	Out.color    = Diffuse;
	Out.texcoord = In.texcoord * float2(Texcoord.z, Texcoord.w) + float2(Texcoord.x, Texcoord.y);
	Out.worldNormal.xyz = mul(In.normal.xyz, (float3x3)transpose(World));
	Out.worldNormal = normalize(Out.worldNormal);
	Out.worldNormal.w = 1;

	return Out;
}

//ピクセルシェーダー
OUT_PS PS_Main(OUT_VS In)
{
	OUT_PS color;
	color.target0 = In.color * DiffuseTexture.Sample(DiffuseSampler, In.texcoord);
	color.target1 = In.worldPosition;
	color.target2 = In.worldNormal;
	return color;
}

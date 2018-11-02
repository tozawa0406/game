cbuffer scene : register(b0)
{
	matrix World;
	matrix View;
	matrix Proj;
	float4 Texcoord;
	float4 Color;
	matrix LightView;               // ライトビュー変換行列
	matrix LightProj;               // 射影変換行列
};

Texture2D    DiffuseTexture  : register(t0);   
SamplerState DiffuseSampler  : register(s0);

Texture2D    ShadowTexture : register(t1);
SamplerState ShadowSampler : register(s1);

struct VERTEX
{
	float3 position;
	float3 normal;
	float4 color;
	float2 texcoord;
	float4 boneIndex;
	float4 weight;
};

StructuredBuffer<VERTEX> Vertex : register(t0);

struct OUT_VS
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float4 ZCalcTex : TEXCOORD1;
	float4 color    : COLOR0;
};

//バーテックスシェーダー
OUT_VS VS_Main(uint id : SV_VertexID)
{   
	VERTEX v = Vertex[id];

	OUT_VS Out = (OUT_VS)0;

	Out.position = float4(v.position, 1);

	Out.position = mul(Out.position, transpose(World));
	Out.position = mul(Out.position, transpose(View));
	Out.position = mul(Out.position, transpose(Proj));

	// ライトの目線によるワールドビュー射影変換をする
	Out.ZCalcTex = v.weight;

	// 法線とライトの方向から頂点の色を決定
	// 濃くなりすぎないように調節しています
	matrix mtx = transpose(World);
	mtx._41_42_43 = 0;
	float3 N = normalize(mul(v.normal, mtx));
	float3 lightDirect = normalize(float3(LightView._31, LightView._32, LightView._33));
	float k = max(dot(N, -lightDirect), 0.0f);
	Out.color = Color * (0.3 + k);
	Out.color.a = 1;

	Out.color = float4(1, 1, 1, 1);

	Out.texcoord = v.texcoord;

    return Out;
}

//ピクセルシェーダー
float4 PS_Main(OUT_VS In) : SV_Target
{
	float4 Out = In.color * DiffuseTexture.Sample(DiffuseSampler, In.texcoord);

	// ライト目線によるZ値の再算出
	float inv = 1.0f / In.ZCalcTex.w;
	float zValue = In.ZCalcTex.z * inv;

	// 射影空間のXY座標をテクスチャ座標に変換
	float3 transTexCoord;
	transTexCoord.x = (1.0f + In.ZCalcTex.x * inv) * 0.5f;
	transTexCoord.y = (1.0f - In.ZCalcTex.y * inv) * 0.5f;

	float4 texColor = ShadowTexture.Sample(ShadowSampler, transTexCoord);
	
	// リアルZ値抽出
	float SM_Z = texColor.r;

	// 算出点がシャドウマップのZ値よりも大きければ影と判断
	if (zValue > SM_Z + 0.0005f)
	{
		Out.rgb *= 0.5f;
	}

	return Out;
}

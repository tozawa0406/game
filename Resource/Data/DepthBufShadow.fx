float4x4 World;		// 変換行列
float4x4 View;
float4x4 Proj;
float4x4 LightView;               // ライトビュー変換行列
float4x4 LightProj;               // 射影変換行列
float4   Diffuse;
float4   texcoord;

// 頂点シェーダ
void VS_DepthBufShadow(
	in  float4 InPos       : POSITION,
	in  float3 InNorm      : NORMAL,
	in  float2 InTexcoord  : TEXCOORD0,

	out float4 OutPos      : POSITION,
	out float2 OutTexcoord : TEXCOORD0,
	out float4 OutZCalcTex : TEXCOORD1,
	out float4 OutColor    : COLOR0
)
{
	// 普通にカメラの目線によるワールドビュー射影変換をする
	OutPos = mul(InPos , World);
	OutPos = mul(OutPos, View);
	OutPos = mul(OutPos, Proj);

	// ライトの目線によるワールドビュー射影変換をする
	float4x4 mtx;
	mtx = mul(World, LightView);
	mtx = mul(mtx  , LightProj);
	OutZCalcTex = mul(InPos, mtx);

	// 法線とライトの方向から頂点の色を決定
	// 濃くなりすぎないように調節しています
	mtx = World;
	mtx._41_42_43 = 0;
	float3 N = normalize(mul(InNorm, mtx));
	float3 lightDirect = normalize(float3(LightView._13, LightView._23, LightView._33));
	float k = max(dot(N, -lightDirect), 0.0f);
	OutColor = Diffuse * (0.3 + k);
	OutColor.a = 1;

	OutTexcoord = InTexcoord;
}

sampler DefSampler = sampler_state	// サンプラーステート
{
	texture = (texShadowMap);
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

sampler TexSampler = sampler_state
{
	Texture = (Tex);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

// ピクセルシェーダ
void PS_DepthBufShadow(
	in  float4 InColor    : COLOR0,
	in  float2 InTexcoord : TEXCOORD0,
	in  float4 InZCalcTex : TEXCOORD1,

	out float4 OutColor   : COLOR0
)
{
	float4 Out = InColor * tex2D(TexSampler, InTexcoord);

	// ライト目線によるZ値の再算出
	float inv = 1.0f / InZCalcTex.w;
	float zValue = InZCalcTex.z * inv;

	// 射影空間のXY座標をテクスチャ座標に変換
	float3 transTexCoord;
	transTexCoord.x = (1.0f + InZCalcTex.x * inv) * 0.5f;
	transTexCoord.y = (1.0f - InZCalcTex.y * inv) * 0.5f;

	float4 texColor = tex2D(DefSampler, transTexCoord);

	// リアルZ値抽出
	float SM_Z = texColor.r + (texColor.g + (texColor.b / 256.0f) / 256.0f) / 256.0f;

	// 算出点がシャドウマップのZ値よりも大きければ影と判断
	if (zValue > SM_Z + 0.0032f)
	{
		Out.rgb *= 0.5f;
	}

	OutColor = Out;
}

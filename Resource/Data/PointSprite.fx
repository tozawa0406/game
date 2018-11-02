float4x4 World;		// 変換行列
float4x4 View;
float4x4 Proj;
float4   Diffuse;

struct VS_OUTPUT
{
	float4 Pos      : POSITION;
	float  Size     : PSIZE;
	float4 Diff     : COLOR0;
};

struct PS_OUTPUT
{
	float4 Diff    : COLOR0;
	float2 Texture : TEXCOORD0;
};

// 頂点シェーダーの関数
void VS_Main(
	in float4 InPos		: POSITION,
	in float  InSize	: PSIZE,
	in float4 InDiff	: COLOR0,

	out float4 OutPos	: POSITION,
	out float  OutSize	: PSIZE,
	out float4 OutDiff	: COLOR0
)
{
	float4x4 inv = View;
	inv = transpose(inv);
	inv._14_24_34 = 0;

	OutPos = mul(InPos, inv);
	OutPos = mul(OutPos, World);   // ワールド変換
	OutPos = mul(OutPos, View);    // ビュー変換
	OutPos = mul(OutPos, Proj);    // 射影変換

	OutDiff = Diffuse;

	float d = length(OutPos.xyz);
	OutSize = (InSize * (Proj._22 / Proj._11) / d) * 400;
}

texture Tex;		// ピクセルシェーダーで使うテクスチャ
// テクスチャサンプラ
sampler TexSampler = sampler_state
{
	Texture = <Tex>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

// ピクセルシェーダーの関数(テクスチャあり)
float4 PS_Main(float4 Diff : COLOR0, float2 Texture : TEXCOORD0) : COLOR0
{
	float4 outDiff = Diff * tex2D(TexSampler, Texture);

	return outDiff;
}

// テクニックの設定
technique Tech		// テクニック名「TexTech」
{
	pass Pass0			// 1番目のパス名「Pass0」
	{
		//ステート設定
		PointSpriteEnable = TRUE;

		BlendOp   = ADD;
		SrcBlend  = SRCALPHA;
		DestBlend = ONE;

		ZWriteEnable = FALSE;
		ZEnable      = TRUE;
		Lighting     = FALSE;

		AlphaTestEnable = TRUE;
		AlphaRef        = 100;
		AlphaFunc       = GREATER;

		VertexShader = compile vs_2_0 VS_Main();	// 頂点シェーダーの設定
		PixelShader  = compile ps_2_0 PS_Main();	// ピクセルシェーダーの設定
	}
}

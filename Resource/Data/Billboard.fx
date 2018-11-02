float4x4 World;		// 変換行列
float4x4 View;
float4x4 Proj;
float4   EyePos;
float4   LightPos;			// 光源の位置(モデル座標)
float4   Diffuse;			// ディフーズ色
float4   texcoord;

struct VS_INPUT
{
	float4 Pos     : POSITION;
	float4 Normal  : NORMAL0;
	float4 Diff    : COLOR0;
	float2 Texture : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Pos     : POSITION;
	float4 Diff    : COLOR0;
	float2 Texture : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 Diff    : COLOR0;
	float2 Texture : TEXCOORD0;
};

// 頂点シェーダーの関数
VS_OUTPUT VS_Main(VS_INPUT In)
{
	VS_OUTPUT Out;

	// 頂点をビュー空間に変換
	float4x4 inv = View;
	inv = transpose(inv);
	inv._14_24_34 = 0;

	Out.Pos = mul(In.Pos , inv);
	Out.Pos = mul(Out.Pos, World);   // ワールド変換
	Out.Pos = mul(Out.Pos, View);    // ビュー変換
	Out.Pos = mul(Out.Pos, Proj);    // 射影変換

	// ディフューズ色を計算
//	float4 L = normalize(LightPos - In.Pos);
//	Out.Diff = Diffuse * max(0.4, dot(In.Normal, L));

	Out.Diff = Diffuse;

	// テクスチャ座標を作る
	Out.Texture = In.Texture * float2(texcoord.z, texcoord.w) + float2(texcoord.x, texcoord.y);

	return Out;
}


texture Tex;		// ピクセルシェーダーで使うテクスチャ
// テクスチャサンプラ
sampler TexSampler = sampler_state
{
	Texture = (Tex);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

// ピクセルシェーダーの関数(テクスチャあり)
float4 PS_Main(PS_OUTPUT In) : COLOR0
{
	float4 outDiff = In.Diff * tex2D(TexSampler, In.Texture);

	return outDiff;
}

// テクニックの設定
technique Tech		// テクニック名「TexTech」
{
	pass Pass0			// 1番目のパス名「Pass0」
	{
		//ステート設定
		ShadeMode = GOURAUD;
		ZEnable   = TRUE;
		CullMode  = CCW;
		Lighting  = FALSE;
		AlphaBlendEnable = TRUE;
		AlphaTestEnable = TRUE;
		AlphaRef = 100;
		AlphaFunc = GREATER;

		VertexShader = compile vs_2_0 VS_Main();	// 頂点シェーダーの設定
		PixelShader  = compile ps_2_0 PS_Main();	// ピクセルシェーダーの設定
	}
}

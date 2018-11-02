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

struct VS_PHONG_OUTPUT
{
	float4 Pos     : POSITION;
	float4 Diff    : COLOR0;
	float2 Texture : TEXCOORD0;
	float3 N       : TEXCOORD1;   //オブジェクトの法線ベクトル
	float3 L       : TEXCOORD2;   //頂点 -> ライト位置 ベクトル
	float3 E       : TEXCOORD3;   //頂点 -> 視点 ベクトル
	float  Power   : TEXCOORD4;   //後光の強度
};

// 頂点シェーダーの関数
VS_PHONG_OUTPUT VS_Main(VS_INPUT In)
{
	VS_PHONG_OUTPUT Out;

	// ワールド変換
	Out.Pos = mul(In.Pos , World);
	float4 phongPos = Out.Pos;
	// ビュー変換
	Out.Pos = mul(Out.Pos, View);
	// 射影変換
	Out.Pos = mul(Out.Pos, Proj);

	// テクスチャ座標はそのまま
	Out.Texture = In.Texture;


	// フォンシェーダ
	// 頂点からライトへのべクトル
	Out.L = normalize(LightPos.xyz - phongPos.xyz);
	// 頂点から視点へのベクトル
	Out.E = normalize(EyePos.xyz - phongPos.xyz);

	// モデルの回転(スケール)行列
	float4x4 rot = World;
	rot._41_42_43 = 0;			// 移動成分を消す

	float4 n = mul(In.Normal, rot);   // 法線の回転

	Out.N = normalize(n.xyz);		  // 法線ベクトル

	//ライトベクトルと法線ベクトルの内積を計算し、計算結果の色の最低値を環境光( m_Ambient )に制限する
	Out.Diff = max(0.3, dot(Out.N, Out.L));


	// リムライティング
	//ライトがカメラの正面近くにあるほど強度が強くなるようにする
	Out.Power = max(0.0, dot(Out.E, -Out.L));

	return Out;
}

// 頂点シェーダーの関数
VS_OUTPUT VS_Tex_Main(VS_INPUT In)
{
	VS_OUTPUT Out;

	// 頂点をビュー空間に変換
	Out.Pos = mul(In.Pos , World);   // ワールド変換
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
float4 PS_Tex_Main(PS_OUTPUT In) : COLOR0
{
	float4 outDiff = In.Diff * tex2D(TexSampler, In.Texture);

	return outDiff;
}

// ピクセルシェーダーの関数(テクスチャあり)
float4 PS_Phong_Main(VS_PHONG_OUTPUT In) : COLOR0
{
	float4 Out;
	
	// フォンシェーディング
	// ライトへのベクトルと視点へのベクトルの合成
	float3 H = normalize(In.L + In.E);
	//スペキュラーカラーを計算             範囲    強度
	float S = pow(max(0.0f, dot(In.N, H)), 10.0) * 0.75;



	// リムライティング
	//オブジェクトの輪郭付近の強度が強くなるようにする
	float P = 1.0f - max(0.0f, dot(In.N, In.E));

	//後光を適応する範囲および強度を調整する
	//このあたりの計算はオブジェクトのマテリアルにより任意に変更すること
	P = P * 0.6;
	P = P * P;


	// ハーフランバートシェーディング
	float halfLambert = max(0.2, dot(In.N, In.L));
	halfLambert = halfLambert * 0.5f + 0.5;
	halfLambert = halfLambert * halfLambert;

	//スペキュラーカラーを加算する
	Out = halfLambert * tex2D(TexSampler, In.Texture) + S + (In.Power * P);

	return Out;
}

// ピクセルシェーダー(テクスチャなし)
float4 PS_Main(float4 inDiff  : COLOR0) : COLOR0
{
	return inDiff;
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
		VertexShader = compile vs_2_0 VS_Tex_Main();	// 頂点シェーダーの設定
		PixelShader  = compile ps_2_0 PS_Tex_Main();	// ピクセルシェーダーの設定
	}
}

// テクニックの設定
technique ModelTech		// テクニック名「TexTech」
{
	pass Pass0			// 1番目のパス名「Pass0」
	{
		//ステート設定
		ShadeMode = GOURAUD;
		ZEnable   = TRUE;
		CullMode  = NONE;
		Lighting  = FALSE;
		AlphaBlendEnable = FALSE;
		VertexShader = compile vs_2_0 VS_Main();	// 頂点シェーダーの設定
		PixelShader  = compile ps_2_0 PS_Phong_Main();	// ピクセルシェーダーの設定
	}
}

technique NotTexTech		// テクニック名
{
	pass Pass0			// 1番目のパス名「Pass0」
	{
		//ステート設定
		ShadeMode = GOURAUD;
		ZEnable = TRUE;
		CullMode = NONE;
		Lighting = TRUE;
		AlphaBlendEnable = FALSE;
		VertexShader = compile vs_2_0 VS_Main();	// 頂点シェーダーの設定
		PixelShader  = compile ps_2_0 PS_Main();	// ピクセルシェーダーの設定
	}
}
float4x4 World;		// 変換行列
float4x4 View;
float4x4 Proj;
float4   EyePos;
float4   LightPos;			// 光源の位置(モデル座標)
float4x4 LightView;               // ライトビュー変換行列
float4x4 LightProj;               // 射影変換行列
float4   Diffuse;			// ディフーズ色
float4   texcoord;

// 頂点シェーダーの関数
void VS_Main(
	in float4 InPos     : POSITION,
	in float4 InNormal  : NORMAL0,
	in float4 InDiff    : COLOR0,
	in float2 InTexture : TEXCOORD0,

	out float4 OutPos     : POSITION,
	out float4 OutDiff    : COLOR0,
	out float2 OutTexture : TEXCOORD0,
	out float3 OutN       : TEXCOORD1,   //オブジェクトの法線ベクトル
	out float3 OutL       : TEXCOORD2,   //頂点 -> ライト位置 ベクトル
	out float3 OutE       : TEXCOORD3,   //頂点 -> 視点 ベクトル
	out float  OutPower   : TEXCOORD4,   //後光の強度
	out float4 OutZCalc   : TEXCOORD5 // Z値算出用テクスチャ
)
{
	OutPos = mul(InPos , World);
	float4 phongPos = OutPos;
	OutPos = mul(OutPos, View);
	OutPos = mul(OutPos, Proj);

	// テクスチャ座標はそのまま
	OutTexture = InTexture;

	// フォンシェーダ
	// 頂点からライトへのべクトル
	OutL = normalize(LightPos.xyz - phongPos.xyz);
	// 頂点から視点へのベクトル
	OutE = normalize(EyePos.xyz - phongPos.xyz);

	// モデルの回転(スケール)行列
	float4x4 rot = World;
	rot._41_42_43 = 0;			// 移動成分を消す

	float4 n = mul(InNormal, rot);   // 法線の回転

	OutN = normalize(n.xyz);		  // 法線ベクトル

	//ライトベクトルと法線ベクトルの内積を計算し、計算結果の色の最低値を環境光( m_Ambient )に制限する
	OutDiff = max(0.3, dot(OutN, OutL));


	// リムライティング
	//ライトがカメラの正面近くにあるほど強度が強くなるようにする
	OutPower = max(0.0, dot(OutE, -OutL));


	// 影
	// ライトの目線によるワールドビュー射影変換をする
	float4x4 mtx;
	mtx = mul(World, LightView);
	mtx = mul(mtx  , LightProj);
	OutZCalc = mul(InPos, mtx);

	// 法線とライトの方向から頂点の色を決定
	// 濃くなりすぎないように調節しています
	mtx = World;
	mtx._41_42_43 = 0;
	float3 N = normalize(mul((float3)InNormal, mtx));
	float3 lightDirect = normalize(float3(LightView._13, LightView._23, LightView._33));
	float k = max(dot(N, -lightDirect), 0.0f);
	OutDiff = Diffuse * (0.7 + k);
	OutDiff.a = 1;
}

sampler DefSampler = sampler_state
{
	texture = (texShadowMap);
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

// テクスチャサンプラ
sampler TexSampler = sampler_state
{
	Texture = (Tex);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

// ピクセルシェーダーの関数(テクスチャあり)
void PS_Main(
	in float4 InPos     : POSITION,
	in float4 InDiff    : COLOR0,
	in float2 InTexture : TEXCOORD0,
	in float3 InN       : TEXCOORD1,	//オブジェクトの法線ベクトル
	in float3 InL       : TEXCOORD2,	//頂点 -> ライト位置 ベクトル
	in float3 InE       : TEXCOORD3,	//頂点 -> 視点 ベクトル
	in float  InPower   : TEXCOORD4,	//後光の強度
	in float4 InZCalc   : TEXCOORD5,	// Z値算出用テクスチャ

	out float4 Out : COLOR0
)
{
	Out = InDiff * tex2D(TexSampler, InTexture);
	
	// フォンシェーディング
	// ライトへのベクトルと視点へのベクトルの合成
	float3 H = normalize(InL + InE);
	//スペキュラーカラーを計算             範囲    強度
	float S = pow(max(0.0f, dot(InN, H)), 10.0) * 0.75;

	// リムライティング
	//オブジェクトの輪郭付近の強度が強くなるようにする
	float P = 1.0f - max(0.0f, dot(InN, InE));

	//後光を適応する範囲および強度を調整する
	//このあたりの計算はオブジェクトのマテリアルにより任意に変更すること
	P = P * 0.8;
	P = P * P;

	// ハーフランバートシェーディング
	float halfLambert = max(0.2, dot(InN, InL));
	halfLambert = halfLambert * 0.5f + 0.5;
	halfLambert = halfLambert * halfLambert;

	//スペキュラーカラーを加算する
	Out = halfLambert * (InDiff * tex2D(TexSampler, InTexture)) + S + (InPower * P);

	// ライト目線によるZ値の再算出
	float inv = 1.0f / InZCalc.w;
	float zValue = InZCalc.z * inv;

	// 射影空間のXY座標をテクスチャ座標に変換
	float3 transTexCoord;
	transTexCoord.x = (1.0f + InZCalc.x * inv) * 0.5f;
	transTexCoord.y = (1.0f - InZCalc.y * inv) * 0.5f;

	float4 texColor = tex2D(DefSampler, transTexCoord);

	// リアルZ値抽出
	float SM_Z = texColor.r + (texColor.g + (texColor.b / 256.0f) / 256.0f) / 256.0f;

	// 算出点がシャドウマップのZ値よりも大きければ影と判断
	if (zValue > SM_Z + 0.0019f)
	{
		Out.rgb *= 0.5f;
	}
	Out.a = 1;
}

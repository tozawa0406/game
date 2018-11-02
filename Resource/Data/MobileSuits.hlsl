cbuffer scene : register(b0)
{
	matrix World;
	matrix View;
	matrix Proj;
	float4 Diffuse;
	float4 EyePos;
	float4 LightPos;			// 光源の位置(モデル座標)
	matrix LightView;               // ライトビュー変換行列
	matrix LightProj;               // 射影変換行列
};

Texture2D    DiffuseTexture : register(t0);   
SamplerState DiffuseSampler : register(s0);

Texture2D    ShadowTexture : register(t1);
SamplerState ShadowSampler : register(s1);

struct IN_VS
{
	float4 position : POSITION;
	float4 normal   : NORMAL;
	float4 color    : COLOR;
	float2 texcoord : TEXCOORD;
};

struct OUT_VS
{
	float4 position : SV_POSITION;
	float4 color    : COLOR;
	float2 texcoord : TEXCOORD0;
	float3 N        : TEXCOORD1;   //オブジェクトの法線ベクトル
	float3 L        : TEXCOORD2;   //頂点 -> ライト位置 ベクトル
	float3 E        : TEXCOORD3;   //頂点 -> 視点 ベクトル
	float  power    : TEXCOORD4;   //後光の強度
	float4 ZCalcTex : TEXCOORD5;
};

//バーテックスシェーダー
OUT_VS VS_MeshMain(IN_VS In)
{   
	OUT_VS Out = (OUT_VS)0;

	Out.position = mul(In.position , transpose(World));
	float4 phongPos = Out.position;
	Out.position = mul(Out.position, transpose(View));
	Out.position = mul(Out.position, transpose(Proj));

	// フォンシェーダ
	// 頂点からライトへのべクトル
	Out.L = normalize(LightPos.xyz - phongPos.xyz);
	// 頂点から視点へのベクトル
	Out.E = normalize(EyePos.xyz - phongPos.xyz);

	// モデルの回転(スケール)行列
	float4x4 rot = transpose(World);
	rot._41_42_43 = 0;			// 移動成分を消す

	float4 n = mul(In.normal, rot);   // 法線の回転

	Out.N = normalize(n.xyz);		  // 法線ベクトル

	//ライトベクトルと法線ベクトルの内積を計算し、計算結果の色の最低値を環境光( m_Ambient )に制限する
	Out.color = max(0.3, dot(Out.N, Out.L));

	// リムライティング
	//ライトがカメラの正面近くにあるほど強度が強くなるようにする
	Out.power = max(0.0, dot(Out.E, -Out.L));

	//// ライトの目線によるワールドビュー射影変換をする
	float4 calc;
	calc = mul(In.position, transpose(World));
	calc = mul(calc, transpose(LightView));
	calc = mul(calc, transpose(LightProj));
	Out.ZCalcTex = calc;

	// 法線とライトの方向から頂点の色を決定
	// 濃くなりすぎないように調節しています
	float3 N = normalize(mul((float3)In.normal, rot));
	float3 lightDirect = normalize(LightPos.xyz);
	float k = max(dot(Out.N, -lightDirect), 0.0f);
	Out.color = Diffuse * (0.8 + k);
	Out.color.a = 1;

	Out.texcoord = In.texcoord;

    return Out;
}

//ピクセルシェーダー
float4 PS_Main(OUT_VS In) : SV_Target
{
	// フォンシェーディング
	// ライトへのベクトルと視点へのベクトルの合成
	float3 H = normalize(In.L + In.E);
	//スペキュラーカラーを計算             範囲    強度
	float S = pow(max(0.0f, dot(In.N, H)), 10.0) * 0.75;

	// リムライティング
	// オブジェクトの輪郭付近の強度が強くなるようにする
	float P = 1.0f - max(0.0f, dot(In.N, In.E));

	// 後光を適応する範囲および強度を調整する
	// このあたりの計算はオブジェクトのマテリアルにより任意に変更すること
	P = P * 0.8;
	P = P * P;

	// ハーフランバートシェーディング
	float halfLambert = max(0.2, dot(In.N, In.L));
	halfLambert = halfLambert * 0.5f + 0.5;
	halfLambert = halfLambert * halfLambert;

	// スペキュラーカラーを加算する
	float4 color = halfLambert * (In.color * DiffuseTexture.Sample(DiffuseSampler, In.texcoord)) + S + (In.power * P);

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
		color.rgb *= 0.5f;
	}
	color.a = 1;

	return color;
}

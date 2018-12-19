#include "VSParam.hlsl"
#include "ShadowFunc.hlsl"

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

cbuffer CBMatrix : register(b2)
{
	float4   TexelSize;
	float4   cameraPos;
	float4   lightDir;
	float4   splitPos;
	float4   splitPosXMax;
	float4   splitPosXMin;
	float4x4 Shadow0;
	float4x4 Shadow1;
	float4x4 Shadow2;
	float4x4 Shadow3;
};

Texture2D    DiffuseTexture  : register(t0);   
SamplerState DiffuseSampler  : register(s0);

Texture2D    ShadowTexture : register(t1);
SamplerState ShadowSampler : register(s1);

Texture2D       ShadowMap0    : register(t3);     //!< シャドウマップ(カスケード0)です.
Texture2D       ShadowMap1    : register(t4);     //!< シャドウマップ(カスケード1)です.
Texture2D       ShadowMap2    : register(t5);     //!< シャドウマップ(カスケード2)です.
Texture2D       ShadowMap3    : register(t6);     //!< シャドウマップ(カスケード3)です.

SamplerComparisonState  ShadowSmp     : register(s3);     //!< シャドウマップ用サンプラー比較ステートです.

struct OUT_PS
{
	float4 target0 : SV_Target0;
	float4 target1 : SV_Target1;
	float4 target2 : SV_Target2;
};

//バーテックスシェーダー
OUT_VS VS_DepthBufShadow(IN_VS In)
{   
	OUT_VS Out = (OUT_VS)0;

	Out.position = mul(In.position , transpose(World));
	Out.worldPosition = Out.position;
	Out.position = mul(Out.position, transpose(View));
	Out.position = mul(Out.position, transpose(Proj));

	//// ライトの目線によるワールドビュー射影変換をする
	float4 calc;
	calc = mul(In.position, transpose(World));
	calc = mul(calc       , transpose(LightView));
	calc = mul(calc       , transpose(LightProj));
//	Out.ZCalcTex = calc;

	// 法線とライトの方向から頂点の色を決定
	// 濃くなりすぎないように調節しています
	matrix mtx = transpose(World);
	mtx._41_42_43 = 0;
	float3 N = normalize(mul(In.normal, mtx).xyz);
	float3 lightDirect = normalize(float3(LightView._31, LightView._32, LightView._33));
	float k = max(dot(N, -lightDirect), 0.0f);
	Out.color = Color * (0.3 + k);
	Out.color.a = 1;

	Out.color = Color;

	Out.texcoord = In.texcoord;
	Out.worldNormal.xyz = mul(In.normal.xyz, (float3x3)transpose(World));
	Out.worldNormal = normalize(Out.worldNormal);
	Out.worldNormal.w = 1;

	Out.lightDir = -lightDir.xyz;
	Out.cameraPos = cameraPos.xyz;

	// カスケードシャドウマップ用.
	Out.splitPos = splitPos;
	Out.splitPosXMax = splitPosXMax;
	Out.splitPosXMin = splitPosXMin;
	Out.sdwcoord[0] = mul(Out.worldPosition, transpose(Shadow0));
	Out.sdwcoord[1] = mul(Out.worldPosition, transpose(Shadow1));
	Out.sdwcoord[2] = mul(Out.worldPosition, transpose(Shadow2));
	Out.sdwcoord[3] = mul(Out.worldPosition, transpose(Shadow3));

	Out.texelSize = TexelSize;

    return Out;
}

//ピクセルシェーダー
OUT_PS PS_DepthBufShadow(OUT_VS In)
{
	float4 Out = In.color * DiffuseTexture.Sample(DiffuseSampler, In.texcoord);

	float3 sdwColor = 1;
	float  shadow = 1;

	float dist = In.position.w;  // ビュー空間でのZ座標
	float x = In.worldPosition.x;

	if (dist < In.splitPos.x && (In.splitPosXMin[0] <= x && x <= In.splitPosXMax[0]))
	{
		//index = 0;
		sdwColor.gb = 1 - In.texelSize.z;
		shadow = CreateShadow(In.sdwcoord[0], In.texelSize, 1, ShadowMap0, ShadowSmp);
	}
	else if (dist < In.splitPos.y && (In.splitPosXMin[1] <= x && x <= In.splitPosXMax[1]))
	{
		//index = 1;
		sdwColor.rb = 1 - In.texelSize.z;
		shadow = CreateShadow(In.sdwcoord[1], In.texelSize, 2, ShadowMap1, ShadowSmp);
	}
	else if (dist < In.splitPos.z && (In.splitPosXMin[2] <= x && x <= In.splitPosXMax[2]))
	{
		//index = 2;
		sdwColor.rg = 1 - In.texelSize.z;
		shadow = CreateShadow(In.sdwcoord[2], In.texelSize, 4, ShadowMap2, ShadowSmp);
	}
	else
	{
		//index = 3;
		sdwColor.g = 1 - In.texelSize.z;
		shadow = CreateShadow(In.sdwcoord[3], In.texelSize, 8, ShadowMap3, ShadowSmp);
	}

	Out.rgb *= sdwColor;
	Out.rgb *= shadow;

	OUT_PS outPS;
	outPS.target0 = Out;
	outPS.target1 = In.worldPosition;
	outPS.target2 = In.worldNormal;
	return outPS;
}

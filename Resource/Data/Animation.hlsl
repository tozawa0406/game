#include "VSParam.hlsl"
#include "ShadowFunc.hlsl"

cbuffer scene : register(b0)
{
	matrix World;
	matrix View;
	matrix Proj;
	float4 Texcoord;
	float4 Color;
	float4 LightPosition;
	matrix LightView;               // ライトビュー変換行列
	matrix LightProj;               // 射影変換行列
};

cbuffer bone : register(b1)
{
	matrix BoneInv[512];
	matrix BoneAnim[512];
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

Texture2D    NormalTexture  : register(t2);
SamplerState NormalSampler  : register(s2);

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
OUT_VS VS_Main(IN_VS In)
{   
	OUT_VS Out = (OUT_VS)0;

	Out.position = (mul(mul(In.position, transpose(BoneInv[In.boneIndex.x])), transpose(BoneAnim[In.boneIndex.x])) * In.weight.x) +
					(mul(mul(In.position, transpose(BoneInv[In.boneIndex.y])), transpose(BoneAnim[In.boneIndex.y])) * In.weight.y) +
					(mul(mul(In.position, transpose(BoneInv[In.boneIndex.z])), transpose(BoneAnim[In.boneIndex.z])) * In.weight.z) +
					(mul(mul(In.position, transpose(BoneInv[In.boneIndex.w])), transpose(BoneAnim[In.boneIndex.w])) * In.weight.w);

	Out.position += In.position * step(In.weight.x, 0.0001f);
	float4 position = Out.position;

	// 座標系の計算
	Out.position = mul(World, Out.position);
	Out.worldPosition = Out.position;
	Out.position = mul(Out.position, transpose(View));
	Out.position = mul(Out.position, transpose(Proj));

	float3 N = normalize(In.normal.xyz);
	float3 T = normalize(In.tangent.xyz);
	float3 B = normalize(cross(N, T));

	//視線ベクトルを計算
	float3 eye = normalize(LightPosition.xyz - position.xyz);

	//視線ベクトルを頂点座標系に変換する
	Out.eye.x = dot(eye, T);
	Out.eye.y = dot(eye, B);
	Out.eye.z = dot(eye, N);
	Out.eye = normalize(Out.eye);

	//頂点座標 -> ライトの位置ベクトル
	float3 light = normalize(position.xyz - LightPosition.xyz);

	//ライトベクトルを頂点座標系に変換する
	Out.light.x = dot(light, T);
	Out.light.y = dot(light, B);
	Out.light.z = dot(light, N);
	Out.light = normalize(Out.light);

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
	Out.sdwcoord[0] = mul(Shadow0, Out.worldPosition);
	Out.sdwcoord[1] = mul(Shadow1, Out.worldPosition);
	Out.sdwcoord[2] = mul(Shadow2, Out.worldPosition);
	Out.sdwcoord[3] = mul(Shadow3, Out.worldPosition);

	Out.texelSize = TexelSize;

	return Out;
}

//ピクセルシェーダー
OUT_PS PS_Main(OUT_VS In)
{
	//法線マップを参照し、法線を取得する
	//法線マップは 0.0f ～ 1.0f の範囲に保存してあるので -1.0f ～ 1.0f の範囲に変換する
	float3 normal = 2.0f * NormalTexture.Sample(NormalSampler, In.texcoord).xyz - 1.0f;

	//フォンシェーディングによるスペキュラーの色を計算する
	//ハーフベクトルの計算
	float3 H = normalize(In.light + In.eye);

	//スペキュラーカラーを計算する
	//	float S = pow(max(0.0f, dot(Normal, H)), m_Specular) * m_SpecularPower;
	float S = 0;

	//合成する
	float4 Out = In.color * DiffuseTexture.Sample(DiffuseSampler, In.texcoord);
	clip(Out.a - 0.5f);
	Out.xyz *= max(0.6f, dot(normal.xyz, In.light.xyz)) + S;

	float shadow = 0;
	float3 sdwColor = 1;

	float dist = In.position.w;  // ビュー空間でのZ座標
	
	if (dist < In.splitPos.x && (In.splitPosXMin[0] <= In.worldPosition.x && In.worldPosition.x <= In.splitPosXMax[0]))
	{
		//index = 0;
		sdwColor.gb = 1 - In.texelSize.z;
		shadow = CreateShadow(In.sdwcoord[0], In.texelSize, 1, ShadowMap0, ShadowSmp);
	}
	else if (dist < In.splitPos.y && (In.splitPosXMin[1] <= In.worldPosition.x && In.worldPosition.x <= In.splitPosXMax[1]))
	{
		//index = 1;
		sdwColor.rb = 1 - In.texelSize.z;
		shadow = CreateShadow(In.sdwcoord[1], In.texelSize, 2, ShadowMap1, ShadowSmp);
	}
	else if (dist < In.splitPos.z && (In.splitPosXMin[2] <= In.worldPosition.x && In.worldPosition.x <= In.splitPosXMax[2]))
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

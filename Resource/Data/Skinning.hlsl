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

cbuffer bone : register(b1)
{
	matrix BoneInv[512];
	matrix BoneAnim[512];
};

struct VERTEX
{
	float3 position;
	float3 normal;
	float4 color;
	float2 texcoord;
	float4 boneIndex;
	float4 weight;
};

StructuredBuffer<VERTEX>	Vertex		: register(t0);
RWStructuredBuffer<VERTEX>	OutVertex	: register(u0);

[numthreads(1, 1, 1)]
void CS_Main(uint3 id : SV_DispatchThreadID)
{   
	VERTEX v = Vertex[id.x];

	float4 Inpos = float4(v.position, 1);
	float4 pos;

	pos = (mul(mul(Inpos, transpose(BoneInv[v.boneIndex.x])), transpose(BoneAnim[v.boneIndex.x])) * v.weight.x) +
		  (mul(mul(Inpos, transpose(BoneInv[v.boneIndex.y])), transpose(BoneAnim[v.boneIndex.y])) * v.weight.y) +
		  (mul(mul(Inpos, transpose(BoneInv[v.boneIndex.z])), transpose(BoneAnim[v.boneIndex.z])) * v.weight.z) +
		  (mul(mul(Inpos, transpose(BoneInv[v.boneIndex.w])), transpose(BoneAnim[v.boneIndex.w])) * v.weight.w);

	pos += Inpos * step(v.weight.x, 0.0001f);

	v.position = pos.xyz;

	// ZCalc
	// ライトの目線によるワールドビュー射影変換をする
	v.weight = mul(Inpos   , transpose(World));
	v.weight = mul(v.weight, transpose(LightView));
	v.weight = mul(v.weight, transpose(LightProj));

	// 法線とライトの方向から頂点の色を決定
	// 濃くなりすぎないように調節しています
	matrix mtx = transpose(World);
	mtx._41_42_43 = 0;
	float3 N = normalize(mul(float4(v.normal, 1), mtx)).xyz;
	float3 lightDirect = normalize(float3(LightView._31, LightView._32, LightView._33));
	float k = max(dot(N, -lightDirect), 0.0f);

	v.color = Color;

    OutVertex[id.x] = v;
}

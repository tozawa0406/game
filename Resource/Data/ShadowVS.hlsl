struct VSInput
{
	float4 Position : POSITION0;
	float4 Normal   : NORMAL0;
	float4 Tangent	: TANGENT0;
	float4 color    : COLOR0;
	float2 TexCoord : TEXCOORD0;
	float4 boneIndex: TEXCOORD1;
	float4 weight   : TEXCOORD2;
};

struct VSOutput
{
    float4  Position : SV_POSITION;
};

cbuffer CBMatrix : register(b0)
{
	float4x4    World;
	float4x4    ViewProj;
};

cbuffer bone : register(b1)
{
	matrix BoneInv[512];
	matrix BoneAnim[512];
};

VSOutput VSFunc( VSInput input )
{
    VSOutput output = (VSOutput)0;

    float4 localPos    = input.Position;

	localPos = (mul(mul(input.Position, transpose(BoneInv[input.boneIndex.x])), transpose(BoneAnim[input.boneIndex.x])) * input.weight.x) +
			   (mul(mul(input.Position, transpose(BoneInv[input.boneIndex.y])), transpose(BoneAnim[input.boneIndex.y])) * input.weight.y) +
			   (mul(mul(input.Position, transpose(BoneInv[input.boneIndex.z])), transpose(BoneAnim[input.boneIndex.z])) * input.weight.z) +
			   (mul(mul(input.Position, transpose(BoneInv[input.boneIndex.w])), transpose(BoneAnim[input.boneIndex.w])) * input.weight.w);

	localPos += input.Position * step(input.weight.x, 0.0001f);

    float4 worldPos    = mul( World,    localPos );
    float4 viewProjPos = mul( ViewProj, worldPos );

    output.Position = viewProjPos;

	return output;
}

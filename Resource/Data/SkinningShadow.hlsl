cbuffer scene : register(b0)
{
	matrix World;
	matrix View;
	matrix Proj;
};

cbuffer bone : register(b1)
{
	matrix BoneInv[512];
	matrix BoneAnim[512];
};

Texture2D    Texture  : register(t0);   
SamplerState Sampler  : register(s0);

struct IN_VS
{
	float4 position : POSITION;
	float4 normal   : NORMAL;
	float4 color    : COLOR0;
	float2 texcoord : TEXCOORD0;
	float4 boneIndex: TEXCOORD1;
	float4 weight   : TEXCOORD2;
};


struct OUT_VS
{
	float4 position : SV_POSITION;
	float4 ZTex     : TEXCOORD0;
};

//�o�[�e�b�N�X�V�F�[�_�[
OUT_VS VS_Main(IN_VS In)
{   
	OUT_VS Out = (OUT_VS)0;

	Out.position = (mul(mul(In.position, transpose(BoneInv[In.boneIndex.x])), transpose(BoneAnim[In.boneIndex.x])) * In.weight.x) +
				   (mul(mul(In.position, transpose(BoneInv[In.boneIndex.y])), transpose(BoneAnim[In.boneIndex.y])) * In.weight.y) +
				   (mul(mul(In.position, transpose(BoneInv[In.boneIndex.z])), transpose(BoneAnim[In.boneIndex.z])) * In.weight.z) +
				   (mul(mul(In.position, transpose(BoneInv[In.boneIndex.w])), transpose(BoneAnim[In.boneIndex.w])) * In.weight.w);

	Out.position += In.position * step(In.weight.x, 0.0001f);

	Out.position = mul(Out.position, transpose(World));
	Out.position = mul(Out.position, transpose(View));
	Out.position = mul(Out.position, transpose(Proj));

	// ���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
	Out.ZTex = Out.position;

    return Out;
}

//�s�N�Z���V�F�[�_�[
float4 PS_Main(OUT_VS In) : SV_Target
{
	// �[�x�Z�o
	float depth = (In.ZTex.z / In.ZTex.w);

	return float4(depth, 0, 0, 1);
}

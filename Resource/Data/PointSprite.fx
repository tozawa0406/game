float4x4 World;		// �ϊ��s��
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

// ���_�V�F�[�_�[�̊֐�
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
	OutPos = mul(OutPos, World);   // ���[���h�ϊ�
	OutPos = mul(OutPos, View);    // �r���[�ϊ�
	OutPos = mul(OutPos, Proj);    // �ˉe�ϊ�

	OutDiff = Diffuse;

	float d = length(OutPos.xyz);
	OutSize = (InSize * (Proj._22 / Proj._11) / d) * 400;
}

texture Tex;		// �s�N�Z���V�F�[�_�[�Ŏg���e�N�X�`��
// �e�N�X�`���T���v��
sampler TexSampler = sampler_state
{
	Texture = <Tex>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

// �s�N�Z���V�F�[�_�[�̊֐�(�e�N�X�`������)
float4 PS_Main(float4 Diff : COLOR0, float2 Texture : TEXCOORD0) : COLOR0
{
	float4 outDiff = Diff * tex2D(TexSampler, Texture);

	return outDiff;
}

// �e�N�j�b�N�̐ݒ�
technique Tech		// �e�N�j�b�N���uTexTech�v
{
	pass Pass0			// 1�Ԗڂ̃p�X���uPass0�v
	{
		//�X�e�[�g�ݒ�
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

		VertexShader = compile vs_2_0 VS_Main();	// ���_�V�F�[�_�[�̐ݒ�
		PixelShader  = compile ps_2_0 PS_Main();	// �s�N�Z���V�F�[�_�[�̐ݒ�
	}
}

float4x4 World;		// �ϊ��s��
float4x4 View;
float4x4 Proj;
float4   EyePos;
float4   LightPos;			// �����̈ʒu(���f�����W)
float4   Diffuse;			// �f�B�t�[�Y�F
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

// ���_�V�F�[�_�[�̊֐�
VS_OUTPUT VS_Main(VS_INPUT In)
{
	VS_OUTPUT Out;

	// ���_���r���[��Ԃɕϊ�
	float4x4 inv = View;
	inv = transpose(inv);
	inv._14_24_34 = 0;

	Out.Pos = mul(In.Pos , inv);
	Out.Pos = mul(Out.Pos, World);   // ���[���h�ϊ�
	Out.Pos = mul(Out.Pos, View);    // �r���[�ϊ�
	Out.Pos = mul(Out.Pos, Proj);    // �ˉe�ϊ�

	// �f�B�t���[�Y�F���v�Z
//	float4 L = normalize(LightPos - In.Pos);
//	Out.Diff = Diffuse * max(0.4, dot(In.Normal, L));

	Out.Diff = Diffuse;

	// �e�N�X�`�����W�����
	Out.Texture = In.Texture * float2(texcoord.z, texcoord.w) + float2(texcoord.x, texcoord.y);

	return Out;
}


texture Tex;		// �s�N�Z���V�F�[�_�[�Ŏg���e�N�X�`��
// �e�N�X�`���T���v��
sampler TexSampler = sampler_state
{
	Texture = (Tex);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

// �s�N�Z���V�F�[�_�[�̊֐�(�e�N�X�`������)
float4 PS_Main(PS_OUTPUT In) : COLOR0
{
	float4 outDiff = In.Diff * tex2D(TexSampler, In.Texture);

	return outDiff;
}

// �e�N�j�b�N�̐ݒ�
technique Tech		// �e�N�j�b�N���uTexTech�v
{
	pass Pass0			// 1�Ԗڂ̃p�X���uPass0�v
	{
		//�X�e�[�g�ݒ�
		ShadeMode = GOURAUD;
		ZEnable   = TRUE;
		CullMode  = CCW;
		Lighting  = FALSE;
		AlphaBlendEnable = TRUE;
		AlphaTestEnable = TRUE;
		AlphaRef = 100;
		AlphaFunc = GREATER;

		VertexShader = compile vs_2_0 VS_Main();	// ���_�V�F�[�_�[�̐ݒ�
		PixelShader  = compile ps_2_0 PS_Main();	// �s�N�Z���V�F�[�_�[�̐ݒ�
	}
}

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

struct VS_PHONG_OUTPUT
{
	float4 Pos     : POSITION;
	float4 Diff    : COLOR0;
	float2 Texture : TEXCOORD0;
	float3 N       : TEXCOORD1;   //�I�u�W�F�N�g�̖@���x�N�g��
	float3 L       : TEXCOORD2;   //���_ -> ���C�g�ʒu �x�N�g��
	float3 E       : TEXCOORD3;   //���_ -> ���_ �x�N�g��
	float  Power   : TEXCOORD4;   //����̋��x
};

// ���_�V�F�[�_�[�̊֐�
VS_PHONG_OUTPUT VS_Main(VS_INPUT In)
{
	VS_PHONG_OUTPUT Out;

	// ���[���h�ϊ�
	Out.Pos = mul(In.Pos , World);
	float4 phongPos = Out.Pos;
	// �r���[�ϊ�
	Out.Pos = mul(Out.Pos, View);
	// �ˉe�ϊ�
	Out.Pos = mul(Out.Pos, Proj);

	// �e�N�X�`�����W�͂��̂܂�
	Out.Texture = In.Texture;


	// �t�H���V�F�[�_
	// ���_���烉�C�g�ւ̂׃N�g��
	Out.L = normalize(LightPos.xyz - phongPos.xyz);
	// ���_���王�_�ւ̃x�N�g��
	Out.E = normalize(EyePos.xyz - phongPos.xyz);

	// ���f���̉�](�X�P�[��)�s��
	float4x4 rot = World;
	rot._41_42_43 = 0;			// �ړ�����������

	float4 n = mul(In.Normal, rot);   // �@���̉�]

	Out.N = normalize(n.xyz);		  // �@���x�N�g��

	//���C�g�x�N�g���Ɩ@���x�N�g���̓��ς��v�Z���A�v�Z���ʂ̐F�̍Œ�l������( m_Ambient )�ɐ�������
	Out.Diff = max(0.3, dot(Out.N, Out.L));


	// �������C�e�B���O
	//���C�g���J�����̐��ʋ߂��ɂ���قǋ��x�������Ȃ�悤�ɂ���
	Out.Power = max(0.0, dot(Out.E, -Out.L));

	return Out;
}

// ���_�V�F�[�_�[�̊֐�
VS_OUTPUT VS_Tex_Main(VS_INPUT In)
{
	VS_OUTPUT Out;

	// ���_���r���[��Ԃɕϊ�
	Out.Pos = mul(In.Pos , World);   // ���[���h�ϊ�
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
float4 PS_Tex_Main(PS_OUTPUT In) : COLOR0
{
	float4 outDiff = In.Diff * tex2D(TexSampler, In.Texture);

	return outDiff;
}

// �s�N�Z���V�F�[�_�[�̊֐�(�e�N�X�`������)
float4 PS_Phong_Main(VS_PHONG_OUTPUT In) : COLOR0
{
	float4 Out;
	
	// �t�H���V�F�[�f�B���O
	// ���C�g�ւ̃x�N�g���Ǝ��_�ւ̃x�N�g���̍���
	float3 H = normalize(In.L + In.E);
	//�X�y�L�����[�J���[���v�Z             �͈�    ���x
	float S = pow(max(0.0f, dot(In.N, H)), 10.0) * 0.75;



	// �������C�e�B���O
	//�I�u�W�F�N�g�̗֊s�t�߂̋��x�������Ȃ�悤�ɂ���
	float P = 1.0f - max(0.0f, dot(In.N, In.E));

	//�����K������͈͂���ы��x�𒲐�����
	//���̂�����̌v�Z�̓I�u�W�F�N�g�̃}�e���A���ɂ��C�ӂɕύX���邱��
	P = P * 0.6;
	P = P * P;


	// �n�[�t�����o�[�g�V�F�[�f�B���O
	float halfLambert = max(0.2, dot(In.N, In.L));
	halfLambert = halfLambert * 0.5f + 0.5;
	halfLambert = halfLambert * halfLambert;

	//�X�y�L�����[�J���[�����Z����
	Out = halfLambert * tex2D(TexSampler, In.Texture) + S + (In.Power * P);

	return Out;
}

// �s�N�Z���V�F�[�_�[(�e�N�X�`���Ȃ�)
float4 PS_Main(float4 inDiff  : COLOR0) : COLOR0
{
	return inDiff;
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
		VertexShader = compile vs_2_0 VS_Tex_Main();	// ���_�V�F�[�_�[�̐ݒ�
		PixelShader  = compile ps_2_0 PS_Tex_Main();	// �s�N�Z���V�F�[�_�[�̐ݒ�
	}
}

// �e�N�j�b�N�̐ݒ�
technique ModelTech		// �e�N�j�b�N���uTexTech�v
{
	pass Pass0			// 1�Ԗڂ̃p�X���uPass0�v
	{
		//�X�e�[�g�ݒ�
		ShadeMode = GOURAUD;
		ZEnable   = TRUE;
		CullMode  = NONE;
		Lighting  = FALSE;
		AlphaBlendEnable = FALSE;
		VertexShader = compile vs_2_0 VS_Main();	// ���_�V�F�[�_�[�̐ݒ�
		PixelShader  = compile ps_2_0 PS_Phong_Main();	// �s�N�Z���V�F�[�_�[�̐ݒ�
	}
}

technique NotTexTech		// �e�N�j�b�N��
{
	pass Pass0			// 1�Ԗڂ̃p�X���uPass0�v
	{
		//�X�e�[�g�ݒ�
		ShadeMode = GOURAUD;
		ZEnable = TRUE;
		CullMode = NONE;
		Lighting = TRUE;
		AlphaBlendEnable = FALSE;
		VertexShader = compile vs_2_0 VS_Main();	// ���_�V�F�[�_�[�̐ݒ�
		PixelShader  = compile ps_2_0 PS_Main();	// �s�N�Z���V�F�[�_�[�̐ݒ�
	}
}
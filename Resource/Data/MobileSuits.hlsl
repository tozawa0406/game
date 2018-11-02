cbuffer scene : register(b0)
{
	matrix World;
	matrix View;
	matrix Proj;
	float4 Diffuse;
	float4 EyePos;
	float4 LightPos;			// �����̈ʒu(���f�����W)
	matrix LightView;               // ���C�g�r���[�ϊ��s��
	matrix LightProj;               // �ˉe�ϊ��s��
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
	float3 N        : TEXCOORD1;   //�I�u�W�F�N�g�̖@���x�N�g��
	float3 L        : TEXCOORD2;   //���_ -> ���C�g�ʒu �x�N�g��
	float3 E        : TEXCOORD3;   //���_ -> ���_ �x�N�g��
	float  power    : TEXCOORD4;   //����̋��x
	float4 ZCalcTex : TEXCOORD5;
};

//�o�[�e�b�N�X�V�F�[�_�[
OUT_VS VS_MeshMain(IN_VS In)
{   
	OUT_VS Out = (OUT_VS)0;

	Out.position = mul(In.position , transpose(World));
	float4 phongPos = Out.position;
	Out.position = mul(Out.position, transpose(View));
	Out.position = mul(Out.position, transpose(Proj));

	// �t�H���V�F�[�_
	// ���_���烉�C�g�ւ̂׃N�g��
	Out.L = normalize(LightPos.xyz - phongPos.xyz);
	// ���_���王�_�ւ̃x�N�g��
	Out.E = normalize(EyePos.xyz - phongPos.xyz);

	// ���f���̉�](�X�P�[��)�s��
	float4x4 rot = transpose(World);
	rot._41_42_43 = 0;			// �ړ�����������

	float4 n = mul(In.normal, rot);   // �@���̉�]

	Out.N = normalize(n.xyz);		  // �@���x�N�g��

	//���C�g�x�N�g���Ɩ@���x�N�g���̓��ς��v�Z���A�v�Z���ʂ̐F�̍Œ�l������( m_Ambient )�ɐ�������
	Out.color = max(0.3, dot(Out.N, Out.L));

	// �������C�e�B���O
	//���C�g���J�����̐��ʋ߂��ɂ���قǋ��x�������Ȃ�悤�ɂ���
	Out.power = max(0.0, dot(Out.E, -Out.L));

	//// ���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
	float4 calc;
	calc = mul(In.position, transpose(World));
	calc = mul(calc, transpose(LightView));
	calc = mul(calc, transpose(LightProj));
	Out.ZCalcTex = calc;

	// �@���ƃ��C�g�̕������璸�_�̐F������
	// �Z���Ȃ肷���Ȃ��悤�ɒ��߂��Ă��܂�
	float3 N = normalize(mul((float3)In.normal, rot));
	float3 lightDirect = normalize(LightPos.xyz);
	float k = max(dot(Out.N, -lightDirect), 0.0f);
	Out.color = Diffuse * (0.8 + k);
	Out.color.a = 1;

	Out.texcoord = In.texcoord;

    return Out;
}

//�s�N�Z���V�F�[�_�[
float4 PS_Main(OUT_VS In) : SV_Target
{
	// �t�H���V�F�[�f�B���O
	// ���C�g�ւ̃x�N�g���Ǝ��_�ւ̃x�N�g���̍���
	float3 H = normalize(In.L + In.E);
	//�X�y�L�����[�J���[���v�Z             �͈�    ���x
	float S = pow(max(0.0f, dot(In.N, H)), 10.0) * 0.75;

	// �������C�e�B���O
	// �I�u�W�F�N�g�̗֊s�t�߂̋��x�������Ȃ�悤�ɂ���
	float P = 1.0f - max(0.0f, dot(In.N, In.E));

	// �����K������͈͂���ы��x�𒲐�����
	// ���̂�����̌v�Z�̓I�u�W�F�N�g�̃}�e���A���ɂ��C�ӂɕύX���邱��
	P = P * 0.8;
	P = P * P;

	// �n�[�t�����o�[�g�V�F�[�f�B���O
	float halfLambert = max(0.2, dot(In.N, In.L));
	halfLambert = halfLambert * 0.5f + 0.5;
	halfLambert = halfLambert * halfLambert;

	// �X�y�L�����[�J���[�����Z����
	float4 color = halfLambert * (In.color * DiffuseTexture.Sample(DiffuseSampler, In.texcoord)) + S + (In.power * P);

	// ���C�g�ڐ��ɂ��Z�l�̍ĎZ�o
	float inv = 1.0f / In.ZCalcTex.w;
	float zValue = In.ZCalcTex.z * inv;

	// �ˉe��Ԃ�XY���W���e�N�X�`�����W�ɕϊ�
	float3 transTexCoord;
	transTexCoord.x = (1.0f + In.ZCalcTex.x * inv) * 0.5f;
	transTexCoord.y = (1.0f - In.ZCalcTex.y * inv) * 0.5f;

	float4 texColor = ShadowTexture.Sample(ShadowSampler, transTexCoord);

	// ���A��Z�l���o
	float SM_Z = texColor.r;

	// �Z�o�_���V���h�E�}�b�v��Z�l�����傫����Ήe�Ɣ��f
	if (zValue > SM_Z + 0.0005f)
	{
		color.rgb *= 0.5f;
	}
	color.a = 1;

	return color;
}

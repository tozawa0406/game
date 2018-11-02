float4x4 World;		// �ϊ��s��
float4x4 View;
float4x4 Proj;
float4   EyePos;
float4   LightPos;			// �����̈ʒu(���f�����W)
float4x4 LightView;               // ���C�g�r���[�ϊ��s��
float4x4 LightProj;               // �ˉe�ϊ��s��
float4   Diffuse;			// �f�B�t�[�Y�F
float4   texcoord;

// ���_�V�F�[�_�[�̊֐�
void VS_Main(
	in float4 InPos     : POSITION,
	in float4 InNormal  : NORMAL0,
	in float4 InDiff    : COLOR0,
	in float2 InTexture : TEXCOORD0,

	out float4 OutPos     : POSITION,
	out float4 OutDiff    : COLOR0,
	out float2 OutTexture : TEXCOORD0,
	out float3 OutN       : TEXCOORD1,   //�I�u�W�F�N�g�̖@���x�N�g��
	out float3 OutL       : TEXCOORD2,   //���_ -> ���C�g�ʒu �x�N�g��
	out float3 OutE       : TEXCOORD3,   //���_ -> ���_ �x�N�g��
	out float  OutPower   : TEXCOORD4,   //����̋��x
	out float4 OutZCalc   : TEXCOORD5 // Z�l�Z�o�p�e�N�X�`��
)
{
	OutPos = mul(InPos , World);
	float4 phongPos = OutPos;
	OutPos = mul(OutPos, View);
	OutPos = mul(OutPos, Proj);

	// �e�N�X�`�����W�͂��̂܂�
	OutTexture = InTexture;

	// �t�H���V�F�[�_
	// ���_���烉�C�g�ւ̂׃N�g��
	OutL = normalize(LightPos.xyz - phongPos.xyz);
	// ���_���王�_�ւ̃x�N�g��
	OutE = normalize(EyePos.xyz - phongPos.xyz);

	// ���f���̉�](�X�P�[��)�s��
	float4x4 rot = World;
	rot._41_42_43 = 0;			// �ړ�����������

	float4 n = mul(InNormal, rot);   // �@���̉�]

	OutN = normalize(n.xyz);		  // �@���x�N�g��

	//���C�g�x�N�g���Ɩ@���x�N�g���̓��ς��v�Z���A�v�Z���ʂ̐F�̍Œ�l������( m_Ambient )�ɐ�������
	OutDiff = max(0.3, dot(OutN, OutL));


	// �������C�e�B���O
	//���C�g���J�����̐��ʋ߂��ɂ���قǋ��x�������Ȃ�悤�ɂ���
	OutPower = max(0.0, dot(OutE, -OutL));


	// �e
	// ���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
	float4x4 mtx;
	mtx = mul(World, LightView);
	mtx = mul(mtx  , LightProj);
	OutZCalc = mul(InPos, mtx);

	// �@���ƃ��C�g�̕������璸�_�̐F������
	// �Z���Ȃ肷���Ȃ��悤�ɒ��߂��Ă��܂�
	mtx = World;
	mtx._41_42_43 = 0;
	float3 N = normalize(mul((float3)InNormal, mtx));
	float3 lightDirect = normalize(float3(LightView._13, LightView._23, LightView._33));
	float k = max(dot(N, -lightDirect), 0.0f);
	OutDiff = Diffuse * (0.7 + k);
	OutDiff.a = 1;
}

sampler DefSampler = sampler_state
{
	texture = (texShadowMap);
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

// �e�N�X�`���T���v��
sampler TexSampler = sampler_state
{
	Texture = (Tex);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

// �s�N�Z���V�F�[�_�[�̊֐�(�e�N�X�`������)
void PS_Main(
	in float4 InPos     : POSITION,
	in float4 InDiff    : COLOR0,
	in float2 InTexture : TEXCOORD0,
	in float3 InN       : TEXCOORD1,	//�I�u�W�F�N�g�̖@���x�N�g��
	in float3 InL       : TEXCOORD2,	//���_ -> ���C�g�ʒu �x�N�g��
	in float3 InE       : TEXCOORD3,	//���_ -> ���_ �x�N�g��
	in float  InPower   : TEXCOORD4,	//����̋��x
	in float4 InZCalc   : TEXCOORD5,	// Z�l�Z�o�p�e�N�X�`��

	out float4 Out : COLOR0
)
{
	Out = InDiff * tex2D(TexSampler, InTexture);
	
	// �t�H���V�F�[�f�B���O
	// ���C�g�ւ̃x�N�g���Ǝ��_�ւ̃x�N�g���̍���
	float3 H = normalize(InL + InE);
	//�X�y�L�����[�J���[���v�Z             �͈�    ���x
	float S = pow(max(0.0f, dot(InN, H)), 10.0) * 0.75;

	// �������C�e�B���O
	//�I�u�W�F�N�g�̗֊s�t�߂̋��x�������Ȃ�悤�ɂ���
	float P = 1.0f - max(0.0f, dot(InN, InE));

	//�����K������͈͂���ы��x�𒲐�����
	//���̂�����̌v�Z�̓I�u�W�F�N�g�̃}�e���A���ɂ��C�ӂɕύX���邱��
	P = P * 0.8;
	P = P * P;

	// �n�[�t�����o�[�g�V�F�[�f�B���O
	float halfLambert = max(0.2, dot(InN, InL));
	halfLambert = halfLambert * 0.5f + 0.5;
	halfLambert = halfLambert * halfLambert;

	//�X�y�L�����[�J���[�����Z����
	Out = halfLambert * (InDiff * tex2D(TexSampler, InTexture)) + S + (InPower * P);

	// ���C�g�ڐ��ɂ��Z�l�̍ĎZ�o
	float inv = 1.0f / InZCalc.w;
	float zValue = InZCalc.z * inv;

	// �ˉe��Ԃ�XY���W���e�N�X�`�����W�ɕϊ�
	float3 transTexCoord;
	transTexCoord.x = (1.0f + InZCalc.x * inv) * 0.5f;
	transTexCoord.y = (1.0f - InZCalc.y * inv) * 0.5f;

	float4 texColor = tex2D(DefSampler, transTexCoord);

	// ���A��Z�l���o
	float SM_Z = texColor.r + (texColor.g + (texColor.b / 256.0f) / 256.0f) / 256.0f;

	// �Z�o�_���V���h�E�}�b�v��Z�l�����傫����Ήe�Ɣ��f
	if (zValue > SM_Z + 0.0019f)
	{
		Out.rgb *= 0.5f;
	}
	Out.a = 1;
}

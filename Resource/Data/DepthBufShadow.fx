float4x4 World;		// �ϊ��s��
float4x4 View;
float4x4 Proj;
float4x4 LightView;               // ���C�g�r���[�ϊ��s��
float4x4 LightProj;               // �ˉe�ϊ��s��
float4   Diffuse;
float4   texcoord;

// ���_�V�F�[�_
void VS_DepthBufShadow(
	in  float4 InPos       : POSITION,
	in  float3 InNorm      : NORMAL,
	in  float2 InTexcoord  : TEXCOORD0,

	out float4 OutPos      : POSITION,
	out float2 OutTexcoord : TEXCOORD0,
	out float4 OutZCalcTex : TEXCOORD1,
	out float4 OutColor    : COLOR0
)
{
	// ���ʂɃJ�����̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
	OutPos = mul(InPos , World);
	OutPos = mul(OutPos, View);
	OutPos = mul(OutPos, Proj);

	// ���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
	float4x4 mtx;
	mtx = mul(World, LightView);
	mtx = mul(mtx  , LightProj);
	OutZCalcTex = mul(InPos, mtx);

	// �@���ƃ��C�g�̕������璸�_�̐F������
	// �Z���Ȃ肷���Ȃ��悤�ɒ��߂��Ă��܂�
	mtx = World;
	mtx._41_42_43 = 0;
	float3 N = normalize(mul(InNorm, mtx));
	float3 lightDirect = normalize(float3(LightView._13, LightView._23, LightView._33));
	float k = max(dot(N, -lightDirect), 0.0f);
	OutColor = Diffuse * (0.3 + k);
	OutColor.a = 1;

	OutTexcoord = InTexcoord;
}

sampler DefSampler = sampler_state	// �T���v���[�X�e�[�g
{
	texture = (texShadowMap);
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

sampler TexSampler = sampler_state
{
	Texture = (Tex);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

// �s�N�Z���V�F�[�_
void PS_DepthBufShadow(
	in  float4 InColor    : COLOR0,
	in  float2 InTexcoord : TEXCOORD0,
	in  float4 InZCalcTex : TEXCOORD1,

	out float4 OutColor   : COLOR0
)
{
	float4 Out = InColor * tex2D(TexSampler, InTexcoord);

	// ���C�g�ڐ��ɂ��Z�l�̍ĎZ�o
	float inv = 1.0f / InZCalcTex.w;
	float zValue = InZCalcTex.z * inv;

	// �ˉe��Ԃ�XY���W���e�N�X�`�����W�ɕϊ�
	float3 transTexCoord;
	transTexCoord.x = (1.0f + InZCalcTex.x * inv) * 0.5f;
	transTexCoord.y = (1.0f - InZCalcTex.y * inv) * 0.5f;

	float4 texColor = tex2D(DefSampler, transTexCoord);

	// ���A��Z�l���o
	float SM_Z = texColor.r + (texColor.g + (texColor.b / 256.0f) / 256.0f) / 256.0f;

	// �Z�o�_���V���h�E�}�b�v��Z�l�����傫����Ήe�Ɣ��f
	if (zValue > SM_Z + 0.0032f)
	{
		Out.rgb *= 0.5f;
	}

	OutColor = Out;
}

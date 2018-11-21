cbuffer scene : register(b0)
{
	matrix World;
	matrix View;
	matrix Proj;
	float4 Texcoord;
	float4 Color;
	float4 LightPosition;
	matrix LightView;               // ���C�g�r���[�ϊ��s��
	matrix LightProj;               // �ˉe�ϊ��s��
};

cbuffer bone : register(b1)
{
	matrix BoneInv[512];
	matrix BoneAnim[512];
};

Texture2D    DiffuseTexture  : register(t0);   
SamplerState DiffuseSampler  : register(s0);

Texture2D    ShadowTexture : register(t1);
SamplerState ShadowSampler : register(s1);

Texture2D    NormalTexture  : register(t2);
SamplerState NormalSampler  : register(s2);

struct IN_VS
{
	float4 position : POSITION0;
	float4 normal   : NORMAL0;
	float4 tangent	: TANGENT0;
	float4 color    : COLOR0;
	float2 texcoord : TEXCOORD0;
	float4 boneIndex: TEXCOORD1;
	float4 weight   : TEXCOORD2;
};

struct OUT_VS
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float4 ZCalcTex : TEXCOORD1;
	float3 eye		: TEXCOORD2;
	float3 light	: TEXCOORD3;
	float4 color    : COLOR0;
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
	float4 position = Out.position;

	// ���W�n�̌v�Z
	Out.position = mul(Out.position, transpose(World));
	Out.position = mul(Out.position, transpose(View));
	Out.position = mul(Out.position, transpose(Proj));

	float3 N = normalize(In.normal.xyz);
	float3 T = normalize(In.tangent.xyz);
	float3 B = normalize(cross(N, T));

	//�����x�N�g�����v�Z
	float3 eye = normalize(LightPosition - position);

	//�����x�N�g���𒸓_���W�n�ɕϊ�����
	Out.eye.x = dot(eye, T);
	Out.eye.y = dot(eye, B);
	Out.eye.z = dot(eye, N);
	Out.eye = normalize(Out.eye);

	//���_���W -> ���C�g�̈ʒu�x�N�g��
	float3 light = normalize(position - LightPosition);

	//���C�g�x�N�g���𒸓_���W�n�ɕϊ�����
	Out.light.x = dot(light, T);
	Out.light.y = dot(light, B);
	Out.light.z = dot(light, N);
	Out.light = normalize(Out.light);

	// ���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
	float4 calc;
	calc = mul(In.position, transpose(World));
	calc = mul(calc, transpose(LightView));
	calc = mul(calc, transpose(LightProj));
	Out.ZCalcTex = calc;

	Out.color = Color;

	Out.texcoord = In.texcoord;
	
	return Out;
}

//�s�N�Z���V�F�[�_�[
float4 PS_Main(OUT_VS In) : SV_Target
{
	//�@���}�b�v���Q�Ƃ��A�@�����擾����
	//�@���}�b�v�� 0.0f �` 1.0f �͈̔͂ɕۑ����Ă���̂� -1.0f �` 1.0f �͈̔͂ɕϊ�����
	float3 normal = 2.0f * NormalTexture.Sample(NormalSampler, In.texcoord).xyz - 1.0f;

	//�t�H���V�F�[�f�B���O�ɂ��X�y�L�����[�̐F���v�Z����
	//�n�[�t�x�N�g���̌v�Z
	float3 H = normalize(In.light + In.eye);

	//�X�y�L�����[�J���[���v�Z����
	//	float S = pow(max(0.0f, dot(Normal, H)), m_Specular) * m_SpecularPower;
	float S = 0;

	//��������
	float4 Out = In.color * DiffuseTexture.Sample(DiffuseSampler, In.texcoord);
	Out.xyz *= max(0.6f, dot(normal.xyz, In.light.xyz)) + S;

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
		Out.rgb *= 0.5f;
	}

	return Out;
}

cbuffer scene : register(b0)
{
	matrix World;
	matrix View;
	matrix Proj;
	float4 Texcoord;
	float4 Color;
	matrix LightView;               // ���C�g�r���[�ϊ��s��
	matrix LightProj;               // �ˉe�ϊ��s��
};

Texture2D    DiffuseTexture  : register(t0);   
SamplerState DiffuseSampler  : register(s0);

Texture2D    ShadowTexture : register(t1);
SamplerState ShadowSampler : register(s1);

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
	float4 color    : COLOR0;
	float4 worldPosition : TEXCOORD2;
	float4 worldNormal   : TEXCOORD3;
};

struct OUT_PS
{
	float4 target0 : SV_Target0;
	float4 target1 : SV_Target1;
	float4 target2 : SV_Target2;
};

//�o�[�e�b�N�X�V�F�[�_�[
OUT_VS VS_DepthBufShadow(IN_VS In)
{   
	OUT_VS Out = (OUT_VS)0;

	Out.position = mul(In.position , transpose(World));
	Out.worldPosition = Out.position;
	Out.position = mul(Out.position, transpose(View));
	Out.position = mul(Out.position, transpose(Proj));

	//// ���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
	float4 calc;
	calc = mul(In.position, transpose(World));
	calc = mul(calc       , transpose(LightView));
	calc = mul(calc       , transpose(LightProj));
	Out.ZCalcTex = calc;

	// �@���ƃ��C�g�̕������璸�_�̐F������
	// �Z���Ȃ肷���Ȃ��悤�ɒ��߂��Ă��܂�
	matrix mtx = transpose(World);
	mtx._41_42_43 = 0;
	float3 N = normalize(mul(In.normal, mtx).xyz);
	float3 lightDirect = normalize(float3(LightView._31, LightView._32, LightView._33));
	float k = max(dot(N, -lightDirect), 0.0f);
	Out.color = Color * (0.3 + k);
	Out.color.a = 1;

	Out.color = Color;

	Out.texcoord = In.texcoord;
	Out.worldNormal.xyz = mul(In.normal.xyz, (float3x3)transpose(World));
	Out.worldNormal = normalize(Out.worldNormal);
	Out.worldNormal.w = 1;

    return Out;
}

//�s�N�Z���V�F�[�_�[
OUT_PS PS_DepthBufShadow(OUT_VS In)
{
	float4 Out = In.color * DiffuseTexture.Sample(DiffuseSampler, In.texcoord);

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

	OUT_PS outPS;
	outPS.target0 = Out;
	outPS.target1 = In.worldPosition;
	outPS.target2 = In.worldNormal;
	return outPS;
}

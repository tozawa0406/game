float4x4 World;		// �ϊ��s��
float4x4 View;
float4x4 Proj;
float4   texcoord;

// ���_�V�F�[�_
void VS_ZBufferCalc(
	in  float4 InPos	   : POSITION,
	in  float2 InTexcoord  : TEXCOORD0,

	out float4 OutPos	   : POSITION,
	out float2 OutTexcoord : TEXCOORD0,
	out float4 OutZTex     : TEXCOORD1)
{

	// ���ʂɃ��[���h�r���[�ˉe�s�������
	OutPos = mul(InPos , World);
	OutPos = mul(OutPos, View);
	OutPos = mul(OutPos, Proj);

	// �e�N�X�`�����W�𒸓_�ɍ��킹��
	OutZTex = OutPos;

	// �r���{�[�h�̃�����Ŏg��
	OutTexcoord = InTexcoord * float2(texcoord.z, texcoord.w) + float2(texcoord.x, texcoord.y);
}

sampler TexSampler = sampler_state
{
	Texture = (Tex);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

// �s�N�Z���V�F�[�_
void PS_ZBufferPlot(in float2 InTexcoord : TEXCOORD0, in float4 InZTex : TEXCOORD1, out float4 OutColor : COLOR)
{
	float4 color = tex2D(TexSampler, InTexcoord);

	// �[�x�Z�o
	float depth = InZTex.z / InZTex.w;

	float4 unpackedDepth = float4(0, 0, 256.0f, 256.0f);
	unpackedDepth.g  = modf(depth * 256.0f, unpackedDepth.r);
	unpackedDepth.b *= modf(unpackedDepth.g * 256.0f, unpackedDepth.g);

	unpackedDepth.a *= color.a;

	OutColor = unpackedDepth / 256.0f;  // �W����
}

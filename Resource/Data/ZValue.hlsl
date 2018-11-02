cbuffer scene : register(b0)
{
	matrix World;
	matrix View;
	matrix Proj;
	float4 Texcoord;
};

Texture2D    Texture : register(t0);   
SamplerState Sampler : register(s0);

struct IN_VS
{
	float4 position : POSITION;
	float4 normal   : NORMAL;
	float4 color    : COLOR0;
	float2 texcoord : TEXCOORD0;
};

struct OUT_VS
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float4 ZTex     : TEXCOORD1;
};

//�o�[�e�b�N�X�V�F�[�_�[
OUT_VS VS_ZBufferCalc(IN_VS In = (IN_VS)0)
{   
	OUT_VS Out = (OUT_VS)0;

	Out.position = mul(In.position , transpose(World));
	Out.position = mul(Out.position, transpose(View));
	Out.position = mul(Out.position, transpose(Proj));

	// �e�N�X�`�����W�𒸓_�ɍ��킹��
	Out.ZTex = Out.position;

	// �r���{�[�h�̃�����Ŏg��
	Out.texcoord = In.texcoord *float2(Texcoord.z, Texcoord.w) + float2(Texcoord.x, Texcoord.y);

    return Out;
}

//�s�N�Z���V�F�[�_�[
float4 PS_ZBufferPlot(OUT_VS In) : SV_Target
{
	float4 color = Texture.Sample(Sampler, In.texcoord);

	// �[�x�Z�o
	float depth = (In.ZTex.z / In.ZTex.w);

	return float4(depth, 0, 0, color.a);
}

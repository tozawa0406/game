cbuffer scene : register(b0)
{
	matrix World;
	matrix View;
	matrix Proj;
	float4 Diffuse;
};

Texture2D    Texture : register(t0);   
SamplerState Sampler : register(s0);

struct VS_IN
{
	float4 position : POSITION;
	float  size     : PSIZE;
	float4 color    : COLOR;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float  size     : PSIZE;
	float4 color    : COLOR;
};

typedef VS_OUT GS_IN;

struct GS_OUT
{
	float4 position : SV_POSITION;
	float4 color    : COLOR;
	float2 texcoord : TEXCOORD0;
};

typedef GS_OUT PS_IN;

//バーテックスシェーダー
VS_OUT VS_Main(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	Out.position = In.position;
	Out.size     = In.size;
	Out.color    = In.color;

	return Out;
}

// ジオメトリシェーダー
[maxvertexcount(4)]   // ジオメトリシェーダーで出力する最大頂点数
void GS_Main(point GS_IN In[1],                       // ポイント プリミティブの入力情報
	inout TriangleStream<GS_OUT> TriStream  // トライアングル プリミティブの出力ストリーム
)
{
	GS_OUT Out;
	float4 position;

	matrix proj = transpose(Proj);
	float d = length(In[0].position.xyz);
	float s = In[0].size * 0.5f;
	Out.color = Diffuse;

	float4x4 inv = View;
	inv._14_24_34 = 0;

	matrix translation = 0;
	translation._11_22_33_44 = 1;
	matrix world = transpose(World);
	translation._41_42_43 = world._41_42_43;
	world._41_42_43 = 0;

	position = float4(-s,  +s, 0, 1.0f);
	Out.position = position;
	Out.position = mul(Out.position, world);
	Out.position = mul(Out.position, inv);
	Out.position = mul(Out.position, translation);
	Out.position = mul(Out.position, transpose(View));
	Out.position = mul(Out.position, transpose(Proj));
	Out.texcoord = float2(0, 0);
	TriStream.Append(Out);

	position = float4(s, +s, 0, 1.0f);
	Out.position = position;
	Out.position = mul(Out.position, world);
	Out.position = mul(Out.position, inv);
	Out.position = mul(Out.position, translation);
	Out.position = mul(Out.position, transpose(View));
	Out.position = mul(Out.position, transpose(Proj));
	Out.texcoord = float2(1, 0);
	TriStream.Append(Out);

	position = float4(-s, -s, 0, 1.0f);
	Out.position = position;
	Out.position = mul(Out.position, world);
	Out.position = mul(Out.position, inv);
	Out.position = mul(Out.position, translation);
	Out.position = mul(Out.position, transpose(View));
	Out.position = mul(Out.position, transpose(Proj));
	Out.texcoord = float2(0, 1);
	TriStream.Append(Out);

	position = float4(s, -s, 0, 1.0f);
	Out.position = position;
	Out.position = mul(Out.position, world);
	Out.position = mul(Out.position, inv);
	Out.position = mul(Out.position, translation);
	Out.position = mul(Out.position, transpose(View));
	Out.position = mul(Out.position, transpose(Proj));
	Out.texcoord = float2(1, 1);
	TriStream.Append(Out);

	TriStream.RestartStrip();
}

//ピクセルシェーダー
float4 PS_Main(PS_IN In) : SV_Target
{
	return In.color * Texture.Sample(Sampler, In.texcoord);
}

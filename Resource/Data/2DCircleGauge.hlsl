//グローバル
cbuffer global : register(b0)
{
	float4 Screen;
};

cbuffer data : register(b1)
{
	float4 Position;
	float4 SizeSplit;
	float4 Scale;
	float4 RotationPattern;
	float4 Diffuse;
};

Texture2D    Texture : register(t0);   
SamplerState Sampler : register(s0);

struct IN_VS
{
	float4 position : POSITION;
	float4 color    : COLOR0;
	float2 texcoord : TEXCOORD0;
};

struct OUT_VS
{
	float4 position : SV_POSITION;
	float4 color    : COLOR0;
	float2 texcoord : TEXCOORD0;
};

//バーテックスシェーダー
OUT_VS VS_Main(IN_VS In)
{   
	OUT_VS Out = (OUT_VS)0;

	float px = (((SizeSplit.x * In.position.x) - Scale.z) * Scale.x + (Scale.z));
	float py = (((SizeSplit.y * In.position.y) - Scale.w) * Scale.y + (Scale.w));

	px -= RotationPattern.z;
	py -= RotationPattern.w;

	float4 pos = Position;
	pos.x += RotationPattern.z - (SizeSplit.x * 0.5f);
	pos.y += RotationPattern.w - (SizeSplit.y * 0.5f);

	pos = float4(pos.x + px * cos(0) - py * sin(0), pos.y + px * sin(0) + py * cos(0), 1.0f, 1.0f);

	float2 inv = { 1 / SizeSplit.z, 1 / SizeSplit.w };
	int splitX = SizeSplit.z;
	int p = RotationPattern.x;

	// 画面座標への変換
	Out.position.x =  pos.x / Screen.x * 2 - 1;
	Out.position.y = -pos.y / Screen.y * 2 + 1;
	Out.position.w = 1;
	Out.position.z = 1;

	Out.color    = Diffuse;
	Out.texcoord = In.position.xy;

    return Out;
}

//ピクセルシェーダー
float4 PS_Main(OUT_VS In) : SV_Target
{
	float2 temp = In.texcoord - float2(0.5f, 0.5f);
	clip(degrees(atan2(temp.x, temp.y)) - degrees(Screen.z));

	float  radius = 0.5f;
	float  r = distance(In.texcoord, float2(0.5f, 0.5f));
	float4 c = In.color;
	c.a = !smoothstep(radius, radius + 0.02f, r) * c.a;
	return c;
}

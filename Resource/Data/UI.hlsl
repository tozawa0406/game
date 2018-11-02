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
	float4 color    : COLOR;
	float2 texcoord : TEXCOORD;
};

struct OUT_VS
{
	float4 position : SV_POSITION;
	float4 color    : COLOR;
	float2 texcoord : TEXCOORD;
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

	pos = float4(pos.x + px * cos(RotationPattern.y) - py * sin(RotationPattern.y),pos.y + px * sin(RotationPattern.y) + py * cos(RotationPattern.y), 1.0f, 1.0f);

	float2 inv = { 1 / SizeSplit.z, 1 / SizeSplit.w };
	int splitX = SizeSplit.z;
	int p = RotationPattern.x;

	float2 uv;
	uv.x = inv.x * ((p % splitX) + (1 * In.position.x));
	uv.y = inv.y * ((p / splitX) + (1 * In.position.y));

	// 画面座標への変換
	Out.position.x =  pos.x / Screen.x * 2 - 1;
	Out.position.y = -pos.y / Screen.y * 2 + 1;
	Out.position.w = 1;
	Out.position.z = 1;

	Out.color	 = Diffuse;
	Out.texcoord = uv;

    return Out;
}

//ピクセルシェーダー
float4 PS_Main(OUT_VS In) : SV_Target
{
	return In.color * Texture.Sample(Sampler, In.texcoord);
}

//ピクセルシェーダー
float4 PS_NotTexture(OUT_VS In) : SV_Target
{
	return In.color;
}
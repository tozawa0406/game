float4x4 World;		// 変換行列
float4x4 View;
float4x4 Proj;
float4   texcoord;

// 頂点シェーダ
void VS_ZBufferCalc(
	in  float4 InPos	   : POSITION,
	in  float2 InTexcoord  : TEXCOORD0,

	out float4 OutPos	   : POSITION,
	out float2 OutTexcoord : TEXCOORD0,
	out float4 OutZTex     : TEXCOORD1)
{

	// 普通にワールドビュー射影行列をする
	OutPos = mul(InPos , World);
	OutPos = mul(OutPos, View);
	OutPos = mul(OutPos, Proj);

	// テクスチャ座標を頂点に合わせる
	OutZTex = OutPos;

	// ビルボードのα判定で使う
	OutTexcoord = InTexcoord * float2(texcoord.z, texcoord.w) + float2(texcoord.x, texcoord.y);
}

sampler TexSampler = sampler_state
{
	Texture = (Tex);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

// ピクセルシェーダ
void PS_ZBufferPlot(in float2 InTexcoord : TEXCOORD0, in float4 InZTex : TEXCOORD1, out float4 OutColor : COLOR)
{
	float4 color = tex2D(TexSampler, InTexcoord);

	// 深度算出
	float depth = InZTex.z / InZTex.w;

	float4 unpackedDepth = float4(0, 0, 256.0f, 256.0f);
	unpackedDepth.g  = modf(depth * 256.0f, unpackedDepth.r);
	unpackedDepth.b *= modf(unpackedDepth.g * 256.0f, unpackedDepth.g);

	unpackedDepth.a *= color.a;

	OutColor = unpackedDepth / 256.0f;  // 標準化
}

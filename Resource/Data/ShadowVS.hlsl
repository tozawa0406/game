struct VSInput
{
	float4 Position : POSITION0;
	float4 Normal   : NORMAL0;
	float4 Tangent	: TANGENT0;
	float4 color    : COLOR0;
	float2 TexCoord : TEXCOORD0;
	float4 boneIndex: TEXCOORD1;
	float4 weight   : TEXCOORD2;
};


///////////////////////////////////////////////////////////////////////////////////////////
// VSOutput structure
///////////////////////////////////////////////////////////////////////////////////////////
struct VSOutput
{
    float4  Position : SV_POSITION;         //!< 位置座標です(ビュー射影空間).
};

///////////////////////////////////////////////////////////////////////////////////////////
// CBMatrix buffer
///////////////////////////////////////////////////////////////////////////////////////////
cbuffer CBMatrix : register( b1 )
{
    float4x4    World       : packoffset( c0 );     //!< ワールド行列です.
    float4x4    ViewProj    : packoffset( c4 );     //!< ビュー射影行列です.
};


//-----------------------------------------------------------------------------------------
//! @brief      頂点シェーダメインエントリーポイントです.
//-----------------------------------------------------------------------------------------
VSOutput VSFunc( VSInput input )
{
    VSOutput output = (VSOutput)0;

    float4 localPos    = input.Position;
    float4 worldPos    = mul( World,    localPos );
    float4 viewProjPos = mul( ViewProj, worldPos );

    output.Position = viewProjPos;

    return output;
}

/* この頂点シェーダに対応するピクセルシェーダはありません.*/

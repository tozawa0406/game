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
    float4  Position : SV_POSITION;         //!< �ʒu���W�ł�(�r���[�ˉe���).
};

///////////////////////////////////////////////////////////////////////////////////////////
// CBMatrix buffer
///////////////////////////////////////////////////////////////////////////////////////////
cbuffer CBMatrix : register( b1 )
{
    float4x4    World       : packoffset( c0 );     //!< ���[���h�s��ł�.
    float4x4    ViewProj    : packoffset( c4 );     //!< �r���[�ˉe�s��ł�.
};


//-----------------------------------------------------------------------------------------
//! @brief      ���_�V�F�[�_���C���G���g���[�|�C���g�ł�.
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

/* ���̒��_�V�F�[�_�ɑΉ�����s�N�Z���V�F�[�_�͂���܂���.*/

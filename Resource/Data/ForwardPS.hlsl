// �~�����ł�.
#define PI          3.1415926535f

struct VSOutput
{
    float4  Position    : SV_POSITION;          //!< �X�N���[����Ԃ̈ʒu���W�ł�.
    float4  WorldPos    : WORLD_POSIITON;       //!< ���[���h��Ԃ̈ʒu���W�ł�.
    float3  Normal      : NORMAL;               //!< �@���x�N�g���ł�.
    float2  TexCoord    : TEXCOORD0;            //!< �e�N�X�`�����W�ł�.
    float3  LightDir    : LIGHT_DIRECTION;      //!< ���C�g�̕����x�N�g���ł�.
    float3  CameraPos   : CAMERA_POSITION;      //!< �J�����ʒu�ł�.
    float4  SplitPos    : SPLIT_POSITION;       //!< �J�X�P�[�h�V���h�E�}�b�v�p��������.
    float4  SdwCoord[4] : SHADOW_COORD;         //!< �J�X�P�[�h�V���h�E�}�b�v�p�ʒu���W.
};

Texture2D       ShadowMap0    : register(t3);     //!< �V���h�E�}�b�v(�J�X�P�[�h0)�ł�.
Texture2D       ShadowMap1    : register(t4);     //!< �V���h�E�}�b�v(�J�X�P�[�h1)�ł�.
Texture2D       ShadowMap2    : register(t5);     //!< �V���h�E�}�b�v(�J�X�P�[�h2)�ł�.
Texture2D       ShadowMap3    : register(t6);     //!< �V���h�E�}�b�v(�J�X�P�[�h3)�ł�.

SamplerComparisonState  ShadowSmp     : register(s3);     //!< �V���h�E�}�b�v�p�T���v���[��r�X�e�[�g�ł�.

struct PSOutput
{
    float4 Color : SV_TARGET0;          //!< �^�[�Q�b�g0�̏o�̓J���[�ł�.
};

float3 NormalizedLambert( float3 diffuse, float3 lightDir, float3 normal )
{
   return diffuse * max( dot( normal, lightDir ), 0.0f ) * ( 1.0f / PI );
}

float3 NormalizedPhong( float3 specular, float power, float3 viewDir, float3 normal, float3 lightDir )
{
    float3 R = -viewDir + ( 2.0f * dot( normal, viewDir ) * normal );

    return specular * pow( max ( dot( lightDir, R ), 0.0f ), power ) * ( ( power + 1.0f )/ ( 2.0 * PI ) );
}

PSOutput PSFunc( VSOutput input )
{
    PSOutput output = (PSOutput)0;

    // �f�B�t���[�Y�}�b�v���t�F�b�`.
//    float4 mapKd = DiffuseMap.Sample( DiffuseSmp, input.TexCoord );

    // �A���t�@�e�X�g.
//    clip( ( mapKd.a < 0.125f ) ? -1.0f : 1.0f );

    // �X�y�L�����[�}�b�v���t�F�b�`.
//    float4 mapKs = SpecularMap.Sample( SpecularSmp, input.TexCoord );

    float sdwThreshold = 1.0f;      // �V���h�E�ɂ��邩�ǂ�����臒l�ł�.
    float sdwBias      = 0.01f;     // �V���h�E�o�C�A�X�ł�.
    float sdwColor     = 0.1f;      // �V���h�E�̃J���[�ł�.

    // �e�s�N�Z���ʒu�܂ł̋���.
    float dist = input.Position.w;  // �r���[��Ԃł�Z���W.

    //int index = 0;
    if ( dist < input.SplitPos.x )
    {
        //index = 0;
        float2 coord = input.SdwCoord[0].xy / input.SdwCoord[0].w;
        float  depth = input.SdwCoord[0].z  / input.SdwCoord[0].w;
        sdwThreshold = ShadowMap0.SampleCmpLevelZero( ShadowSmp, coord, depth - sdwBias );
        sdwThreshold = saturate( sdwThreshold + sdwColor );
    }
    else if ( dist < input.SplitPos.y )
    {
        //index = 1;
        float2 coord = input.SdwCoord[1].xy / input.SdwCoord[1].w;
        float  depth = input.SdwCoord[1].z  / input.SdwCoord[1].w;
        sdwThreshold = ShadowMap1.SampleCmpLevelZero( ShadowSmp, coord, depth - sdwBias );
        sdwThreshold = saturate( sdwThreshold + sdwColor );
    }
    else if ( dist < input.SplitPos.z )
    {
        //index = 2;
        float2 coord = input.SdwCoord[2].xy / input.SdwCoord[2].w;
        float  depth = input.SdwCoord[2].z  / input.SdwCoord[2].w;
        sdwThreshold = ShadowMap2.SampleCmpLevelZero( ShadowSmp, coord, depth - sdwBias );
        sdwThreshold = saturate( sdwThreshold + sdwColor );
    }
    else
    {
        //index = 3;
        float2 coord = input.SdwCoord[3].xy / input.SdwCoord[3].w;
        float  depth = input.SdwCoord[3].z  / input.SdwCoord[3].w;
        sdwThreshold = ShadowMap3.SampleCmpLevelZero( ShadowSmp, coord, depth - sdwBias );
        sdwThreshold = saturate( sdwThreshold + sdwColor );
    }

    // �X�y�L�����[�}�b�v���t�F�b�`.
//    float4 spe = SpecularMap.Sample( SpecularSmp, input.TexCoord );
    {
        float3 N = normalize( input.Normal );
        float3 V = normalize( input.WorldPos.xyz - input.CameraPos );
        float3 L = normalize( input.LightDir );

//        float3 diffuse   = NormalizedLambert( Diffuse * mapKd.rgb, L, N );
 //       float3 specular  = NormalizedPhong( Specular * mapKs.rgb, Power, V, N, L );
    #if 1
//		output.Color.rgb = (diffuse.rgb + specular.rgb) * sdwThreshold;
		output.Color.rgb = 1;
		output.Color.rgb *= sdwThreshold;
    #else
        //// �f�o�b�O�p.
        //float4 colors[5] = {
        //    float4( 1.0f, 0.0f, 0.0f, 1.0f ),
        //    float4( 0.0f, 1.0f, 0.0f, 1.0f ),
        //    float4( 0.0f, 0.0f, 1.0f, 1.0f ),
        //    float4( 1.0f, 1.0f, 0.0f, 1.0f ),
        //    float4( 0.0f, 0.0f, 0.0f, 1.0f ),
        //};
        //float4 shadowColor = colors[ index ] * ( 1.0f - sdwThreshold );
        //output.Color.rgb = ( diffuse.rgb + specular.rgb ) + shadowColor.rgb;
    #endif
        output.Color.a   = 1;
    }

    return output;
}
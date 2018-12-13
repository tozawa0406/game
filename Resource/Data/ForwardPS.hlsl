// 円周率です.
#define PI          3.1415926535f

struct VSOutput
{
    float4  Position    : SV_POSITION;          //!< スクリーン空間の位置座標です.
    float4  WorldPos    : WORLD_POSIITON;       //!< ワールド空間の位置座標です.
    float3  Normal      : NORMAL;               //!< 法線ベクトルです.
    float2  TexCoord    : TEXCOORD0;            //!< テクスチャ座標です.
    float3  LightDir    : LIGHT_DIRECTION;      //!< ライトの方向ベクトルです.
    float3  CameraPos   : CAMERA_POSITION;      //!< カメラ位置です.
    float4  SplitPos    : SPLIT_POSITION;       //!< カスケードシャドウマップ用分割距離.
    float4  SdwCoord[4] : SHADOW_COORD;         //!< カスケードシャドウマップ用位置座標.
};

Texture2D       ShadowMap0    : register(t3);     //!< シャドウマップ(カスケード0)です.
Texture2D       ShadowMap1    : register(t4);     //!< シャドウマップ(カスケード1)です.
Texture2D       ShadowMap2    : register(t5);     //!< シャドウマップ(カスケード2)です.
Texture2D       ShadowMap3    : register(t6);     //!< シャドウマップ(カスケード3)です.

SamplerComparisonState  ShadowSmp     : register(s3);     //!< シャドウマップ用サンプラー比較ステートです.

struct PSOutput
{
    float4 Color : SV_TARGET0;          //!< ターゲット0の出力カラーです.
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

    // ディフューズマップをフェッチ.
//    float4 mapKd = DiffuseMap.Sample( DiffuseSmp, input.TexCoord );

    // アルファテスト.
//    clip( ( mapKd.a < 0.125f ) ? -1.0f : 1.0f );

    // スペキュラーマップをフェッチ.
//    float4 mapKs = SpecularMap.Sample( SpecularSmp, input.TexCoord );

    float sdwThreshold = 1.0f;      // シャドウにするかどうかの閾値です.
    float sdwBias      = 0.01f;     // シャドウバイアスです.
    float sdwColor     = 0.1f;      // シャドウのカラーです.

    // 各ピクセル位置までの距離.
    float dist = input.Position.w;  // ビュー空間でのZ座標.

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

    // スペキュラーマップをフェッチ.
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
        //// デバッグ用.
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
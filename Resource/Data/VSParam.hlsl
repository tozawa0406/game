struct IN_VS
{
	float4 position : POSITION0;
	float4 normal   : NORMAL0;
	float4 tangent	: TANGENT0;
	float4 color    : COLOR0;
	float2 texcoord : TEXCOORD0;
	float4 boneIndex: TEXCOORD1;
	float4 weight   : TEXCOORD2;
};

struct OUT_VS
{
	float4 position  : SV_POSITION;
	float2 texcoord  : TEXCOORD0;
	float3 eye		 : TEXCOORD1;
	float3 light	 : TEXCOORD2;
	float4 texelSize : TEXCOORD3;
	float4 color     : COLOR0;
	float4 worldPosition : WORLD_POSITION0;
	float4 worldNormal   : WORLD_POSITION1;
	float3 lightDir      : LIGHT_DIRECTION;
	float3 cameraPos     : CAMERA_POSITION;
	float4 splitPos      : SPLIT_POSITION0;
	float4 splitPosXMax  : SPLIT_POSITION1;
	float4 splitPosXMin  : SPLIT_POSITION2;
	float4 sdwcoord[4]   : SHADOW_COORD;
};

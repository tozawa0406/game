//-----------------------------------------------------------------------------
//
//	ポイントスプライトシェーダー[PointSprite.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _POINT_SPRITE_SHADER_H_
#define _POINT_SPRITE_SHADER_H_

#include "Shader.h"

class PointSpriteShader : public Shader
{
	static constexpr char* FILENAME = "PointSprite.fx";

	HRESULT Init(void) override;
public:	
	PointSpriteShader(ShaderManager* manager) : Shader(manager, shaderDirectoryName + FILENAME) {}
	HRESULT SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const override;

	struct CONSTANT
	{
		MATRIX  world;
		MATRIX  view;
		MATRIX  proj;
		VECTOR4 diffuse;
	};
};

#endif // _POINT_SPRITE_SHADER_H_
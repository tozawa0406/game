//-----------------------------------------------------------------------------
//
//	デフォルトシェーダー[Default.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _SKINNING_SHADOW_SHADER_H_
#define _SKINNING_SHADOW_SHADER_H_

#include "Shader.h"
#include "../Sprite/Texture.h"

class SkinningShadowShader : public Shader
{
	static constexpr char* FILENAME = "Default.fx";

	HRESULT Init(void) override;
public:
	SkinningShadowShader(ShaderManager* manager) : Shader(manager, shaderDirectoryName + FILENAME) {}
	HRESULT SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const override;

	struct CONSTANT
	{
		MATRIX  world;
		MATRIX  view;
		MATRIX  proj;
	};
};

#endif // _SKINNING_SHADOW_SHADER_H_
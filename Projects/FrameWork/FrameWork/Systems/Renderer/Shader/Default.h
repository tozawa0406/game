//-----------------------------------------------------------------------------
//
//	デフォルトシェーダー[Default.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _DEFAULT_SHADER_H_
#define _DEFAULT_SHADER_H_

#include "Shader.h"
#include "../Sprite/Texture.h"

class DefaultShader : public Shader
{
	static constexpr char* FILENAME = "Default.fx";

	HRESULT Init(void) override;
public:
	DefaultShader(ShaderManager* manager) : Shader(manager, shaderDirectoryName + FILENAME) {}
	HRESULT SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const override;

	struct CONSTANT
	{
		MATRIX  world;
		MATRIX  view;
		MATRIX  proj;

		VECTOR4 texcoord;
		VECTOR4 diffuse;
//		VECTOR4 eyePos;
		VECTOR4 lightPosition;			// 光源の位置(モデル座標)
		MATRIX  lightView;
		MATRIX  lightProj;
	};

	struct CONSTANT_BONE
	{
		MATRIX boneInv[512];
		MATRIX boneAnim[512];
	};
};

#endif // _DEFAULT_SHADER_H_
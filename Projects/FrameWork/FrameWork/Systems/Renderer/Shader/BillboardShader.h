//-----------------------------------------------------------------------------
//
//	ビルボードシェーダー[Billboard.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _BILLBOARD_SHADER_H_
#define _BILLBOARD_SHADER_H_

#include "Shader.h"

class BillboardShader : public Shader
{
	static constexpr char* FILENAME = "Billboard.fx";
public:
	BillboardShader(ShaderManager* manager) : Shader(manager, shaderDirectoryName + FILENAME) {}
	HRESULT SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const override;
};

#endif // _BILLBOARD_SHADER_H_
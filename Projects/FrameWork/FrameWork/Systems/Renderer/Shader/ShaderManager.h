//-----------------------------------------------------------------------------
//
//	シェーダー管理[ShaderManager.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include "../../../Define/Define.h"
#include "../../BaseManager.h"
#include "Shader.h"

class ShaderManager : public Interface
{
	friend Systems;
public:
	~ShaderManager(void);
	Shader* GetShader(Shader::ENUM shaderNum) { return shader_[(int)shaderNum]; }

private:
	ShaderManager(Systems* systems);
	HRESULT Init(void);

	Shader* shader_[(int)Shader::ENUM::MAX];
};

#endif // _SHADER_H_
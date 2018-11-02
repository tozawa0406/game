//-----------------------------------------------------------------------------
//
//	深度バッファシェーダー[DepthShadow.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _DEPTH_SHADOW_SHADER_H_
#define _DEPTH_SHADOW_SHADER_H_

#include "Shader.h"

class DepthShadow : public Shader
{
	static constexpr char* FILENAME = "DepthBufShadow.fx";
public:
	DepthShadow(ShaderManager* manager);
	~DepthShadow(void);

	HRESULT SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const override;

private:
	HRESULT Init(void);

	struct CONSTANT
	{
		MATRIX  world;
		MATRIX  view;
		MATRIX  proj;
		VECTOR4 texcoord;
		VECTOR4 color;
		MATRIX  lightView;
		MATRIX  lightProj;
	};
};

#endif // _DEPTH_SHADOW_SHADER_H_

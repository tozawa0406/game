//-----------------------------------------------------------------------------
//
//	Auther : ŒËàVãÄ‘¾
//-----------------------------------------------------------------------------
#ifndef _2D_CIRCLE_GAUGE_SHADER_H_
#define _2D_CIRCLE_GAUGE_SHADER_H_

#include "Shader.h"

class CircleGaugeShader : public Shader
{
	static constexpr char* FILENAME = "2DCircleGauge.hlsl";

	HRESULT Init(void) override;
public:	
	CircleGaugeShader(ShaderManager* manager) : Shader(manager, shaderDirectoryName + FILENAME) {}
	HRESULT SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const override;

	struct CONSTANT
	{
		VECTOR4 screen;
	};
};

#endif // _2D_CIRCLE_GAUGE_SHADER_H_
//-----------------------------------------------------------------------------
//
//	Zテクスチャシェーダー[ZTexture.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _ZTEXTURE_SHADER_H_
#define _ZTEXTURE_SHADER_H_

#include "Shader.h"
#include "../Sprite/Texture.h"

class ZTexture : public Shader
{
	static constexpr char* FILENAME = "ZValue.fx";
public:
	ZTexture(ShaderManager* manager);
	~ZTexture(void);

	HRESULT BeginDraw(void);
	HRESULT EndDraw(void);
	HRESULT SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const override;

	const MATRIX& GetLightView(void) const { return view_; }
	const MATRIX& GetLightProj(void) const { return proj_; }

private:
	HRESULT Init(void) override;

	MATRIX view_;
	MATRIX proj_;

	struct CONSTANT
	{
		MATRIX  world;
		MATRIX  view;
		MATRIX  proj;
		VECTOR4 texcoord;
	};
};

#endif // _ZTEXTURE_SHADER_H_

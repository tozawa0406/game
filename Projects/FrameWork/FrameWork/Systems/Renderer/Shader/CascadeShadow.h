//-----------------------------------------------------------------------------
//
//	Zテクスチャシェーダー[ZTexture.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _CASCADE_SHADOW_SHADER_H_
#define _CASCADE_SHADOW_SHADER_H_

#include "Shader.h"
#include "../Sprite/Texture.h"
#include "../../../Graphics/DirectX11/Dx11Utility/CascadeManager.h"

class CascadeShadow : public Shader
{
	static constexpr char* FILENAME = "ZValue.fx";
public:
	CascadeShadow(ShaderManager* manager);
	~CascadeShadow(void);

	HRESULT BeginDraw(int i);
	HRESULT EndDraw(void);
	HRESULT SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const override;

private:
	HRESULT Init(void) override;

	int drawNum_;
	CascadeManager* cascade_;

	struct CONSTANT
	{
		MATRIX world;
		MATRIX shadowViewProj;
	};
};

#endif // _ZTEXTURE_SHADER_H_

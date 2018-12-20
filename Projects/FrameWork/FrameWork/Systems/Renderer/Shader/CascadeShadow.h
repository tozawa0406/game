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
#include "../../../Graphics/Utility/CascadeManager.h"

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

	struct CONSTANT_DRAW
	{
		VECTOR4		texlSize;
		VECTOR4		cameraPos;
		VECTOR4		lightDirection_;
		float		splitPos[CascadeManager::MAX_CASCADE];
		float		splitPosXMax[CascadeManager::MAX_CASCADE];
		float		splitPosXMin[CascadeManager::MAX_CASCADE];
		MATRIX		shadow[CascadeManager::MAX_CASCADE];
	};
};

#endif // _ZTEXTURE_SHADER_H_

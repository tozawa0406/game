//-----------------------------------------------------------------------------
//
//	3D•`‰æ[ObjectRenderer.h]
//	Auther : ŒËàVãÄ‘¾
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _OBJECT_RENDERER_H_
#define _OBJECT_RENDERER_H_

#include "../../Define/Define.h"
#include "../../Graphics/Wrapper.h"
#include "../GameSystems.h"
#include "Sprite/Texture.h"
#include "Model/Model.h"
#include "Shader/ShaderManager.h"

struct ObjectRenderer
{
	enum RendererType
	{
		SPRITE = 0,
		MODEL,
	};

	friend class ObjectRendererManager;

	const Transform*	GetTransform(void)	const { return transform_; }
	Shader::ENUM		GetShader(void)		const { return shader;     }

	bool		fastDraw;
	bool		enable;
	bool		shadow;
	bool		sort;
	MATERIAL	material;
	Shader::ENUM shader;

	ObjectRenderer(RendererType t);
	virtual ~ObjectRenderer(void);
	virtual bool Animation(float add) = 0;

protected:
	void Init(Systems* s, const Transform* t);

	Systems*		 systems_;
	const Transform* transform_;

private:

	RendererType     type;
};

#endif // _CANVAS_RENDERER_H_
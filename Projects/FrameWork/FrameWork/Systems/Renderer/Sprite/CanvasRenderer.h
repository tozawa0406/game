//-----------------------------------------------------------------------------
//
//	2D•`‰æ[CanvasRenderer.h]
//	Auther : ŒËàVãÄ‘¾
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _CANVAS_RENDERER_H_
#define _CANVAS_RENDERER_H_

#include "../../../Define/Define.h"
#include "../../../Graphics/Wrapper.h"
#include "../../GameSystems.h"
#include "Texture.h"

struct CanvasRenderer
{
	friend class CanvasRendererManager;

	void Init(Systems* systems, byte priority, int texNum);
	uint GetBuffer(void)       const { return buffer_; }
	byte GetPrimitiveNum(void) const { return pnum_; }
	void SetPriority(byte p) { priority_ = p; systems_->GetCanvasRenderer()->Sort(); }
	Wrapper::PRIMITIVE::TYPE GetPrimitiveType(void) const { return type_; }

	int			  texNum;
	VECTOR2       position;
	VECTOR2       size;
	VECTOR2       scale;
	VECTOR2       scaleOffset;
	float         angle;
	VECTOR2       rotationOffset;
	COLOR         color;
	bool          enable;
	Shader::ENUM  shader;

	float   pattern;
	VECTOR2 split;

	CanvasRenderer(void);
	~CanvasRenderer(void);

	CanvasRenderer operator = (const CanvasRenderer& d)
	{
		texNum         = d.texNum;
		pattern        = d.pattern;
		split          = d.split;
		position       = d.position;
		size           = d.size;
		scale          = d.scale;
		scaleOffset    = d.scaleOffset;
		angle          = d.angle;
		rotationOffset = d.rotationOffset;
		color          = d.color;
		return *this;
	}
	void Animation(float add);

private:
	void Point(void);
	void Line(void);
	void Rectangle(void);
	void FillTriangle(void);
	void FillRectangle(void);
	void FillCircle(void);
	void Sprite(void);

	Wrapper::PRIMITIVE::TYPE type_;
	byte pnum_;
	byte priority_;
	uint buffer_;

	Systems* systems_;
};

#endif // _CANVAS_RENDERER_H_
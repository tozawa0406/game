//-----------------------------------------------------------------------------
//
//	ƒ‚ƒfƒ‹•`‰æ[MeshRenderer.h]
//	Auther : ŒËàVãÄ‘¾
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _MESH_RENDERER_H_
#define _MESH_RENDERER_H_

#include "../ObjectRenderer.h"
#include "Model.h"

struct MeshRenderer : public ObjectRenderer
{
	void Init(Systems* systems, int modelNum, const Transform* trans);
	int  GetModelNum(void) const { return modelNum_; }

	MeshRenderer(void);
	~MeshRenderer(void);

	bool Animation(float add) override;
	void Skinning(void);
	void ChangeModel(int modelNum);
	void ChangeAnimation(int animNum, int frame, bool end = false);
	void AnimEndPattern(void);

	inline int GetMaxAnimation(void)	const { return animationMax_[animation_]; }

	inline float GetPattern(void)		const { return pattern_; }
	inline float GetPatternOld(void)	const { return patternOld_; }
	inline int   GetAnimation(void)		const { return animation_; }
	inline int   GetAnimationOld(void)	const { return animationOld_; }
	inline float GetRate(void)			const { return rate_; }

private:
	int   modelNum_;
	float pattern_;
	float patternOld_;
	int   animation_;
	int   animationOld_;
	bool  end_;
	bool  isSkinning_;

	float rate_;
	float addRate_;
	std::vector<int>	animationMax_;
};

#endif // _MESH_RENDERER_H_

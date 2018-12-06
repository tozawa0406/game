/*
 * @file		BloodSplash.h
 * @brief		斬った時の血エフェクト
 * @author		戸澤翔太
 * @data		2018/11/22
 */
#ifndef _BLOOD_SPLASH_H_
#define _BLOOD_SPLASH_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>
#include <FrameWork/Systems/DebugSetting/GUI.h>

class BloodSplash : public Object
{
public:
	BloodSplash(VECTOR3 position, float rotation);
	~BloodSplash(void);

	void Init(void)   override;
	void Uninit(void) override;
	void Update(void) override;

private:
	//! 出現してからのフレーム数
	uint8			cnt_;
	//! レンダラ
	SpriteRenderer	renderer_;
};


#endif // _BLOOD_SPLASH_H_

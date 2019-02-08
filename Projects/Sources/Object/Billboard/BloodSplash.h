/*
 * @file		BloodSplash.h
 * @brief		斬った時の血エフェクト
 * @author		戸澤翔太
 * @data		2018/11/22
 */
#ifndef _BLOOD_SPLASH_H_
#define _BLOOD_SPLASH_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Systems/Particle/Particle.h>
#include <FrameWork/Systems/DebugSetting/GUI.h>

class BloodSplash : public Particle
{
public:
	/* @brief	コンストラクタ
	 * @param	(position)		初期位置
	 * @param	(rotation)		初期回転角		*/
	BloodSplash(VECTOR3 position, float rotation);
	/* @brief	デストラクタ		*/
	~BloodSplash(void);
};


#endif // _BLOOD_SPLASH_H_

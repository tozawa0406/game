/*
 * @file		BloodSplash.h
 * @brief		�a�������̌��G�t�F�N�g
 * @author		���V�đ�
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
	/* @brief	�R���X�g���N�^
	 * @param	(position)		�����ʒu
	 * @param	(rotation)		������]�p		*/
	BloodSplash(VECTOR3 position, float rotation);
	/* @brief	�f�X�g���N�^		*/
	~BloodSplash(void);
};


#endif // _BLOOD_SPLASH_H_

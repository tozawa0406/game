/*
 * @file		BloodSplash.h
 * @brief		�a�������̌��G�t�F�N�g
 * @author		���V�đ�
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
	/* @brief	�R���X�g���N�^
	 * @param	(position)		�����ʒu
	 * @param	(rotation)		������]�p		*/
	BloodSplash(VECTOR3 position, float rotation);
	/* @brief	�f�X�g���N�^		*/
	~BloodSplash(void);


	
	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Init(void)   override;
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Uninit(void) override;
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Update(void) override;

private:
	//! �o�����Ă���̃t���[����
	uint8			cnt_;
	//! �����_��
	SpriteRenderer	renderer_;
};


#endif // _BLOOD_SPLASH_H_

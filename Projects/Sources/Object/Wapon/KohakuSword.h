/*
 * @file		KohakuSword.h
 * @brief		��
 * @author		���V�đ�
 * @data		2018/10/30
 */
#ifndef _KOHAKU_SWORD_H_
#define _KOHAKU_SWORD_H_

#include "Wapon.h"

class KohakuSword : public Wapon, public GUI
{
public:
	/* @brief	�R���X�g���N�^		*/
	KohakuSword(void);
	/* @brief	�f�X�g���N�^		*/
	~KohakuSword(void);



	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�Ȃ�	�@			*/
	void Init(void)   override;
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Uninit(void) override;
	
	/* @brief	�X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Update(void) override;
	
	/* @brief	Gui�̍X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void GuiUpdate(void) override;

private:
	/* @brief	�[������
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void PaidSword(void)  override;

	/* @brief	��������
	�@* @param	�Ȃ�
	�@* @return	�Ȃ�				*/
	void DrawnSword(void) override;

	bool debug_hit_;		//! �f�o�b�O�p
};

#endif // _KOHAKU_SWORD_H_

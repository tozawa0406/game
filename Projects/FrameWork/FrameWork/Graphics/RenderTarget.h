/*
 * @file		RenderTarget.h
 * @brief		�O���t�B�b�N�XAPI���̃����_�[�^�[�Q�b�g�̏���
 * @author		���V�đ�
 * @data		2018/12/21
 */

#ifndef _RENDER_TARGET_H_
#define _RENDER_TARGET_H_

#include "../Define/Define.h"

class RenderTarget
{
public:
	/* @enum	�����_�[�^�[�Q�b�g�̎��	*/
	enum class List : uint8
	{
		DEFAULT = 0,
		SCREEN_SHOT,
		COLOR,
		POSITION,
		NORMAL,
		MAX
	};

	//! @def	�`��̑傫����
	static constexpr float SIZE_X = 240;
	//! @def	�`��̑傫���c
	static constexpr float SIZE_Y = 135;

	/* @brief	�R���X�g���N�^		*/
	RenderTarget(void) : debugDraw_(List::MAX) {}
	/* @brief	�f�X�g���N�^		*/
	virtual ~RenderTarget(void) {}

	/* @brief	����������			*/
	virtual HRESULT Init(void)		= 0;
	/* @brief	�㏈��				*/
	virtual void	Uninit(void)	= 0;

	/* @brief	�����_�[�^�[�Q�b�g�̃N���A����			*/
	virtual void ClearRendererTarget(List num, COLOR color) = 0;

	/* @brief	�e�̕`��J�n		*/
	virtual void BeginDrawShadow(int i) = 0;
	/* @brief	�e�̕`��I��		*/
	virtual void EndDrawShadow(void) = 0;

	/* @brief	�}���`�����_�[�^�[�Q�b�g�̕`��J�n		*/
	virtual void BeginMultiRendererTarget(void) = 0;
	/* @brief	�}���`�����_�[�^�[�Q�b�g�̕`��I��		*/
	virtual void EndMultiRendererTarget(void)	= 0;

	/* @brief	Gui�̍X�V����		*/
	virtual void GuiUpdate(void) {}

	/* @brief	�f�o�b�O�p�`�揈��	*/
	virtual void Draw(List num, VECTOR2 position, VECTOR2 size) = 0;
	/* @brief	�f�o�b�O�p�`�揈��	*/
	virtual void DrawShadowMap(void) = 0;

	/* @brief	�X�N���[���V���b�g�̐���			*/
	virtual void CreateScreenshot(const string& filename) = 0;

	/* @brief	�f�o�b�O�p�ɕ`�悷�郌���_�[�^�[�Q�b�g�̐ݒ�
	 * @param	(num)	�`�悵���������_�[�^�[�Q�b�g�̎��
	 * @return	�Ȃ�				*/
	inline void SetDebugDraw(List num) { debugDraw_ = num; }

protected:
	//! �f�o�b�O�ŉ�ʂɕ`�悷�郌���_�[�^�[�Q�b�g�̎��
	List debugDraw_;
};

#endif // _RENDER_TARGET_H_
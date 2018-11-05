/*
 * @file		GuiManager.h
 * @brief		GUI�Ǘ�
 * @author		���V�đ�
 * @data		2018/11/05
 */
#ifndef _GUI_MANAGER_H_
#define _GUI_MANAGER_H_

#include "../../Define/Define.h"
#include "../BaseManager.h"
#include "../../Windows/Windows.h"
#include "ImGUI/imgui.h"
#include "GUI.h"

class Systems;
class Debug;
class GuiManager : public BaseManager<GUI>
{
public:
	static constexpr int FLASHING   = 10;
	static constexpr int SELECT_GUI = 30;

	GuiManager(void);
	~GuiManager(void);
	HRESULT Init(void)   override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	Draw(void)   override;

	void	GuiUpdate(void);

	const VECTOR3* GetSelect(void);

	// @brief	Init�̑O�ɌĂяo��
	inline void SetDebug(Debug* debug) { debug_ = debug; }

	// Getter
	inline Debug*	GetDebug(void)		{ return debug_;		}
	inline bool		GetLookObject(void) { return lookObject_;	}

private:
	//! �e�ւ̃|�C���^
	Debug* debug_;
	friend GUI;

	//! Object�Q��Gui���X�g�\��
	bool guiObject_;
	//! Subject�Q��Gui���X�g�\��
	bool guiSubject_;
	//! Gui��I�������I�u�W�F�N�g�𒍎�����
	bool lookObject_;
	//! �A�b�v�f�[�g��ɕ`�悵�����ǂ���
	bool draw_;
	//! ���ݕ\�����Ă���Gui�̐�
	int drawNum_;
	//! ���o�Ŏg���t���[���̃J�E���^
	int frame_;
};

#endif // _GUI_MANAGER_H_

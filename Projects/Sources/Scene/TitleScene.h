//-----------------------------------------------------------------------------
//
//	�^�C�g��[TitleScene.h]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _TITLE_SCENE_H_
#define _TITLE_SCENE_H_

#include <FrameWork/Scene/BaseScene.h>

//-----------------------------------------------------------------------------
//	�N���X�錾
//-----------------------------------------------------------------------------
class TitleScene : public BaseScene, public GUI
{
	//! @def	�R���g���[���̃^�C�v�ɂ��z��ԍ�
	enum class InputType : uint8
	{
		Keyboard = 0,
		PS4,
		X,

		MAX
	};
public:
	TitleScene(SceneManager* manager);
	~TitleScene(void);

	void Init(void)   override;
	void Uninit(void) override;

	void GuiUpdate(void) override;

private:
	SceneList	Update(void) override;

	void		Flashing(Controller& ctrl);
	int 		JudgeCtrlType(Controller& ctrl);
	SceneList	EndScene(Controller& ctrl);

	//! �t���[���̃J�E���^
	int				frameCnt_;
	//! �uplease press�v
	CanvasRenderer	press_;
	//! �uEnter�v�u�Z�v�uX�v
	CanvasRenderer	button_[static_cast<uint8>(InputType::MAX)];
};

#endif // _TITLE_SCENE_H_

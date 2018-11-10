//-----------------------------------------------------------------------------
//
//	�|�[�Y[PauseScene.h]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _PAUSE_SCENE_H_
#define _PAUSE_SCENE_H_

#include <FrameWork/Scene/BaseScene.h>

class Pause : public BaseScene, public GUI
{
	// �萔��`
	static constexpr int TITLE_CONTROLLER  = 20000;
	static constexpr int PAUSE_SELECT_UP   = 0;
	static constexpr int PAUSE_SELECT_DOWN = 2;
	static constexpr int PADDING_TIME      = 30;

public:
	static constexpr float PAUSE_W = (float)Graphics::WIDTH  / 3;		//��
	static constexpr float PAUSE_H = (float)Graphics::HEIGHT / 7;		//����

	Pause(SceneManager* manager);
	~Pause(void);

	void Init(void)   override;
	void Uninit(void) override;
	SceneList Update(void) override;		//�X�V����

private:
	void EnableUI(bool enable);

	VECTOR2 pos_[3];		//�I���I�u�W�F�N�g�̈ʒu���W
	int		selectNum_;
	int     paddingTimeCnt_;

	CanvasRenderer back_;
	CanvasRenderer menuBack_[3];
	CanvasRenderer notSelect_[2];
	CanvasRenderer menu_[3];
};

#endif // _PAUSE_SCENE_H_

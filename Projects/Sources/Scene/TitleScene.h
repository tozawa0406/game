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
public:
	//�萔��`
	static constexpr int FLASH_RANGE = 30;		// �_�ł̊Ԋu

	TitleScene(SceneManager* manager);	// �R���X�g���N�^
	~TitleScene(void);					// �f�X�g���N�^

	void Init(void)   override;
	void Uninit(void) override;

private:
	SceneList Update(void) override;					// �X�V����(�Ԃ�l�͑I��ԍ�)

	CanvasRenderer press_;
};

#endif // _TITLE_SCENE_H_

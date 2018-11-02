//-----------------------------------------------------------------------------
//
//	�^�C�g��[TitleScene.h]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _TITLE_SCENE_H_
#define _TITLE_SCENE_H_

#include "../BaseScene.h"
#include "../../Systems/Renderer/Sprite/CanvasRenderer.h"
#include "../../Systems/DebugSetting/GUI.h"
#include "../../Systems/GameSystems.h"

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

private:
	int  Update(void) override;					// �X�V����(�Ԃ�l�͑I��ԍ�)
};

#endif // _TITLE_SCENE_H_

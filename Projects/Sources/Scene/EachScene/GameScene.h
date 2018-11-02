//-----------------------------------------------------------------------------
//
//	�Q�[��[GameScene.h]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Scene/BaseScene.h>
#include <FrameWork/Systems/GameSystems.h>
#include <FrameWork/Systems/DebugSetting/Debug.h>

#include <FrameWork/Systems/Renderer/SkyDome.h>
#include <FrameWork/Systems/Renderer/MeshField.h>

#include <FrameWork/Systems/Renderer/Model/MeshRenderer.h>
#include <FrameWork/Systems/Renderer/Sprite/SpriteRenderer.h>

#include <FrameWork/Object/ObjectManager.h>

//-----------------------------------------------------------------------------
//	�N���X�錾
//-----------------------------------------------------------------------------
class Light;
class GameScene : public BaseScene, public GUI
{
	// �}�N����`
	static constexpr int CREATE_ENEMY_RANGE = 30;
	static constexpr int GET_POINT = 100;

public:
	static constexpr int BILLBOARD_NUM = 100;

	GameScene(SceneManager* manager);		// �R���X�g���N�^
	~GameScene(void);						// �f�X�g���N�^
	MeshField*		GetMeshField(void)		{ return meshField_; }
	Light*			GetLight(void)			{ return light_;     }

private:
	int  Update(void) override;		// �X�V����

	int  frameCnt = 0;		//�t���[���̃J�E���^

	int  ResbornCnt;

	ObjectManager*	objectManager_;

	SkyDome*		sky_;
	MeshField*		meshField_;

	Light*			light_;
};

#endif // _GAME_SCENE_H_

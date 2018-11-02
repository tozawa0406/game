//-----------------------------------------------------------------------------
//
//	�V�[�����[BaseScene.h]
//	Auther : ���V�đ�
//																	2018/02/27
//-----------------------------------------------------------------------------
#ifndef _BASE_SCENE_H_
#define _BASE_SCENE_H_

#include "../Define/Define.h"

#include "../Systems/Renderer/Sprite/CanvasRenderer.h"
#include "../Systems/Input/Controller.h"

#include "../Systems/DebugSetting/Debug.h"

//-----------------------------------------------------------------------------
//	�N���X�錾
//-----------------------------------------------------------------------------
class Scene
{
public:
	enum class Num			// �V�[����
	{
		TITLE = 0,		// �^�C�g��
		GAME,			// �Q�[��
		RESULT,			// ���U���g
		MAX
	};
};

class SceneManager;
class BaseScene
{
public:
	BaseScene(SceneManager* manager);
	virtual ~BaseScene(void) {};					// �f�X�g���N�^

	virtual int				Update(void)	 { return 0; }			// �X�V����
	virtual SceneManager*	GetManager(void) { return manager_; }	// �e�̎擾

	Controller* GetCtrl(int i);						// �R���g���[���̎擾
	Systems*    GetSystems(void) { return systems_; }

protected:
	SceneManager* manager_;		// �e
	Systems*      systems_;
};

#endif // _BASE_SCENE_H_

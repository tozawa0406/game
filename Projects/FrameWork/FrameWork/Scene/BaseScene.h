//-----------------------------------------------------------------------------
//
//	�V�[�����[BaseScene.h]
//	Auther : ���V�đ�
//																	2018/02/27
//-----------------------------------------------------------------------------
#ifndef _BASE_SCENE_H_
#define _BASE_SCENE_H_

#include "../Define/Define.h"
#include "../../../Sources/Scene/EachScene.h"

#include "../Systems/Renderer/CanvasRenderer/CanvasRendererImage.h"
#include "../Systems/Input/Controller.h"

#include "../Systems/GameSystems.h"
#include "../Systems/DebugSetting/Debug.h"

//-----------------------------------------------------------------------------
//	�N���X�錾
//-----------------------------------------------------------------------------
class SceneManager;
class BaseScene
{
public:
	BaseScene(SceneManager* manager);
	virtual ~BaseScene(void) {};					// �f�X�g���N�^

	virtual void Init(void)   = 0;
	virtual void Uninit(void) = 0;

	virtual SceneList		Update(void)	 { return SceneList::NOTCHANGE; }			// �X�V����
	virtual SceneManager*	GetManager(void) { return manager_; }	// �e�̎擾

	Controller* GetCtrl(int i);						// �R���g���[���̎擾
	Systems*    GetSystems(void) { return systems_; }

protected:
	SceneManager* manager_;		// �e
	Systems*      systems_;
};

#endif // _BASE_SCENE_H_

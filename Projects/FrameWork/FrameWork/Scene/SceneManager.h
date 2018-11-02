//-----------------------------------------------------------------------------
//
//	�V�[������[SceneManager.h]
//	Auther : ���V�đ�
//																	2018/02/27
//-----------------------------------------------------------------------------
#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "../Define/Define.h"

#include "BaseScene.h"
#include "../Systems/Renderer/Sprite/CanvasRenderer.h"
#include "../Systems/Input/Controller.h"

#include "../Systems/DebugSetting/Debug.h"

#include "../Systems/Loading.h"

//-----------------------------------------------------------------------------
//	�N���X�錾
//-----------------------------------------------------------------------------
class CameraManager;
class SceneManager : public Interface
{
	static constexpr int CAMERA_DEBUG = 1;

	static constexpr int SCENE_FADE_IN  = 30;				//�t�F�[�h�̎���(�t���[���P��)
	static constexpr int SCENE_FADE_OUT = 30;

	static constexpr int CHANGE_NORMAL = 0x000000;

	static constexpr int CHANGE_CHECK  = 0x111111;
	static constexpr int CHANGE_TITLE  = 0x000010;

public:
	static constexpr int CHANGE_SCENE  = 0x000001;
	static constexpr int CHANGE_GAME   = 0x000100;

	SceneManager(Systems* systems);				// �R���X�g���N�^
	~SceneManager(void);						// �f�X�g���N�^
	HRESULT Init(void)   override { return S_OK; }
	void    Update(void) override;				// �X�V����
	void    Draw(void)   override;				// �`�揈��
	void    Change(int scene = CHANGE_SCENE);	// �J�ڏ�������

	// Getter
	Scene::Num		GetSceneNum(void)      { return displayMode_;	}		// ���݃V�[���̔ԍ�
	BaseScene*		GetScene(void)         { return scene_;			}		// ���݂̃V�[���̃|�C���^
	BaseScene*		GetPause(void)         { return pause_;			}		// �|�[�Y�V�[���ւ̃|�C���^
	CameraManager*	GetCameraManager(void) { return camera_;		}		// �J�����}�l�[�W���[�ւ̃|�C���^
	bool			isPause(void)          { return isPause_;		}		// �|�[�Y��ԃt���O
	void			SetPause(bool pause)   { isPause_ = pause;		}		// �|�[�Y��ԃt���O

private:
	BaseScene* scene_;		// �V�[��
	BaseScene* pause_;			// �|�[�Y

	void SceneUpdate(void);			// �V�[���̍X�V����
	void Fade(void);				// �t�F�[�h����
	void ChangeActual(void);		// �V�[���J�ڏ���
	void ForceSceneChange(void);	// �����V�[���J��(�f�o�b�O�p)
	BaseScene* CreateScene(void);	// �V�[����new����

	int			sceneChange_;		// �V�[���J��
	Scene::Num	displayMode_;		// ��ʂ̏��
	int			fadeCnt_;			// �t�F�[�h�i�s
	bool		isPause_;			// �|�[�Y�̏��

	CanvasRenderer fade_;

	Loading*		loading_;
	bool			startLoad_;
	CameraManager* camera_;		// �J�����}�l�[�W���[
};

#endif // _SCENE_MANAGER_H_

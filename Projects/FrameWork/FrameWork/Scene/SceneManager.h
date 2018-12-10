//-----------------------------------------------------------------------------
//
//	�V�[������[SceneManager.h]
//	Auther : ���V�đ�
//																	2018/02/27
//-----------------------------------------------------------------------------
#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "../Define/Define.h"

#include <Scene/EachScene.h>
#include "BaseScene.h"
#include "../Systems/Renderer/CanvasRenderer/CanvasRendererImage.h"
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

public:

	SceneManager(Systems* systems);				// �R���X�g���N�^
	~SceneManager(void);						// �f�X�g���N�^
	HRESULT Init(void)   override;
	void    Uninit(void) override;
	void    Update(void) override;				// �X�V����
	void    Draw(void)   override;				// �`�揈��
	void    Change(SceneList scene = SceneList::NEXT);	// �J�ڏ�������

	// Getter
	SceneList		GetSceneNum(void)      { return displayMode_;	}		// ���݃V�[���̔ԍ�
	BaseScene*		GetScene(void)         { return scene_;			}		// ���݂̃V�[���̃|�C���^
	BaseScene*		GetPause(void)         { return pause_;			}		// �|�[�Y�V�[���ւ̃|�C���^
	CameraManager*	GetCameraManager(void) { return camera_;		}		// �J�����}�l�[�W���[�ւ̃|�C���^
	bool			isPause(void)          { return isPause_;		}		// �|�[�Y��ԃt���O
	void			SetPause(bool pause)   { isPause_ = pause;		}		// �|�[�Y��ԃt���O

private:
	BaseScene* scene_;		// �V�[��
	BaseScene* pause_;		// �|�[�Y
	EachScene* eachScene_;	// �V�[���̏����Ǘ�

	void SceneUpdate(void);			// �V�[���̍X�V����
	void Fade(void);				// �t�F�[�h����
	void ChangeActual(void);		// �V�[���J�ڏ���
	void ForceSceneChange(void);	// �����V�[���J��(�f�o�b�O�p)

	SceneList	sceneChange_;		// �V�[���J��
	SceneList	displayMode_;		// ��ʂ̏��
	int			fadeCnt_;			// �t�F�[�h�i�s
	bool		isPause_;			// �|�[�Y�̏��

	CanvasRenderer::Image fade_;

	Loading*		loading_;
	bool			startLoad_;
	CameraManager* camera_;		// �J�����}�l�[�W���[
};

#endif // _SCENE_MANAGER_H_

/*
 * @file		SceneManager.h
 * @brief		�V�[���Ǘ��N���X
 * @author		���V�đ�
 * @data		2018/02/27
 */
#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "../Define/Define.h"

#include <Scene/EachScene.h>
#include "BaseScene.h"
#include "../Systems/Renderer/CanvasRenderer/CanvasRendererImage.h"
#include "../Systems/Input/Controller.h"

#include "../Systems/DebugSetting/Debug.h"

#include "../Systems/Loading.h"

class CameraManager;
class SceneManager : public Interface
{
	//! @def	�J�����f�o�b�O
	static constexpr int CAMERA_DEBUG = 1;

	//! @def	�t�F�[�h�C���̎���(�t���[���P��)
	static constexpr int SCENE_FADE_IN  = 30;
	//! @def	�t�F�[�h�A�E�g�̎���(�t���[���P��)
	static constexpr int SCENE_FADE_OUT = 30;

public:
	/* @brief	�R���X�g���N�^
	 * @param	(systems)	�V�X�e���n		*/
	SceneManager(Systems* systems);
	/* @brief	�f�X�g���N�^				*/
	~SceneManager(void);



	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�������s					*/
	HRESULT Init(void)   override;

	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�						*/
	void    Uninit(void) override;

	/* @brief	�X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�						*/
	void    Update(void) override;

	/* @brief	�`�揈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�						*/
	void    Draw(void)   override;

	/* @brief	�J�ڏ�������
	 * @param	(scene)		�ړ��������V�[��
	 * @return	�Ȃ�						*/
	void    Change(SceneList scene = SceneList::NEXT);

	/* @brief	�V�[�����Ǘ��N���X�̎擾	*/
	inline EachScene*		GetEachSceneInfo(void) { return eachScene_;		}
	/* @brief	���݃V�[���̔ԍ��擾		*/
	inline SceneList		GetSceneNum(void)      { return displayMode_;	}
	/* @brief	���݂̃V�[���̃|�C���^�擾	*/
	inline BaseScene*		GetScene(void)         { return scene_;			}
	/* @brief	�|�[�Y�V�[���̃|�C���^�擾	*/
	inline BaseScene*		GetPause(void)         { return pause_;			}
	/* @brief	�J�����Ǘ��̃|�C���^�擾	*/
	inline CameraManager*	GetCameraManager(void) { return camera_;		}
	/* @brief	�|�[�Y��Ԏ擾				*/
	inline bool				IsPause(void)          { return isPause_;		}
	/* @brief	�|�[�Y��Ԑݒ�				*/
	inline void				SetPause(bool pause)   { isPause_ = pause;		}

private:
	BaseScene* scene_;		//! �V�[��
	BaseScene* pause_;		//! �|�[�Y
	EachScene* eachScene_;	//! �V�[���̏����Ǘ�

	/* @brief	�V�[���̍X�V
	 * @param	�Ȃ�
	 * @return	�Ȃ�						*/
	void SceneUpdate(void);
	
	/* @brief	�t�F�[�h����
	 * @param	�Ȃ�
	 * @return	�Ȃ�						*/
	void Fade(void);

	/* @brief	�V�[���J�ڏ���
	 * @param	�Ȃ�
	 * @return	�Ȃ�						*/
	void ChangeActual(void);
	
	/* @brief	�����V�[���J��(�f�o�b�O�p)
	 * @param	�Ȃ�
	 * @return	�Ȃ�						*/
	void ForceSceneChange(void);

	SceneList	sceneChange_;		//! �V�[���J��
	SceneList	displayMode_;		//! ��ʂ̏��
	int			fadeCnt_;			//! �t�F�[�h�i�s
	bool		isPause_;			//! �|�[�Y�̏��

	CanvasRenderer::Image fade_;	//! �t�F�[�h�̍�

	Loading*		loading_;		//! ���[�h�N���X
	bool			startLoad_;		//! ���[�h�t���O
	CameraManager*	camera_;		//1 �J�����}�l�[�W���[
};

#endif // _SCENE_MANAGER_H_

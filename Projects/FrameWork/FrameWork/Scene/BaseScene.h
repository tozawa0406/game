/*
 * @file		BaseScene.h
 * @brief		�V�[���̊��N���X
 * @author		���V�đ�
 * @data		2018/02/27
 */
#ifndef _BASE_SCENE_H_
#define _BASE_SCENE_H_

#include "../Define/Define.h"
#include "../../../Sources/Scene/00_EachScene.h"

#include "../Windows/Windows.h"
#include "../Systems/Renderer/CanvasRenderer/CanvasRendererImage.h"
#include "../Systems/Renderer/CanvasRenderer/CanvasRendererText.h"
#include "../Systems/Input/Controller.h"

#include "../Systems/GameSystems.h"
#include "../Systems/DebugSetting/Debug.h"

class SceneManager;
class Light;
class MeshField;
class BaseScene
{
public:
	/* @brief	�R���X�g���N�^		*/
	BaseScene(void);
	/* @brief	�f�X�g���N�^		*/
	virtual ~BaseScene(void);

	/* @brief	����������			*/
	virtual void Init(SceneList sceneNum) { sceneNum_ = sceneNum; };
	/* @brief	�㏈��				*/
	virtual void Uninit(void) = 0;

	/* @brief	�X�V����			*/
	virtual SceneList		Update(void)	 { return SceneList::NOTCHANGE; }

	/* @brief	�}�l�[�W���[�̎擾����
	 * @param	�Ȃ�
	 * @return	�}�l�[�W���[�̃|�C���^		*/
	inline  SceneManager*	GetManager(void) { return manager_; }

	/* @brief	�}�l�[�W���[�̐ݒ菈��
	 * @param	(manager)	�}�l�[�W���[�̃|�C���^
	 * @return	�Ȃ�				*/
	void SetManager(SceneManager* manager);

	/* @brief	���C�g�擾����		*/
	inline Light*		GetLight(void)		{ return light_;		}

	/* @brief	���b�V���t�B�[���h�擾����		*/
	inline MeshField*	GetMeshField(void)	{ return meshField_;	}

	/* @brief	�V�[���ԍ��̎擾	*/
	inline SceneList GetSceneNum(void) { return sceneNum_; }

protected:	
	/* @brief	�R���g���[���̎擾����
	 * @param	(i)		�R���g���[���̔ԍ�
	 * @return	�R���g���[���̃|�C���^		*/
	Controller* GetCtrl(int i);

	/* @brief	�T�E���h�擾����
	 * @param	�Ȃ�
	 * @return	�T�E���h�̃|�C���^			*/
	Sound*		GetSound(void);

	//! �}�l�[�W���[
	SceneManager*	manager_;
	//! �V�X�e���n
	Systems*		systems_;
	//! ���C�g
	Light*			light_;
	//! ���b�V���t�B�[���h
	MeshField*		meshField_;
	//! �����̃V�[���ԍ�
	SceneList		sceneNum_;
};

#endif // _BASE_SCENE_H_

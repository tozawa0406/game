/*
 * @file		BaseScene.h
 * @brief		�V�[���̊��N���X
 * @author		���V�đ�
 * @data		2018/02/27
 */
#ifndef _BASE_SCENE_H_
#define _BASE_SCENE_H_

#include "../Define/Define.h"
#include "../../../Sources/Scene/EachScene.h"

#include "../Windows/Windows.h"
#include "../Systems/Renderer/CanvasRenderer/CanvasRendererImage.h"
#include "../Systems/Renderer/CanvasRenderer/CanvasRendererText.h"
#include "../Systems/Input/Controller.h"

#include "../Systems/GameSystems.h"
#include "../Systems/DebugSetting/Debug.h"

class SceneManager;
class BaseScene
{
public:
	/* @brief	�R���X�g���N�^		*/
	BaseScene(void);
	/* @brief	�f�X�g���N�^		*/
	virtual ~BaseScene(void);

	/* @brief	����������			*/
	virtual void Init(void)   = 0;
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
	SceneManager* manager_;
	//! �V�X�e���n
	Systems*      systems_;
};

#endif // _BASE_SCENE_H_

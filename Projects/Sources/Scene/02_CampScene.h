/*
 * @file		CampScene.h
 * @brief		�Q�[���V�[��
 * @author		���V�đ�
 * @data		2018/08/18
 */
#ifndef _CAMP_SCENE_H_
#define _CAMP_SCENE_H_

#include <FrameWork/Scene/BaseScene.h>

#include <FrameWork/Systems/Renderer/SkyDome.h>
#include <FrameWork/Systems/Renderer/MeshField.h>

#include <FrameWork/Systems/Renderer/Model/MeshRenderer.h>
#include <FrameWork/Systems/Renderer/Sprite/SpriteRenderer.h>

#include <FrameWork/Object/ObjectManager.h>
#include "../Object/StaticObject/Goal.h"

#include "../Object/GameObject/AttackManager.h"
#include "../Object/UI/Tutorial/TutorialManager.h"

class CampScene : public BaseScene, public GUI
{
public:
	/* @brief	�R���X�g���N�^		*/
	CampScene(void);
	/* @brief	�f�X�g���N�^		*/
	~CampScene(void);


	
	/* @brief	����������
	 * @param	(sceneNum)	�V�[���ԍ�
	 * @return	�Ȃ�				*/
	void Init(SceneList sceneNum) override;
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�					*/
	void Uninit(void) override;
	
	/* @brief	�X�V����
	 * @param	�Ȃ�
	 * @return	�V�[���J�ڔԍ�			*/
	SceneList Update(void) override;

private:
	/* @brief	�t�B�[���h�̐���
	 * @sa		Init()
	 * @param	�Ȃ�
	 * @return	�Ȃ�					*/
	void CreateField(void);

	//! �I�u�W�F�N�g�̃}�l�[�W���[
	ObjectManager*	objectManager_;
	//! �J�ڈʒu
	Goal*			goal_;

	//! �V��
	SkyDome*		sky_;
	//! �U���Ǘ��N���X
	AttackManager*	attackManager_;
	//! �`���[�g���A��
	TutorialManager*	tutorialManager_;
};

#endif // _CAMP_SCENE_H_

/*
 * @file		GameScene.h
 * @brief		�Q�[���V�[��
 * @author		���V�đ�
 * @data		2018/08/18
 */
#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <FrameWork/Scene/BaseScene.h>

#include <FrameWork/Systems/Renderer/SkyDome.h>
#include <FrameWork/Systems/Renderer/MeshField.h>

#include <FrameWork/Systems/Renderer/Model/MeshRenderer.h>
#include <FrameWork/Systems/Renderer/Sprite/SpriteRenderer.h>

#include <FrameWork/Object/ObjectManager.h>

class Light;
class GameScene : public BaseScene, public GUI
{
public:
	/* @brief	�R���X�g���N�^		*/
	GameScene(void);
	/* @brief	�f�X�g���N�^		*/
	~GameScene(void);



	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Init(void)   override;
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�					*/
	void Uninit(void) override;
	
	/* @brief	�X�V����
	 * @param	�Ȃ�
	 * @return	�V�[���J�ڔԍ�			*/
	SceneList Update(void) override;

	/* @brief	�t�B�[���h�擾�֐�
	 * @return	�t�B�[���h�̃|�C���^		*/
	MeshField*		GetMeshField(void)		{ return meshField_; }
	/* @brief	���C�g�擾����
	 * @return	���C�g�̃|�C���^			*/
	Light*			GetLight(void)			{ return light_;     }

private:
	//! �I�u�W�F�N�g�̃}�l�[�W���[
	ObjectManager*	objectManager_;

	//! �V��
	SkyDome*		sky_;
	//! �t�B�[���h
	MeshField*		meshField_;
	//! ���C�g
	Light*			light_;
};

#endif // _GAME_SCENE_H_

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

//-----------------------------------------------------------------------------
//	�N���X�錾
//-----------------------------------------------------------------------------
class Light;
class GameScene : public BaseScene, public GUI
{
public:
	GameScene(SceneManager* manager);
	~GameScene(void);

	void Init(void)   override;
	void Uninit(void) override;

	/* @brief	�t�B�[���h�擾�֐�
	 * @return	�t�B�[���h�̃|�C���^		*/
	MeshField*		GetMeshField(void)		{ return meshField_; }
	/* @brief	���C�g�擾����
	 * @return	���C�g�̃|�C���^			*/
	Light*			GetLight(void)			{ return light_;     }

private:
	SceneList Update(void) override;

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

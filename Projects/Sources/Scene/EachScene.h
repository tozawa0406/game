/*
 * @file		EachScene.h
 * @brief		�V�[�����Ǘ��N���X
 * @author		���V�đ�
 * @data		2018/08/18
 */
#ifndef _EACH_SCENE_H_
#define _EACH_SCENE_H_

#include <FrameWork/Define/Define.h>

//! @enum	�V�[���J�ڃt���O
enum class SceneList : uint8
{
	NEXT		= 255,	// ���̃V�[��
	NOTCHANGE	= 0,	// �J�ڂȂ�
	TITLE,				// �^�C�g��
	GAME,				// �Q�[��
	RESULT,				// ���U���g
	MAX
};

class SceneManager;
class BaseScene;
class EachScene
{
public:
	/* @brief	�R���X�g���N�^		*/
	EachScene(void);
	/* @brief	�f�X�g���N�^		*/
	~EachScene(void);



	/* @brief	�V�[���Ǘ��̕ϐ��̒l��ύX����
	 * @param	(sceneChange)	�V�[���J�ڃt���O
	 * @param	(sceneNum)		�V�[���Ǘ��ԍ�
	 * @return	�Ȃ�					*/
	void		ChangeScene(SceneList sceneChange, SceneList& sceneNum);

		/* @brief	�|�[�Y�I�����̃R�}���h
	 * @param	(returnPause)		�|�[�Y�̕Ԃ�l
	 * @return	���̃V�[���Ɉڂ�ꍇ��0�ȊO				*/
	int 		ChangePause(int returnPause);

	/* @brief	�V�[����new����
	 * @param	(sceneNum)		�V�[���Ǘ��ԍ�
	 * @return	�V�[���ԍ��ɉ������V�[����new����		*/
	BaseScene*  CreateScene(SceneManager* manager, SceneList sceneNum);
	
	/* @fn		CreatePause
	 * @brief	�|�[�Y�V�[����new����
	 * @param	(sceneNum)		�V�[���Ǘ��ԍ�
	 * @return	�|�[�Y��new�����A�g��Ȃ��ꍇ��nullptr		*/
	BaseScene*	CreatePause(SceneManager* manager, SceneList sceneNum);
};

#endif // _EACH_SCENE

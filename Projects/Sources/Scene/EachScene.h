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
	EachScene(void);
	~EachScene(void);

	void		ChangeScene(SceneList sceneChange, SceneList& sceneNum);
	int 		ChangePause(int returnPause);
	BaseScene*  CreateScene(SceneManager* manager, SceneList sceneNum);
	BaseScene*	CreatePause(SceneManager* manager, SceneList sceneNum);
};

#endif // _EACH_SCENE

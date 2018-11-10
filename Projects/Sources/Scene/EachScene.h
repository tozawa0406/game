//-----------------------------------------------------------------------------
//
//	�Q�[��[GameScene.h]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _EACH_SCENE_H_
#define _EACH_SCENE_H_

#include <FrameWork/Define/Define.h>

enum class SceneList : uint8		// �V�[����
{
	NEXT		= 255,
	NOTCHANGE	= 0,
	TITLE		= 1,		// �^�C�g��
	GAME,			// �Q�[��
	RESULT,			// ���U���g
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

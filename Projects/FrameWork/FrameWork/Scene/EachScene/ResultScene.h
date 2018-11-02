//-----------------------------------------------------------------------------
//
//	���U���g[ResultScene.h]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _RESULT_SCENE_H_
#define _RESULT_SCENE_H_

#include "../../Define/Define.h"
#include "../BaseScene.h"
#include "../../Systems/GameSystems.h"
#include "../../Systems/DebugSetting/GUI.h"

//-----------------------------------------------------------------------------
//	�N���X�錾
//-----------------------------------------------------------------------------
class ResultScene : public BaseScene, public GUI
{
public:
	ResultScene(SceneManager* manager);		// �R���X�g���N�^
	~ResultScene(void);						// �f�X�g���N�^

private:
	int Update(void) override;						// �X�V����	
};

#endif // _RESULT_SCENE_H_

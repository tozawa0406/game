//-----------------------------------------------------------------------------
//
//	���U���g[ResultScene.h]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _RESULT_SCENE_H_
#define _RESULT_SCENE_H_

#include <FrameWork/Scene/BaseScene.h>

//-----------------------------------------------------------------------------
//	�N���X�錾
//-----------------------------------------------------------------------------
class ResultScene : public BaseScene, public GUI
{
public:
	ResultScene(SceneManager* manager);		// �R���X�g���N�^
	~ResultScene(void);						// �f�X�g���N�^

	void Init(void)   override;
	void Uninit(void) override;
private:
	SceneList Update(void) override;						// �X�V����	
};

#endif // _RESULT_SCENE_H_

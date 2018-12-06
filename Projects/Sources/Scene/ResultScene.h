/*
 * @file		ResultScene.h
 * @brief		���U���g�V�[��
 * @author		���V�đ�
 * @data		2018/08/18
 */
#ifndef _RESULT_SCENE_H_
#define _RESULT_SCENE_H_

#include <FrameWork/Scene/BaseScene.h>

class ResultScene : public BaseScene, public GUI
{
public:
	ResultScene(SceneManager* manager);
	~ResultScene(void);

	void Init(void)   override;
	void Uninit(void) override;
private:
	SceneList Update(void) override;						// �X�V����	
};

#endif // _RESULT_SCENE_H_

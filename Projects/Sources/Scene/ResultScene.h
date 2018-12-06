/*
 * @file		ResultScene.h
 * @brief		リザルトシーン
 * @author		戸澤翔太
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
	SceneList Update(void) override;						// 更新処理	
};

#endif // _RESULT_SCENE_H_

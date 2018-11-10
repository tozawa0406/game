//-----------------------------------------------------------------------------
//
//	リザルト[ResultScene.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _RESULT_SCENE_H_
#define _RESULT_SCENE_H_

#include <FrameWork/Scene/BaseScene.h>

//-----------------------------------------------------------------------------
//	クラス宣言
//-----------------------------------------------------------------------------
class ResultScene : public BaseScene, public GUI
{
public:
	ResultScene(SceneManager* manager);		// コンストラクタ
	~ResultScene(void);						// デストラクタ

	void Init(void)   override;
	void Uninit(void) override;
private:
	SceneList Update(void) override;						// 更新処理	
};

#endif // _RESULT_SCENE_H_

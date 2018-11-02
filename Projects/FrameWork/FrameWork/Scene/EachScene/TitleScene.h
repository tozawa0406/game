//-----------------------------------------------------------------------------
//
//	タイトル[TitleScene.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _TITLE_SCENE_H_
#define _TITLE_SCENE_H_

#include "../BaseScene.h"
#include "../../Systems/Renderer/Sprite/CanvasRenderer.h"
#include "../../Systems/DebugSetting/GUI.h"
#include "../../Systems/GameSystems.h"

//-----------------------------------------------------------------------------
//	クラス宣言
//-----------------------------------------------------------------------------
class TitleScene : public BaseScene, public GUI
{
public:
	//定数定義
	static constexpr int FLASH_RANGE = 30;		// 点滅の間隔

	TitleScene(SceneManager* manager);	// コンストラクタ
	~TitleScene(void);					// デストラクタ

private:
	int  Update(void) override;					// 更新処理(返り値は選択番号)
};

#endif // _TITLE_SCENE_H_

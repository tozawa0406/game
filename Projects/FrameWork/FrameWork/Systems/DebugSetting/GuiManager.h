/*
 * @file		GuiManager.h
 * @brief		GUI管理
 * @author		戸澤翔太
 * @data		2018/11/05
 */
#ifndef _GUI_MANAGER_H_
#define _GUI_MANAGER_H_

#include "../../Define/Define.h"
#include "../BaseManager.h"
#include "../../Windows/Windows.h"
#include "ImGUI/imgui.h"
#include "GUI.h"

class Systems;
class Debug;
class GuiManager : public BaseManager<GUI>
{
public:
	static constexpr int FLASHING   = 10;
	static constexpr int SELECT_GUI = 30;

	GuiManager(void);
	~GuiManager(void);
	HRESULT Init(void)   override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	Draw(void)   override;

	void	GuiUpdate(void);

	const VECTOR3* GetSelect(void);

	// @brief	Initの前に呼び出し
	inline void SetDebug(Debug* debug) { debug_ = debug; }

	// Getter
	inline Debug*	GetDebug(void)		{ return debug_;		}
	inline bool		GetLookObject(void) { return lookObject_;	}

private:
	//! 親へのポインタ
	Debug* debug_;
	friend GUI;

	//! Object群のGuiリスト表示
	bool guiObject_;
	//! Subject群のGuiリスト表示
	bool guiSubject_;
	//! Guiを選択したオブジェクトを注視する
	bool lookObject_;
	//! アップデート後に描画したかどうか
	bool draw_;
	//! 現在表示しているGuiの数
	int drawNum_;
	//! 演出で使うフレームのカウンタ
	int frame_;
};

#endif // _GUI_MANAGER_H_

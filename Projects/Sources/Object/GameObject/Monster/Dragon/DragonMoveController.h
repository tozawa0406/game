/*
 * @file		DragonMoveController.h
 * @brief		敵の動きの制御
 * @author		戸澤翔太
 * @data		2018/11/15
 */
#ifndef _DRAGON_MOVE_CONTROLLER_H_
#define _DRAGON_MOVE_CONTROLLER_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Systems/DebugSetting/GUI.h>
#include "../../GameObject.h"

class Dragon;
class DragonMoveController : public GUI
{
public:
	/* @brief	コンストラクタ		*/
	DragonMoveController(void);
	/* @brief	デストラクタ		*/
	~DragonMoveController(void);


	
	/* @brief	初期化
	 * @param	(parent)	動かすもの
	 * @return	なし				*/
	void Init(Dragon* parent);
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	void Uninit(void);
	
	/* @brief	行動処理
	 * @param	(act)	行動フラグ
	 * @param	(flag)	状態フラグ
	 * @return	なし				*/
	void Action(int& act, uint& flag);
	
	/* @brief	標的の設定
	 * @param	(target)	標的のポインタ	*/
	inline void SetTarget(GameObject* target) { target_ = target; }
	
	/* @brief	Guiの更新処理
	 * @param	なし
	 * @return	なし				*/
	void GuiUpdate(void) override;

private:
	Dragon*			parent_;	//! 親のポインタ
	GameObject*		target_;	//! 標的のポインタ

	VECTOR3 dir_;		//! 方向
	bool	isDash_;	//! ダッシュフラグ
	int		cnt_;		//! カウンタ
};

#endif // _DRAGON_MOVE_CONTROLLER_H_

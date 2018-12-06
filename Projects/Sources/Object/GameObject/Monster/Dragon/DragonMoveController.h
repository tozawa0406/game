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
	DragonMoveController(void);
	~DragonMoveController(void);

	void Init(Dragon* parent);
	void Uninit(void);

	void GuiUpdate(void) override;

	/* @brief	標的の設定
	 * @param	(target)	標的のポインタ	*/
	inline void SetTarget(GameObject* target) { target_ = target; }

	void Action(int& act, uint& flag);


private:
	Dragon*			parent_;	//! 親のポインタ
	GameObject*		target_;	//! 標的のポインタ

	VECTOR3 dir_;		//! 方向
	bool	isDash_;	//! ダッシュフラグ
	int		cnt_;		//! カウンタ
};

#endif // _DRAGON_MOVE_CONTROLLER_H_

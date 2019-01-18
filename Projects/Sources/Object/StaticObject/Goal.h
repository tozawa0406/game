/*
 * @file		Goal.h
 * @brief		次のシーンへ遷移するオブジェクト
 * @author		戸澤翔太
 * @data		2018/01/18
 */
#ifndef _GOAL_H_
#define _GOAL_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class Goal : public Object
{
public:
	/* @brief	コンストラクタ		*/
	Goal(void);
	/* @brief	デストラクタ		*/
	~Goal(void);


	
	/* @brief	初期化処理
	 * @param	なし
	 * @return	なし				*/
	void Init(void) override;
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	void Uninit(void) override;

	/* @brief	更新処理			*/
	void Update(void) override;

	inline bool IsChange(void) { return change_; }

private:
	//! 当たり判定
	Collider3D::OBB*	collider_;
	//! シーン遷移
	bool				change_;
};

#endif // _GOAL_H_

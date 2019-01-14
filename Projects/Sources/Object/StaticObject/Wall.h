/*
 * @file		Wall.h
 * @brief		壁のオブジェクト生成管理
 * @author		戸澤翔太
 * @data		2019/01/15
 */
#ifndef _WALL_H_
#define _WALL_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class Wall : public Object
{
public:
	/* @brief	コンストラクタ		*/
	Wall(void);
	/* @brief	デストラクタ		*/
	~Wall(void);
	


	/* @brief	初期化処理
	 * @param	なし
	 * @return	なし				*/
	void Init(void) override;
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	void Uninit(void) override;

	/* @brief	更新処理(動かないためなし)		*/
	void Update(void) override {}
};

#endif // _WALL_H_

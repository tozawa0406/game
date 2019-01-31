/*
 * @file		DontDestroyOnLoad.h
 * @brief		シーン間に値を保持するクラス
 * @author		戸澤翔太
 * @data		2019/01/21
 */
#ifndef _DONT_DESTROY_ON_LOAD_H_
#define _DONT_DESTROY_ON_LOAD_H_

#include <FrameWork/Define/Define.h>
#include <unordered_map>

enum class DontDestroyList : uint8
{
	PLAYER_STATE,
	MAX_LIFE,
	MAX_STAMINE,
	CURRENT_LIFE,
	CURRENT_STAMINA,
	STAMINA_CNT,
	TIMER_TIME,
	TIMER_FRAME,
	TIMER_SECOND,
	CURRENT_ITEM,

	MAX
};

class DontDestroyOnLoad
{
public:
	/* @brief	コンストラクタ		*/
	DontDestroyOnLoad(void);
	/* @brief	デストラクタ		*/
	~DontDestroyOnLoad(void);

	/* @brief	初期化処理
	 * @param	なし
	 * @return	なし				*/
	void Init(void);

	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	void Uninit(void);

	/* @brief	リセット処理
	 * @param	なし
	 * @return	なし				*/
	void Reset(void);

	/* @brief	値のセーブ処理
	 * @param	(type)		型指定(テンプレートと絶対に合わせること)
	 * @param	(num)		リスト番号(重複していた場合上書き)
	 * @param	(value)		保持する値
	 * @return	なし				*/
	template<class T>
	void Save(DontDestroyList num, T value) {}
	template<>
	void Save(DontDestroyList num, int value) { intList_[num] = value; }	
	template<>
	void Save(DontDestroyList num, float value) { floatList_[num] = value; }

	/* @brief	値のロード処理
	 * @param	(num)		リスト番号
	 * @return	値					*/
	template<class T>
	T		Load(DontDestroyList num) { return num; }
	template<>
	int		Load<int>(DontDestroyList num) { return (intList_.count(num) == 0) ? 0 : intList_[num]; }
	template<>
	float	Load<float>(DontDestroyList num) { return (floatList_.count(num) == 0) ? 0 : floatList_[num]; }


private:
	//! intを格納するマップ
	std::unordered_map<DontDestroyList, int>	intList_;
	//! floatを格納するマップ
	std::unordered_map<DontDestroyList, float>	floatList_;

};

#endif // _DONT_DESTROY_ON_LOAD_H_

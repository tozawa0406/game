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
	void Save(const string& key, T value) { UNREFERENCED_PARAMETER(key); UNREFERENCED_PARAMETER(value); }
	template<>
	void Save(const string& key, int value) { intList_[key] = value; }
	template<>
	void Save(const string& key, float value) { floatList_[key] = value; }

	/* @brief	値のロード処理
	 * @param	(num)			リスト番号
	 * @param	(defaultValue)	キーがなかった時に帰ってくる値
	 * @return	値					*/
	template<class T>
	T		Load(const string& key, T defaultValue) { return num; }
	template<>
	int		Load<int>(const string& key, int defaultValue) { return (intList_.count(key) == 0) ? defaultValue : intList_[key]; }
	template<>
	float	Load<float>(const string& key, float defaultValue) { return (floatList_.count(key) == 0) ? defaultValue : floatList_[key]; }

private:
	//! intを格納するマップ
	std::unordered_map<string, int>	intList_;
	//! floatを格納するマップ
	std::unordered_map<string, float>	floatList_;

};

#endif // _DONT_DESTROY_ON_LOAD_H_

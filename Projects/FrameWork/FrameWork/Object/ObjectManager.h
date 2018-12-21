/*
 * @file		ObjectManager.h
 * @brief		オブジェクト管理クラス
 * @author		戸澤翔太
 * @data		2017/09/05
 */
#ifndef _OBJECT_MANAGER_H_ 
#define _OBJECT_MANAGER_H_

#include "../Scene/BaseScene.h"

class Object;
class ObjectManager : public BaseManager<Object>
{
public:
	/* @breif	コンストラクタ
	 * @param	(parent)	生成したシーン		*/
	ObjectManager(BaseScene* parent);
	/* @brief	デストラクタ	*/
	~ObjectManager(void);

	/* @brief	初期化処理
	 * @param	なし
	 * @return	成功失敗		*/
	HRESULT Init(void)   override;

	/* @brief	後処理
	 * @param	なし
	 * @return	なし			*/
	void    Uninit(void) override;

	/* @brief	更新処理
	 * @param	なし
	 * @return	なし			*/
	void    Update(void) override;

	/* @brief	オブジェクト取得		*/
	std::vector<Object*>& Get(void)        { return obj_;  }

	/* @fn		GetScene
	 * @brief	シーンの取得
	 * @return	自身の存在するシーン		*/
	inline BaseScene* GetScene(void) { return scene_; }


	
	/* @fn		Create
	 * @brief	オブジェクトの生成
	 * @param	newの際に必要な引数
	 * @return	オブジェクトのポインタ
	 * @detail	オブジェクトの生成、そして配列に追加し、
				オブジェクトにもマネージェーへのポインタを設定
				オブジェクトの初期化処理をしてそのポインタを返す		*/
	template<class T, class... Args>
	T* Create(Args... args)
	{
		const auto& temp = new T(args...);
		if (temp)
		{
			this->Add(temp);
			temp->SetObjectManager(this);
			temp->Init();
			return temp;
		}
		return nullptr;
	}


private:
	/* @brief	オブジェクト削除
	 * @param	なし
	 * @return	なし			*/
	void DestroyCheck(void);

	//! 自身の存在するシーン
	BaseScene* scene_;
};

#endif // _OBJECT_H_
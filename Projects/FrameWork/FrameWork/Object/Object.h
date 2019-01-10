/*
 * @file		Object.h
 * @brief		オブジェクト
 * @author		戸澤翔太
 * @data		2017/09/05
 */
#ifndef _OBJECT_H_ 
#define _OBJECT_H_

#include "../Define/Define.h"
#include "../Windows/Windows.h"
#include "../Systems/GameSystems.h"
#include "../Systems/Renderer/CanvasRenderer/CanvasRendererImage.h"
#include "../Systems/Renderer/Sprite/SpriteRenderer.h"
#include "../Systems/Renderer/Model/MeshRenderer.h"

#include "../Systems/DebugSetting/GUI.h"

#include "../../../Sources/Object/ObjectTag.h"

class Controller;
class BaseScene;
class GuiManager;
class Systems;
class Object
{
public:
	friend class ObjectManager;
protected:
	/* @brief	初期化処理		*/
	virtual void Init(void)   = 0;
	/* @brief	後処理			*/
	virtual void Uninit(void) = 0;
	/* @brief	更新処理		*/
	virtual void Update(void) = 0;

	/* @brief	タグの設定
	 * @param	(tag)	設定したいタグ
	 * @return	なし			*/
	inline void SetTag(ObjectTag tag)	{ tag_	= tag; }

	/* @brief	削除処理
	 * @param	なし
	 * @return	なし			*/
	inline void Destroy(void)	{ destroy_ = true; }

	/* @brief	削除状態取得
	 * @param	なし
	 * @return	削除状態		*/
	inline bool GetDestroy(void) { return destroy_; }

	/* @brief	コントローラ取得
	 * @param	(i)		コントローラ番号
	 * @return	コントロ―ラのポインタ		*/
	Controller* GetCtrl(int i);

	//! 姿勢
	Transform  transform_;
	//! マネージャー
	ObjectManager* manager_;

public:
	/* @brief	コンストラクタ		*/
	Object(ObjectTag tag);
	/* @brief	デストラクタ		*/
	virtual ~Object(void);

	/* @brief	姿勢取得			*/
	inline Transform&	GetTransform(void) { return transform_; }
	/* @brief	タグの取得			*/
	inline ObjectTag	GetTag(void)       { return tag_;       }

	/* @fn		SetObejctManager
	 * @brief	マネージャーの設定
	 * @param	(manager)	自身の管理者		*/
	inline void SetObjectManager(ObjectManager* manager) { manager_ = manager; }

private:
	//! 削除フラグ
	bool		destroy_;
	//! タグ
	ObjectTag	tag_;

	friend GuiManager;
};

#endif // _OBJECT_H_
/*
 * @file		RendererBase.h
 * @brief		描画の基底クラス
 * @author		戸澤翔太
 * @data		2018/12/20
 */
#ifndef _RENDERER_BASE_H_
#define _RENDERER_BASE_H_

#include "../../Define/Define.h"
#include "../../Graphics/Wrapper.h"
#include "../GameSystems.h"

class RendererBase
{
public:
	//! @enum	UI描画の種類
	enum class RendererType : uint8
	{
		Canvas = 0,
		Object,
		MAX
	};

	/* @brief	コンストラクタ	*/
	RendererBase(RendererType type) : enable_(true), rendererType_(type) {}
	/* @brief	デストラクタ	*/
	virtual ~RendererBase(void) {}

	/* @brief	後処理			*/
	virtual void Uninit() = 0;

	/* @brief	使用状況確認			*/
	inline bool			GetEnable(void)			const  { return enable_;		}
	/* @brief	描画タイプ確認			*/
	inline RendererType GetRendererTypr(void)	const { return rendererType_;	}

	/* @brief	使用状況設定			*/
	inline void SetEnable(bool enable) { enable_ = enable;	}

protected:
	bool			enable_;			//! 使用状況

private:
	RendererType	rendererType_;		//! 種類
};

#endif // _RENDERER_BASE_H_
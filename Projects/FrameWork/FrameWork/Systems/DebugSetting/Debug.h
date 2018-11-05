/*
 * @file		Debug.h
 * @brief		デバッグ操作
 * @author		戸澤翔太
 * @data		2018/11/05
 */
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "../../Define/Define.h"
#include "../BaseManager.h"

class GuiManager;
class Debug : public Interface
{
public:
	Debug(Systems* systems);
	~Debug(void);
	HRESULT Init(void)   override;
	void	Uninit(void) override;
	void    Update(void) override;
	void	Draw(void)   override;
	void    On(void);
	
	// Getter
	inline bool			GetDebug(void)		{ return debug_;		}
	inline bool			GetDebugPause(void) { return debugPause_;	}
	inline GuiManager*	GetGuiManager(void) { return gui_;			}
	// Setter
	inline void SetDebugPause(bool pause) { debugPause_ = pause; }

	/* @fn		BoolToString
	 * @brief	bool型の文字列描画
	 * @param	(judge)		文字列にしたいbool型
	 * @return	結果となる文字列		*/
	inline string BoolToString(bool judge) { return (judge) ? "true" : "false"; }

private:
	GuiManager*		gui_;			//! guiのマネージャ
	bool			debug_;			//! デッバグ時かどうか
	bool			debugPause_;	//! デバッグ用のポーズを行うかどうか
};

#endif // _DEBUG_H_
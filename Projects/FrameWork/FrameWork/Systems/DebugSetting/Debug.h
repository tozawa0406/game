//-----------------------------------------------------------------------------
//
//	デバッグ処理[Debug.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "../../Define/Define.h"
#include "../BaseManager.h"

class GuiManager;
class Debug : public Interface
{
	friend class Windows;
	friend class Systems;
	friend struct Primitive;
	friend struct ColliderRenderer;
public:

	Debug(Systems* systems);
	~Debug(void) override;
	HRESULT Init(void)  override { return S_OK; }
	void   Update(void) override;
	void   On(void);
	string BoolToString(bool judge);

	bool GetDebug(void) { return debug; }
	bool GetDebugPause(void) { return debugPause; }
	void SetDebugPause(bool pause) { debugPause = pause; }
	GuiManager* GetGuiManager(void) { return gui; }

private:
	void Draw(void) override;

	GuiManager*  gui;

	bool debug;
	bool debugPause;
};

#endif // _DEBUG_H_
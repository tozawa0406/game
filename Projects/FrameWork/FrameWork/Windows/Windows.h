/*
 * @file		Windows.h
 * @brief		Windowの処理全般
 * @author		戸澤翔太
 * @data		2018/05/10
 */
#ifndef _WINDOWS_H_
#define _WINDOWS_H_

#include "../Define/Define.h"
#include "../Graphics/Graphics.h"

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Xinput.lib")

class Systems;
class Windows
{
	//! @def	ウィンドウの名前
	static constexpr char*	CLASS_NAME		= "Dragon Hunter";
	//! @def	ウィンドウの名前
	static constexpr char*	WINDOW_NAME		= "Dragon Hunter";
	//! @def	ウィンドウスタイル
	static constexpr int	WINDOW_STYLE	= WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX;
	//! グラフィックスのタイプ(ダイアログで指定(今回はしない))
	Graphics::Type			graphicsType_	= Graphics::Type::DirectX11;

public:
	//! @def	フルスクリーン設定
	static constexpr bool FULL_SCREEN	= false;
	//! @def	FPSの設定
	static constexpr int  FPS			= 60;
	//! @def	ウィンドウ横サイズ
	static constexpr int  WIDTH			= 1280;
	//! @def	ウィンドウ縦サイズ
	static constexpr int  HEIGHT		= 720;

	/* @brief	コンストラクタ		*/
	Windows(void);
	/* @brief	デストラクタ		*/
	~Windows(void);

	/* @brief	FPSの取得処理		*/
	inline float		GetFps(void)		const { return fps_;		}
	/* @brief	HWNDの取得処理		*/
	inline HWND			GetHWND(void)		const { return hWnd_;		}
	/* @brief	MSGの取得処理		*/
	inline MSG			GetMsg(void)		const { return msg_;		}
	/* @brief	HINSTANCEの取得処理	*/
	inline HINSTANCE	GetHInstance(void)	const { return hInstance_;	}
	/* @brief	グラフィックスの種類取得	*/
	inline Graphics::Type GetGraphicsType(void) const { return graphicsType_; }

	/* @brief	Graphicsの取得処理	*/
	inline Graphics*	GetGraphics(void)	{ return graphics_;	}
	/* @brief	Systemsの取得処理	*/
	inline Systems*		GetSystems(void)	{ return systems_;	}



	/* @brief	初期化処理
	 * @sa		main()
	 * @param	(Instancs)	HINSTANCE
	 * @param	(cmdShow)	cmdShow
	 * @return	成功失敗			*/
	HRESULT Init(HINSTANCE Instance, int cmdShow);

	/* @brief	ゲームループ処理
	 * @sa		main()
	 * @param	(fps)	設定したいFPS
	 * @return	なし				*/
	void    GameLoop(DWORD fps);

	/* @brief	後処理
	 * @param	なし
	 * @return	MSG.wParamの値		*/
	WPARAM  Uninit(void);



	/* @brief	エラーメッセージウィンドウの出力
	 * @param	(message)	エラー内容
	 * @param	(error)		ウィンドウ名(エラーのジャンル)
	 * @param	(hr)		成功失敗の判定
	 * @return	hrが失敗ならtrue：成功ならfalse		*/
	bool ErrorMessage(const char* message, char* error, HRESULT hr);

private:
	/* @brief	ウィンドウ情報の設定
	 * @sa		Init()
	 * @param	なし
	 * @return	なし				*/
	void    SetWindowInfo(void);

	/* @brief	ウィンドウの生成
	 * @sa		Init()
	 * @param	(cmdShow)	cmdShow
	 * @return	なし				*/
	void    SetWindow(int cmdShow);

	/* @brief	メッセージプロセス
	 * @sa		GameLoop()
	 * @param	なし
	 * @return	ウィンドウを閉じたらtrue	*/
	bool    MsgProcess(void);

	/* @brief	更新処理
	 * @sa		GameLoop()
	 * @param	なし
	 * @return	GraphicsとSystemsの初期化に失敗したらtrue		*/
	bool    Update(void);

	/* @brief	描画処理
	 * @sa		GameLoop()
	 * @param	なし
	 * @return	なし				*/
	void    Draw(void);

	/* @brief	ゲーム系の初期化処理
	 * @param	(type)	グラフィックスの種類
	 * @return	初期化に失敗したらtrue		*/
	bool	InitGame(Graphics::Type type);

	/* @brief	Windowsの中心を返す
	 * @sa		SetWindow()
	 * @param	(dr)	ウィンドウの右
	 * @param	(wr)	調整位置の右
	 * @return	中心				*/
	inline LONG WindowCenter(LONG dr, LONG wr) { return Half(dr - wr) > 0 ? Half(dr - wr) : 0; }

	Graphics*	graphics_;		//! グラフィックスAPI系の処理
	Systems*	systems_;		//! システム系

	HWND		hWnd_;
	HINSTANCE	hInstance_;
	MSG			msg_;
	WNDCLASSEX	wcex_;

	int			timeNow_;		//! 現在時刻
	int			timeOld_;		//! 前の時刻
	int			timeFPS_;		//! FPSカウント時の時刻
	int			fpsCnt_;		//! FPS計測カウンタ
	float		fps_;			//! FPS

	/* @brief	ウィンドウプロシージャ		*/
	static LRESULT CALLBACK WndProc(HWND hWnd, uint uMsg, WPARAM wParam, LPARAM lParam);
};

#endif // _WINDOWS_H
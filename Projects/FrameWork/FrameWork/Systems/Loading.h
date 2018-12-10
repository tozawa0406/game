//-----------------------------------------------------------------------------
//
//	ÉçÅ[ÉhâÊñ [Loading.h]
//	Auther : åÀ‡V„ƒëæ
//																	2018/09/10
//-----------------------------------------------------------------------------
#ifndef _LOADING_H_
#define _LOADING_H_

#include "BaseManager.h"
#include "Renderer/CanvasRenderer/CanvasRendererImage.h"
#include <thread>
#include <mutex>

#include "Renderer/Sprite/Texture.h"
#include "Renderer/Model/Model.h"
#include "Sound/Sound.h"
#include "Score.h"

class Systems;
class Loading : public Interface
{
	static constexpr float BAR_SIZE_X			= 300;
	static constexpr float BAR_SIZE_Y			= 10;

	static constexpr float BAR_PADDING_X		= 100;
	static constexpr float BAR_PADDING_Y		= 75;

	static constexpr float FADE_SPAN			= 90;
	static constexpr float ROTATION_STOP_SPAN	= 30;
	static constexpr float ROTATION_SPEED		= 0.075f;
public:
	Loading(Systems* systems);
	~Loading(void);

	HRESULT Init(void)   override;
	void	Uninit(void) override;
	void	FadeAlpha(float a);

	void	EndFade(void) { for (auto& d : display_) { d.SetEnable(false); } loadingPercent_->SetEnable(false); }

	void	Start(int sceneNum);
	bool	IsLoading(void) { return LoadingSafe(); }
	void	AddLoading(void) { nowLoading_++; }
	void	End(void);

private:
	bool Load(int sceneNum);

	bool LoadingSafe(void);
	void LoadingSafe(bool loading);

	Texture*		texture_;
	Model*			model_;
	Sound*			sound_;

	Score*			loadingPercent_;
	int				allTask_;
	int				nowLoading_;
	int				loadingCnt_;
	float			patternCnt_;

	bool			isLoading_;
	std::mutex		mutex_;
	std::thread*	thread_;

	CanvasRenderer::Image	display_[4];
};

#endif // _LOADING_H_
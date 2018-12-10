//-----------------------------------------------------------------------------
//
//	スコア描画[Score.h]
//	Auther : 戸澤翔太
//																	2017/07/18
//-----------------------------------------------------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_

#include "../Define/Define.h"
#include "Renderer/Sprite/Texture.h"
#include "Renderer/CanvasRenderer/CanvasRendererImage.h"
#include "../Object/Object.h"

//-----------------------------------------------------------------------------
//	クラス定義
//-----------------------------------------------------------------------------
class SceneManager;
class Systems;
class Score
{
	// マクロ定義
	static constexpr int SECOND = 6;

public:
	static constexpr int GAME_TIME = 180;
	static constexpr int DIG_MAX = 7;

	Score(SceneManager* parent, bool bZero, int dig = DIG_MAX);
	~Score(void);

	void Update(int score, VECTOR2 pos);

	void    SetEnable(bool enable)   { for (int i = 0; i < dig_; i++) { (texture_ + i)->SetEnable(enable); } }
	void    SetColor(COLOR color)	 { color_ = color; }
	void    SetScale(float scale)    { scale_ = scale;  }
	void	SetPriority(byte p)		 { priority_ = p; }
	VECTOR2 GetSize(void)            { return texture_->GetSize() * scale_; }

private:
	SceneManager* parent_;

	int SCORE_MAX_ = 0;					//最大桁数
	byte	priority_;
	CanvasRenderer::Image* texture_;
	COLOR	 color_;
	float    scale_;
	bool     bZero_;
	int      dig_;

	Systems* systems_;
};

class Manager;
class NumBoard
{
public:
	NumBoard(BaseScene* parent, VECTOR2 pos = { 0, 0 }, float scale = 1.f);
	~NumBoard(void);

	bool Update(void);

private:
	BaseScene* scene_;

	VECTOR2 position_;
	int     timeCnt_;
	CanvasRenderer::Image back_;
	Score*    min_;
	Score*    sec_;
};

#endif // _SCORE_H_

//-----------------------------------------------------------------------------
//
//	システム系[GameSystems.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "GameSystems.h"
#include "../Windows/Windows.h"
#include "../Graphics/Graphics.h"
#include "../Scene/SceneManager.h"
#include "Screenshot.h"

Systems* Systems::system_;

Systems::Systems(Windows* window) : window_(window), graphics_(nullptr)
{
	graphics_ = window->GetGraphics();
}

HRESULT Systems::Init(void)
{
	system_ = this;

	for (auto& i : manager_)
	{
		i = nullptr;
	}

	// インスタンス生成 & 初期化
	if (!Create<Debug>					 ((int)SystemsNum::DEBUG          , "デバッグ機能"		)) { return E_FAIL; }
	if (!Create<Input>					 ((int)SystemsNum::INPUT          , "入力"              )) { return E_FAIL; }
	if (!Create<CanvasRendererManager>	 ((int)SystemsNum::CANVASRENDERER , "2D描画"            )) { return E_FAIL; }
	if (!Create<ObjectRendererManager>	 ((int)SystemsNum::OBJECTRENDERER , "3D描画"			)) { return E_FAIL; }
	if (!Create<Texture>				 ((int)SystemsNum::TEXTURE        , "テクスチャ"		)) { return E_FAIL; }
	if (!Create<Model>					 ((int)SystemsNum::MODEL		  , "モデル"			)) { return E_FAIL; }
	if (!Create<ShaderManager>			 ((int)SystemsNum::SHADER         , "シェーダー"		)) { return E_FAIL; }
	if (!Create<Sound>					 ((int)SystemsNum::SOUND          , "サウンド"			)) { return E_FAIL; }
	if (!Create<Collision2DManager>		 ((int)SystemsNum::COLLISION2D    , "2D当たり判定"		)) { return E_FAIL; }
	if (!Create<Collision3DManager>		 ((int)SystemsNum::COLLISION3D    , "3D当たり判定"		)) { return E_FAIL; }
	if (!Create<ColliderRendererManager> ((int)SystemsNum::COLLISIONDRAW  , "当たり判定描画"	)) { return E_FAIL; }
	if (!Create<ParticleManager>		 ((int)SystemsNum::PARTICLE		  , "パーティクル"		)) { return E_FAIL; }
	if (!Create<SceneManager>			 ((int)SystemsNum::SCENE          , "シーン"			)) { return E_FAIL; }
	if (!Create<Screenshot>				 ((int)SystemsNum::SCREENSHOT     , "スクリーンショット")) { return E_FAIL; }

	return S_OK;
}

void Systems::Uninit(void)
{
	// 後処理
	for (int i = (int)SystemsNum::MAX - 1; i >= 0; --i)
	{
		if (manager_[i])
		{
			manager_[i]->Uninit();
			DeletePtr(manager_[i]);
		}
	}
}

void Systems::Update(void)
{
	for (int i = 0; i < (int)SystemsNum::MAX; ++i)
	{
		if (manager_[i])
		{
			manager_[i]->Update();
		}
	}
}

void Systems::Draw(void)
{
	// 影の描画
	const auto& objRenderer = (ObjectRendererManager*)manager_[(int)SystemsNum::OBJECTRENDERER];
	objRenderer->DrawShadow();

	window_->GetGraphics()->ClearRenderer();

	// ゲームの描画
	const auto& scene = (SceneManager*)manager_[(int)SystemsNum::SCENE];
	scene->Draw();

	// 天球とフィールドを先に描画
	objRenderer->FastDraw();

	for (int i = 0; i < (int)SystemsNum::MAX; ++i)
	{
		if (i == (int)SystemsNum::SCENE || i == (int)SystemsNum::DEBUG) { continue; }
		if (!manager_[i]) { continue; }
		manager_[i]->Draw();
	}

	manager_[(int)SystemsNum::DEBUG]->Draw();
}

template<class T>
bool Systems::Create(int i, string error)
{
	manager_[i] = new T(this);
	string message = error + "の初期化に失敗しました";
	if (window_->ErrorMessage(message.c_str(), "エラー", manager_[i]->Init()))
	{
		return false;
	}
	return true;
}

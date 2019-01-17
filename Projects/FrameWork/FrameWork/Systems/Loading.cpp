//-----------------------------------------------------------------------------
//
//	ƒ[ƒh‰æ–Ê[Loading.cpp]
//	Auther : ŒËàVãÄ‘¾
//																	2018/09/10
//-----------------------------------------------------------------------------
#include "Loading.h"
#include "GameSystems.h"
#include "../Windows/Windows.h"

Loading::Loading(Systems* systems) : Interface(systems), isLoading_(false), thread_(nullptr), nowLoading_(0), allTask_(0), loadingCnt_(0), patternCnt_(0)
{
	texture_ = systems_->GetTexture();
	model_   = systems_->GetModel();
	sound_   = systems_->GetSound();
}

Loading::~Loading(void)
{
}

HRESULT Loading::Init(void)
{
	display_[0].Init(251, (int)Resources::Texture::Base::WHITE);
	display_[0].SetPosition(VECTOR2(Half(Windows::WIDTH), Half(Windows::HEIGHT)));
	display_[0].SetSize(VECTOR2(Windows::WIDTH, Windows::HEIGHT));
	display_[0].SetColor(COLOR(0, 0, 0, 0));
	display_[0].SetEnable(false);

	float c = 0.3f;
	display_[1].Init(252, (int)Resources::Texture::Base::WHITE);
	VECTOR2 adjust = { BAR_PADDING_X + Half(BAR_SIZE_X), BAR_PADDING_Y + Half(BAR_SIZE_Y) };
	display_[1].SetPosition(VECTOR2(Windows::WIDTH - adjust.x, Windows::HEIGHT - adjust.y));
	display_[1].SetSize(VECTOR2(BAR_SIZE_X, BAR_SIZE_Y));
	display_[1].SetColor(COLOR(c, c, c, 1));
	display_[1].SetEnable(false);

	c = 0.7f;
	display_[2].Init(253, (int)Resources::Texture::Base::WHITE);
	display_[2].SetPosition(VECTOR2(Windows::WIDTH - adjust.x, Windows::HEIGHT - adjust.y));
	display_[2].SetSize(VECTOR2(0, (float)BAR_SIZE_Y));
	display_[2].SetColor(COLOR(c, c, c, 1));
	display_[2].SetEnable(false);

	display_[3].Init(253, (int)Resources::Texture::Base::LOAD);
	auto p = display_[0].GetPosition();
	p.y -= 100;
	display_[3].SetPosition(p);
	display_[3].SetSize(VECTOR2(150, 150));
	display_[3].SetRotationOffset(VECTOR2(75, 75));
	display_[3].SetColor(COLOR(1, 1, 1, 1));
	display_[3].SetEnable(false);
	display_[3].SetSplit(VECTOR2(3, 1));
	display_[3].SetPattern(0);

	loadingPercent_ = new Score(systems_->GetSceneManager(), false, 3);
	loadingPercent_->SetPriority(252);
	loadingPercent_->SetScale(0.5f);
	loadingPercent_->SetColor(COLOR(c, c, c, 1));

	return S_OK;
}

void Loading::Uninit(void)
{
	for (auto& d : display_) { d.Uninit(); }
	DeletePtr(loadingPercent_);
}

void Loading::FadeAlpha(float a)
{
	// “‚Ì•`‰æ
	if (allTask_ > 0)
	{
		float percent = nowLoading_ / (float)allTask_;
		auto size = display_[2].GetSize();
		size.x = percent * BAR_SIZE_X;
		display_[2].SetSize(size);
		auto pos = display_[2].GetPosition();
		pos.x = Windows::WIDTH - (BAR_PADDING_X + BAR_SIZE_X) + Half(size.x);
		display_[2].SetPosition(pos);
		
		pos = { Windows::WIDTH - (BAR_PADDING_X + loadingPercent_->GetSize().x) , Windows::HEIGHT - BAR_PADDING_Y * 1.3f + Half(BAR_SIZE_Y) };
		loadingPercent_->Update(min((int)(percent * 100), 100), pos);
	}

	// ’†‰›‰æ‘œ
	if (isLoading_)
	{
		patternCnt_ += 0.025f;
		display_[3].SetPattern((float)((int)patternCnt_ % 2));
		if ((int)patternCnt_ % 4 == 3) { display_[3].SetPattern(2); }
		display_[3].SetEnable(true);
	}

	for (int i = 0; i < 3; ++i)
	{
		auto c = display_[i].GetColor();
		c.a = a;
		display_[i].SetColor(c);
		display_[i].SetEnable(true);
	}
}

void Loading::Start(int sceneNum)
{
	LoadingSafe(true);

	// Šù‚ÉŽ‚Á‚Ä‚¢‚éƒŠƒ\[ƒX‚Ì”jŠü
	texture_->Release();
	model_->Release();
	sound_->Release();

	// ‘SƒŠƒ\[ƒX”‚ðŽæ“¾
	allTask_ += texture_->SetUpLoading(this, sceneNum);
	allTask_ += model_->SetUpLoading(this, sceneNum);
	allTask_ += sound_->SetUpLoading(this, sceneNum);

	thread_ = new std::thread([=]() { return Load(sceneNum); });
}

bool Loading::Load(int sceneNum)
{
	texture_->Load(sceneNum);
	model_->Load(sceneNum);
	sound_->Load(sceneNum);

	// 100%‚ð•`‰æ‚·‚é‚½‚ßI—¹‚Ü‚Å100ƒ~ƒŠ•b(0.1•b)‘Ò‚Â
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	LoadingSafe(false);
	return true;
}

void Loading::End(void)
{
	allTask_			= 0;
	nowLoading_			= 0;
	loadingCnt_			= 0;
	patternCnt_			= 0;
	auto size = display_[2].GetSize();
	size.x = 0;
	display_[2].SetSize(size);
	display_[2].SetEnable(true);
	display_[3].SetAngle(0);
	display_[3].SetEnable(false);

	thread_->detach();
	DeletePtr(thread_);
}

bool Loading::LoadingSafe(void)
{
	std::lock_guard<std::mutex> lock(mutex_);

	return isLoading_;
}

void Loading::LoadingSafe(bool loading)
{
	std::lock_guard<std::mutex> lock(mutex_);
	
	isLoading_ = loading;
}

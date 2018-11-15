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
	display_[0].Init(systems_, 251, (int)Texture::Base::FILL_RECTANGLE);
	display_[0].position	= VECTOR2(Half((float)Graphics::WIDTH), Half((float)Graphics::HEIGHT));
	display_[0].size		= VECTOR2((float)Graphics::WIDTH, (float)Graphics::HEIGHT);
	display_[0].color		= COLOR(0, 0, 0, 0);
	display_[0].enable		= false;

	float c = 0.3f;
	display_[1].Init(systems_, 252, (int)Texture::Base::FILL_RECTANGLE);
	VECTOR2 adjust = { BAR_PADDING_X + Half(BAR_SIZE_X), BAR_PADDING_Y + Half(BAR_SIZE_Y) };
	display_[1].position	= VECTOR2(Graphics::WIDTH - adjust.x, Graphics::HEIGHT - adjust.y);
	display_[1].size		= VECTOR2(BAR_SIZE_X, BAR_SIZE_Y);
	display_[1].color		= COLOR(c, c, c, 1);
	display_[1].enable		= false;

	c = 0.7f;
	display_[2].Init(systems_, 253, (int)Texture::Base::FILL_RECTANGLE);
	display_[2].position	= VECTOR2(Graphics::WIDTH - adjust.x, Graphics::HEIGHT - adjust.y);
	display_[2].size		= VECTOR2(0, (float)BAR_SIZE_Y);
	display_[2].color		= COLOR(c, c, c, 1);
	display_[2].enable		= false;

	display_[3].Init(systems_, 253, (int)Texture::Base::LOAD);
	display_[3].position		= display_[0].position;
	display_[3].position.y		-= 100;
	display_[3].size			= VECTOR2(150, 150);
	display_[3].rotationOffset	= VECTOR2(75, 75);
	display_[3].color			= COLOR(1, 1, 1, 1);
	display_[3].enable			= false;
	display_[3].split			= VECTOR2(3, 1);
	display_[3].pattern			= 0;

	loadingPercent_ = new Score(systems_->GetSceneManager(), false, 3);
	loadingPercent_->SetPriority(252);
	loadingPercent_->SetScale(0.5f);
	loadingPercent_->SetColor(COLOR(c, c, c, 1));

	return S_OK;
}

void Loading::Uninit(void)
{
	DeletePtr(loadingPercent_);
}

void Loading::FadeAlpha(float a)
{
	// “‚Ì•`‰æ
	if (allTask_ > 0)
	{
		float percent = nowLoading_ / (float)allTask_;
		display_[2].size.x = percent * BAR_SIZE_X;
		display_[2].position.x = (float)Graphics::WIDTH - (BAR_PADDING_X + BAR_SIZE_X) + Half(display_[2].size.x);
		
		VECTOR2 pos = { Graphics::WIDTH - (BAR_PADDING_X + loadingPercent_->GetSize().x) , Graphics::HEIGHT - BAR_PADDING_Y * 1.3f + Half(BAR_SIZE_Y) };
		loadingPercent_->Update(min((int)(percent * 100), 100), pos);
	}

	// ’†‰›‰æ‘œ
	if (isLoading_)
	{
		patternCnt_ += 0.025f;
		display_[3].pattern = (float)((int)patternCnt_ % 2);
		if ((int)patternCnt_ % 4 == 3) { display_[3].pattern = 2; }
		display_[3].enable = true;
	}

	for (int i = 0; i < 3; ++i)
	{
		display_[i].color.a = a;
		display_[i].enable = true;
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
	display_[2].size.x	= 0;
	display_[2].enable	= true;
	display_[3].angle   = 0;
	display_[3].enable  = false;

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

#include "ClearFailed.h"
#include <FrameWork/Windows/Windows.h>

ClearFailed::ClearFailed(void) : Object(ObjectTag::UI)
	, cnt_(0)
{
}

ClearFailed::~ClearFailed(void)
{
}

void ClearFailed::Init(void)
{
	img_.Init(240, static_cast<int>(Texture::Game::CLEAR_FAILED_UI));
	img_.SetPosition(VECTOR2((float)Half(Windows::WIDTH), 0));
	img_.SetSize(VECTOR2(760, 400));
	img_.SetSplit(VECTOR2(1, 2));
}

void ClearFailed::Uninit(void)
{
	img_.Uninit();
}

void ClearFailed::Update(void)
{
	if (!img_.GetEnable()) { return; }

	VECTOR2 pos = img_.GetPosition();
	img_.SetPosition(VECTOR2(pos.x, min((float)Half(Windows::HEIGHT), pos.y + 10)));
}

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
	VECTOR2 w = VECTOR2(static_cast<float>(Windows::WIDTH), static_cast<float>(Windows::HEIGHT));

	img_.Init(240, static_cast<int>(Texture::Camp::CLEAR_FAILED_UI));
	img_.SetPosition(VECTOR2(Half(w.x), 0));
	img_.SetSize(VECTOR2(760, 400));
	img_.SetSplit(VECTOR2(1, 2));

	brack_.Init(240, static_cast<int>(Texture::Base::WHITE));
	brack_.SetColor(COLOR(0, 0, 0, 0.5f));
	brack_.SetPosition(VECTOR2(Half(w.x), Half(w.y)));
	brack_.SetSize(w);
	brack_.SetEnable(false);
}

void ClearFailed::Uninit(void)
{
	brack_.Uninit();
	img_.Uninit();
}

void ClearFailed::Update(void)
{
	if (!img_.GetEnable()) { return; }
	brack_.SetEnable(true);

	VECTOR2 pos = img_.GetPosition();
	img_.SetPosition(VECTOR2(pos.x, min(Half(Windows::HEIGHT), pos.y + 10)));
}

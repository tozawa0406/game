#include "PlayerLife.h"
#include <FrameWork/Windows/Windows.h>

//! @def	�`�揇
static constexpr int PRIORITY		= 100;
//! @def	�`��ʒu
static const	 VECTOR2 POSITION	= VECTOR2(580, Windows::HEIGHT * 0.1f - 30);
//! @def	�`��T�C�Y
static const	 VECTOR2 SIZE_DRAW	= VECTOR2(800, 80);
//! @def	������
static const	 VECTOR2 SPLIT		= VECTOR2(1, 3);

//! @def	�o�[�̃T�C�Y
static const	 VECTOR2 SIZE_BAR	= VECTOR2(SIZE_DRAW.x - 5, SIZE_DRAW.y * 0.8f);
//! @def	���C�t�o�[
static const	 VECTOR2 LIFE_POSITION		= VECTOR2(POSITION.x - 5, POSITION.y - 6.7f);
//! @def	�X�^�~�i�o�[
static const	 VECTOR2 STAMINA_POSITION	= VECTOR2(POSITION.x - 5, POSITION.y + 10);

//! @def	���C�t�o�[������
static const	 float ONE_LIFE  = SIZE_BAR.x / 150;
//! @def	�o�[�̃T�C�Y�ƈʒu�̔䗦
static const     float ONE_RANGE = 355.0f / 150.0f;

PlayerLife::PlayerLife(void) : Object(ObjectTag::UI)
	, player_(nullptr)
{
}

PlayerLife::~PlayerLife(void)
{
}

void PlayerLife::Init(void)
{
	int texNum = static_cast<int>(Resources::Texture::Camp::HP_BAR);

	// �w�i
	back_.Init(PRIORITY, texNum);
	back_.SetPosition(POSITION);
	back_.SetSize(SIZE_DRAW);
	back_.SetSplit(SPLIT);

	// ���C�t
	life_.Init(PRIORITY + 1, texNum);
	life_.SetPosition(LIFE_POSITION);
	life_.SetSize(SIZE_BAR);
	life_.SetSplit(SPLIT);
	life_.SetPattern(1);

	// �X�^�~�i
	stamina_.Init(PRIORITY + 1, texNum);
	stamina_.SetPosition(STAMINA_POSITION);
	stamina_.SetSize(SIZE_BAR);
	stamina_.SetSplit(SPLIT);
	stamina_.SetPattern(2);
}

void PlayerLife::Uninit(void)
{
	stamina_.Uninit();
	life_.Uninit();
	back_.Uninit();
}

void PlayerLife::Update(void)
{
	if (!player_) { return; }

	int life = player_->GetLife();
	auto pos	= life_.GetPosition();
	auto size	= life_.GetSize();
	pos.x	= LIFE_POSITION.x - ((150 - life) * ONE_RANGE);
	size.x	= life * ONE_LIFE;
	life_.SetPosition(pos);
	life_.SetSize(size);

	float stamina = player_->GetStamina();
	pos		= stamina_.GetPosition();
	size	= stamina_.GetSize();
	pos.x	= LIFE_POSITION.x - ((150 - stamina) * ONE_RANGE);
	size.x	= stamina * ONE_LIFE;
	stamina_.SetPosition(pos);
	stamina_.SetSize(size);
}

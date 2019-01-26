#include "PlayerLife.h"
#include <FrameWork/Windows/Windows.h>

//! @def	�`�揇
static constexpr int PRIORITY		= 100;
//! @def	�`��ʒu
static const	 VECTOR2 POSITION	= VECTOR2(580, Windows::HEIGHT * 0.1f - 30);
//! @def	���C�t�w�i�̕`��ʒu
static const	 VECTOR2 LIFE_BACK_POSITION = VECTOR2(POSITION.x, POSITION.y - 6.7f);
//! @def	�X�^�~�i�w�i�̕`��ʒu
static const	 VECTOR2 STAMINA_BACK_POSITION = VECTOR2(POSITION.x - 2, POSITION.y + 10);
//! @def	�l�ɑ΂���d��(������)
static constexpr float	 X_WEIGHT	= 5;
//! @def	�`��T�C�Y
static const	 VECTOR2 SIZE_DRAW	= VECTOR2(750, 20);
//! @def	������
static const	 VECTOR2 SPLIT		= VECTOR2(1, 4);

//! @def	�o�[�̃T�C�Y
static const	 VECTOR2 SIZE_BAR	= VECTOR2(SIZE_DRAW.x - 5, SIZE_DRAW.y * 0.8f);
//! @def	���C�t�o�[
static const	 VECTOR2 LIFE_POSITION		= VECTOR2(POSITION.x - 6, POSITION.y - 6.7f);
//! @def	�X�^�~�i�o�[
static const	 VECTOR2 STAMINA_POSITION	= VECTOR2(POSITION.x - 6, POSITION.y + 10);

//! @def	���C�t�o�[�w�i������
static const	 float ONE_LIFE_BACK = SIZE_DRAW.x / 150;
//! @def	���C�t�o�[������
static const	 float ONE_LIFE  = SIZE_BAR.x / 150;
//! @def	�o�[�̃T�C�Y�ƈʒu�̔䗦
static const     float ONE_RANGE = 335 / 150.0f;

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

	// ���C�t�w�i
	backLife_.Init(PRIORITY, texNum);
	backLife_.SetPosition(LIFE_BACK_POSITION);
	backLife_.SetSize(SIZE_DRAW);
	backLife_.SetSplit(SPLIT);
	backLife_.SetPattern(0);

	// �X�^�~�i�w�i
	backStamina_.Init(PRIORITY, texNum);
	backStamina_.SetPosition(STAMINA_BACK_POSITION);
	backStamina_.SetSize(SIZE_DRAW);
	backStamina_.SetSplit(SPLIT);
	backStamina_.SetPattern(1);


	// ���C�t
	life_.Init(PRIORITY + 1, texNum);
	life_.SetPosition(LIFE_POSITION);
	life_.SetSize(SIZE_BAR);
	life_.SetSplit(SPLIT);
	life_.SetPattern(2);

	// �X�^�~�i
	stamina_.Init(PRIORITY + 1, texNum);
	stamina_.SetPosition(STAMINA_POSITION);
	stamina_.SetSize(SIZE_BAR);
	stamina_.SetSplit(SPLIT);
	stamina_.SetPattern(3);
}

void PlayerLife::Uninit(void)
{
	stamina_.Uninit();
	life_.Uninit();
	backStamina_.Uninit();
	backLife_.Uninit();
}

void PlayerLife::Update(void)
{
	if (!player_) { return; }

	const auto& max = player_->GetMaxLifeStamina();
	// ���C�t�w�i
	MoveBar(backLife_, max.x, LIFE_BACK_POSITION, ONE_LIFE_BACK);
	// �X�^�~�i�w�i
	MoveBar(backStamina_, max.y, STAMINA_BACK_POSITION, ONE_LIFE_BACK);
	// ���C�t�o�[
	MoveBar(life_, static_cast<float>(player_->GetLife()), LIFE_POSITION, ONE_LIFE);
	// �X�^�~�i�o�[
	MoveBar(stamina_, player_->GetStamina(), STAMINA_POSITION, ONE_LIFE);
}

void PlayerLife::MoveBar(CanvasRenderer::Image& bar, float value, const VECTOR2& offset, const float weight)
{
	// ���ݒl�擾
	auto pos	= bar.GetPosition();
	auto size	= bar.GetSize();

	// �l�ɉ����ăT�C�Y�y�шʒu��ύX
	pos.x	= offset.x - ((150 - value) * ONE_RANGE);
	size.x	= value * weight;

	// �ύX�l��ݒ�
	bar.SetPosition(pos);
	bar.SetSize(size);
}

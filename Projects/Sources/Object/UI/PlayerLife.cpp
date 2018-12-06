#include "PlayerLife.h"
#include <FrameWork/Windows/Windows.h>

//! @def	�`�揇
static constexpr int PRIORITY		= 100;
//! @def	�`��ʒu
static const	 VECTOR2 POSITION	= VECTOR2(580, (float)Windows::HEIGHT * 0.1f - 30);
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

/* @brief	�R���X�g���N�^			*/
PlayerLife::PlayerLife(void) : Object(Object::Tag::UI)
	, player_(nullptr)
{
}

/* @brief	�f�X�g���N�^			*/
PlayerLife::~PlayerLife(void)
{
}

/* @brief	����������
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void PlayerLife::Init(void)
{
	const auto& systems = Systems::Instance();
	int texNum = static_cast<int>(Texture::Game::HP_BAR);

	// �w�i
	back_.Init(systems, PRIORITY, texNum);
	back_.position	= POSITION;
	back_.size		= SIZE_DRAW;
	back_.split		= SPLIT;

	// ���C�t
	life_.Init(systems, PRIORITY + 1, texNum);
	life_.position	= LIFE_POSITION;
	life_.size		= SIZE_BAR;
	life_.split		= SPLIT;
	life_.pattern	= 1;

	// �X�^�~�i
	stamina_.Init(systems, PRIORITY + 1, texNum);
	stamina_.position	= STAMINA_POSITION;
	stamina_.size		= SIZE_BAR;
	stamina_.split		= SPLIT;
	stamina_.pattern	= 2;
}

/* @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void PlayerLife::Uninit(void)
{
}

/* @brief	�X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void PlayerLife::Update(void)
{
	if (!player_) { return; }

	int life = player_->GetLife();
	life_.size.x		= life * ONE_LIFE;
	life_.position.x	= LIFE_POSITION.x - ((150 - life) * ONE_RANGE);

	float stamina = player_->GetStamina();
	stamina_.size.x		= stamina * ONE_LIFE;
	stamina_.position.x = LIFE_POSITION.x - ((150 - stamina) * ONE_RANGE);

}

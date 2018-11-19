#include "PlayerLife.h"
#include <FrameWork/Windows/Windows.h>

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����		*/
PlayerLife::PlayerLife(void) : Object(Object::Tag::UI)
	, player_(nullptr)
{
}

/* @fn		�f�X�g���N�^
 * @brief	...					*/
PlayerLife::~PlayerLife(void)
{
}

/* @fn		Init
 * @brief	����������
 * @param	�Ȃ�
 * @return	�Ȃ�				*/
void PlayerLife::Init(void)
{
	const auto& systems = Systems::Instance();

	back_.Init(systems, 100, static_cast<int>(Texture::Game::HP_BAR));
	back_.position = VECTOR2(580, (float)Windows::HEIGHT * 0.1f - 30);
	back_.size = VECTOR2(800, 80);
	back_.split = VECTOR2(1, 3);
}

/* @fn		Uninit
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�				*/
void PlayerLife::Uninit(void)
{
}

/* @fn		Update
 * @brief	�X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�				*/
void PlayerLife::Update(void)
{
	if (!player_) { return; }


}

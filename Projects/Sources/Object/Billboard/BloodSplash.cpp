#include "BloodSplash.h"

/* @fn		�R���X�g���N�^
 * @brief	������				*/
BloodSplash::BloodSplash(VECTOR3 position) : Object(Object::Tag::BILLBOARD)
{
	transform_.position = position;
	transform_.scale = VECTOR3(10);
}

/* @fn		�f�X�g���N�^
 * @brief						*/
BloodSplash::~BloodSplash(void)
{
}

/* @fn		Init
 * @brief	����������
 * @param	�Ȃ�
 * @return	�Ȃ�				*/
void BloodSplash::Init(void)
{
	renderer_.split = VECTOR2(8, 2);
	renderer_.Init(Systems::Instance(), static_cast<int>(Texture::Game::EXPLOSION), &transform_);
	renderer_.billbord	= true;
	renderer_.shadow	= false;
	renderer_.shader	= Shader::ENUM::BILLBOARD;
}

/* @fn		Uninit
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�				*/
void BloodSplash::Uninit(void)
{
}

/* @fn		Update
 * @brief	�X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�				*/
void BloodSplash::Update(void)
{
	if (renderer_.Animation(0.5f))
	{
		Destroy();
	}
}

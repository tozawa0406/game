#include "BloodSplash.h"

/* @fn		�R���X�g���N�^
 * @brief	������				*/
BloodSplash::BloodSplash(VECTOR3 position, float rotation) : Object(Object::Tag::BILLBOARD)
	, cnt_(0)
{
	transform_.position		= position;
	transform_.scale		= VECTOR3(2, 10, 1);
	transform_.rotation.z	= rotation;
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
	renderer_.Init(Systems::Instance(), static_cast<int>(Texture::Game::SLASH), &transform_);
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
	cnt_++;
	if (cnt_ > 30)
	{
		Destroy();
	}
}

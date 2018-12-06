#include "BloodSplash.h"

//! @def	�傫��
static const VECTOR3 SCALE = VECTOR3(2, 10, 1);

//! @def	������^�C�~���O(�t���[��)
static constexpr int DESTROY = 30;

/* @brief	�R���X�g���N�^			*/
BloodSplash::BloodSplash(VECTOR3 position, float rotation) : Object(Object::Tag::BILLBOARD)
	, cnt_(0)
{
	transform_.position		= position;
	transform_.scale		= SCALE;
	transform_.rotation.z	= rotation;
}

/* @brief	�f�X�g���N�^			*/
BloodSplash::~BloodSplash(void)
{
}

/* @brief	����������
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void BloodSplash::Init(void)
{
	renderer_.Init(Systems::Instance(), static_cast<int>(Texture::Game::SLASH), &transform_);
	renderer_.billbord	= true;
	renderer_.shadow	= false;
	renderer_.shader	= Shader::ENUM::BILLBOARD;
}

/* @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void BloodSplash::Uninit(void)
{
}

/* @brief	�X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void BloodSplash::Update(void)
{
	cnt_++;
	if (cnt_ > DESTROY)
	{
		Destroy();
	}
}

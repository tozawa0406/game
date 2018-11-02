#include "PaidGoodsBox.h"

//! @def	�����ʒu
static const VECTOR3 POSITION = VECTOR3(-30, 9.5f, 55);
//! @def	������]
static const VECTOR3 ROTATION = VECTOR3(0, -1.3f, 0);
//! @def	�����X�P�[��
static const VECTOR3 SCALE = VECTOR3(0.05f, 0.05f * 0.8f, 0.05f);

//! @def	�F
static const COLOR MESH_COLOR = COLOR(0.25f, 0.25f, 1, 1);

//! @def	�����蔻��̃T�C�Y
static const VECTOR3 COLLIDER_SIZE = VECTOR3(15, 12.5f, 31);
//! @def	�����蔻��̃I�t�Z�b�g
static const VECTOR3 COLLIDER_OFFSET = VECTOR3(0, -2, 1);



/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����				*/
PaidGoodsBox::PaidGoodsBox(void) : Object(Object::Tag::STATIC)
{
}

/* @fn		�f�X�g���N�^
 * @brief	...							*/
PaidGoodsBox::~PaidGoodsBox(void)
{
}

/* @fn		Init
 * @brief	����������
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	�p���A���b�V���A�����蔻��̏�����	�@*/
void PaidGoodsBox::Init(void)
{
	// �����p���̐ݒ�
	transform_ = Transform(POSITION, ROTATION, SCALE);

	// ���b�V��
	mesh_.Init(Systems::Instance(), (int)Model::Game::BOX, &transform_);
	mesh_.material.diffuse = MESH_COLOR;

	// �����蔻��
	collider_ = new Collider3D::OBB(this);
	if (collider_)
	{
		collider_->SetSize(COLLIDER_SIZE);
		collider_->SetOffset(COLLIDER_OFFSET);
		collider_->Update();
	}
}

/* @fn		Uninit
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�						*/
void PaidGoodsBox::Uninit(void)
{
}

#include "Debug.h"
#include "../../Windows/Windows.h"
#include "../GameSystems.h"
#include "GuiManager.h"

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����			*/
Debug::Debug(Systems* systems) : Interface(systems), debug_(false), debugPause_(false), gui_(nullptr)
{
}

/* @fn		�f�X�g���N�^
 * @brief	...						*/
Debug::~Debug(void)
{
}

/* @fn		Init
 * @brief	������
 * @param	�Ȃ�
 * @return	�������ɐ����������ǂ�����HRESULT�ŕԂ�		*/
HRESULT Debug::Init(void)
{
#ifdef _SELF_DEBUG
	// gui�̃}�l�[�W���[�̐���
	gui_ = new GuiManager;
	if (gui_)
	{
		gui_->SetDebug(this);
		if (FAILED(gui_->Init()))
		{
			return E_FAIL;
		}
	}
#endif

	return S_OK; 
}

/* @fn		Uninit
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void Debug::Uninit(void)
{
#ifdef _SELF_DEBUG
	UninitDeletePtr(gui_);
#endif
}

/* @fn		Update
 * @brief	�X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void Debug::Update(void)
{
	// �f�o�b�O���łȂ���ΕԂ�
	if (!debug_) { return; }

	if (gui_) { gui_->Update(); }
}

/* @fn		Draw
 * @brief	�`�揈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void Debug::Draw(void) 
{
	// �f�o�b�O���łȂ���ΕԂ�
	if (!debug_) { return; }

	if (gui_) { gui_->Draw(); }
}

/* @fn		On
 * @brief	�f�o�b�O�@�\��ON/OFF�X�C�b�`�֐�
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void Debug::On(void)
{
#ifdef _SELF_DEBUG
	//�t���O�̔��]
	debug_ = !debug_;
#endif
}

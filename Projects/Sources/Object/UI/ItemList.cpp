#include "ItemList.h"
#include <FrameWork/Windows/Windows.h>

//! @def	�`�揇
static constexpr int PRIORITY		= 100;

//! @def	�ʒu
static const	 VECTOR2 POSITION = VECTOR2(Windows::WIDTH - 250.0f, Windows::HEIGHT - 100.0f);
//! @def	�w�i�`��T�C�Y
static const	 VECTOR2 SIZE_BACK = VECTOR2(300, 100);
//! @def	�I�����̔w�i�`��T�C�Y
static const	 VECTOR2 SIZE_BACK_SELECT = VECTOR2(370, 100);
//! @def	�����̃A�C�e���̔w�i
static const	 VECTOR2 SIZE_ITEM_BACK = VECTOR2(125, 100);
//! @def	�A�C�e���ꗗ�̔w�i
static const	 VECTOR2 SIZE_ITEM_BACK_LIST = VECTOR2(SIZE_ITEM_BACK.x * 0.8f, SIZE_ITEM_BACK.y * 0.8f);
//! @def	�A�C�e���̖��O
static const	 VECTOR2 POSITION_ITEM_NAME = VECTOR2(POSITION.x, POSITION.y + 60);
//! @def	L�{�^��UI�ʒu
static const	 VECTOR2 POSITION_L = VECTOR2(POSITION.x - 95, POSITION.y);
//! @def	L�{�^��UI�T�C�Y
static const	 VECTOR2 SIZE_L = VECTOR2(100, 40);

//! @def	�A�C�e�����X�g�̋���
static constexpr int ITEM_RANGE = 40;
//! @def	�A�C�e���̕ύX����(�t���[��)
static constexpr int CHANGE_FRAME = 10;
//! @def	�A�C�e���̕ύX���̈ړ�����
static constexpr int MOVE_LIST = ITEM_RANGE / CHANGE_FRAME;
//! @def	�A�C�e���ړ����̑傫���ύX
static const	 VECTOR2 ITEM_SIZE_DIFF = VECTOR2(SIZE_ITEM_BACK.x - SIZE_ITEM_BACK_LIST.x, SIZE_ITEM_BACK.y - SIZE_ITEM_BACK_LIST.y) * 0.1f;

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����		*/
ItemList::ItemList(void) : Object(Object::Tag::UI), GUI(Systems::Instance(), this, "ItemList")
	, player_(nullptr)
	, flag_(0)
	, cnt_(0)
{
}

/* @fn		�f�X�g���N�^
 * @brief	...					*/
ItemList::~ItemList(void)
{
}

/* @fn		Init
 * @brief	����������
 * @param	�Ȃ�
 * @return	�Ȃ�				*/
void ItemList::Init(void)
{
	const auto& systems = Systems::Instance();
	int texNum = static_cast<int>(Texture::Game::ITEM_UI);

	// �w�i
	back_.Init(systems, PRIORITY, texNum);
	back_.position	= POSITION;
	back_.size		= SIZE_BACK;
	back_.split		= VECTOR2(1, 3);

	for (int i = 0; i < static_cast<int>(BackItem::MAX); ++i)
	{
		backItemBack_[i].Init(systems, PRIORITY + 2, texNum);
		float x = POSITION.x + ((ITEM_RANGE * 2) - (i * ITEM_RANGE));
		backItemBack_[i].position	= VECTOR2(x, POSITION.y);
		backItemBack_[i].split		= VECTOR2(2, 3);
		backItemBack_[i].pattern	= 2;
		backItemBack_[i].size		= SIZE_ITEM_BACK_LIST;
	}
	SetItemBack();

	// �A�C�e���̖��O
	itemName_.Init(systems, PRIORITY + 2, texNum);
	itemName_.position	= POSITION_ITEM_NAME;
	itemName_.size		= SIZE_BACK;
	itemName_.split		= VECTOR2(1, 3);
	itemName_.pattern	= 2;

	// �{�^��UI
	int button = static_cast<int>(ButtonUI::L);
	ui_[button].Init(systems, PRIORITY + 1, texNum);
	ui_[button].position	= POSITION_L;
	ui_[button].size		= SIZE_L;
	ui_[button].split		= VECTOR2(2, 6);
	ui_[button].pattern		= 5;

	button = static_cast<int>(ButtonUI::MARU);
	ui_[button].Init(systems, PRIORITY + 5, texNum);
	ui_[button].position	= VECTOR2(POSITION.x + (ITEM_RANGE * 3), POSITION.y + 30);
	ui_[button].size		= VECTOR2(60, 50);
	ui_[button].split		= VECTOR2(4, 6);
	ui_[button].pattern		= 15;
	ui_[button].enable		= false;
	
	button = static_cast<int>(ButtonUI::SHIKAKU);
	ui_[button].Init(systems, PRIORITY + 5, texNum);
	ui_[button].position	= VECTOR2(POSITION.x - (ITEM_RANGE * 3), POSITION.y + 30);
	ui_[button].size		= VECTOR2(60, 50);
	ui_[button].split		= VECTOR2(4, 6);
	ui_[button].pattern		= 14;
	ui_[button].enable		= false;
}

/* @fn		Uninit
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�				*/
void ItemList::Uninit(void)
{
}

/* @fn		Update
 * @brief	�X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�				*/
void ItemList::Update(void)
{
	if (!player_) { return; }
	const auto& ctrl = GetCtrl(0);
	if (!ctrl) { return; }

	// L����
	if (ctrl->Press(Input::GAMEPAD_L1, DIK_Z))
	{
		// �������ɔw�i�����Ɉ������΂�
		back_.size = SIZE_BACK_SELECT;

		// ���A�C�e���̕`��
		for (int i = 0; i < static_cast<int>(BackItem::EMPTY); ++i)
		{
			backItemBack_[i].enable = true;
		}

		// �{�^��UI�̕\���ύX
		SetButtonUIEnable(false, true, true);

		// �ړ����Ă��Ȃ���
		if (flag_ == 0)
		{
			// �ړ�
			SetMove(*ctrl, Input::GAMEPAD_SQUARE, DIK_K, Input::GAMEPAD_CIRCLE, DIK_H);
		}
	}
	// L�񉟉���
	else
	{
		// �ړ����Ă��Ȃ���
		if (flag_ == 0)
		{
			// �w�i�̃T�C�Y��߂�
			back_.size = SIZE_BACK;

			// �A�C�e���̕\��������
			for (auto& ui : backItemBack_) { ui.enable = false; }
			// ���������`��
			for (int i = static_cast<int>(BackItem::FrontRight); i <= static_cast<int>(BackItem::FrontLeft); ++i)
			{
				backItemBack_[i].enable = true;
			}

			// �{�^��UI�̕\���ύX
			SetButtonUIEnable(true, false, false);
		}
	}

	// �ړ����Ă��Ȃ���
	if (flag_ == 0)
	{
		// �ړ�
		if (SetMove(*ctrl, Input::GAMEPAD_LEFT, DIK_X, Input::GAMEPAD_RIGHT, DIK_C))
		{
			// �w�i�����Ɉ������΂�
			back_.size = SIZE_BACK_SELECT;

			// UI�̕`��
			for (int i = 0; i < static_cast<int>(BackItem::EMPTY); ++i)
			{
				backItemBack_[i].enable = true;
			}

			// �{�^��UI
			SetButtonUIEnable(false, true, true);
		}
	}
	// �ړ���
	else
	{
		// �A�C�e���̈ړ�
		for (auto& ui : backItemBack_) { ui.position.x += MOVE_LIST * flag_; }
		cnt_++;	

		// �����̃A�C�e���̃T�C�Y������������
		backItemBack_[static_cast<int>(BackItem::Center)].size -= ITEM_SIZE_DIFF;

		// ���ɒ����ɍs���A�C�e���̃T�C�Y��傫��
		int num = static_cast<int>((flag_ > 0) ? BackItem::FrontLeft : BackItem::FrontRight);
		backItemBack_[num].size += ITEM_SIZE_DIFF;

		// ���̃t���[���ȏ�ړ�������
		if(cnt_ >= CHANGE_FRAME)
		{
			// �E�ړ�������
			if (flag_ > 0)
			{
				// �ύX�����ʒu�����X�g�ɍX�V
				for (int i = 0; i < static_cast<int>(BackItem::EMPTY); ++i)
				{
					backItemBack_[i].position = backItemBack_[i + 1].position;
				}
			}
			// ���ړ�������
			else
			{
				// �ύX�����ʒu�����X�g�ɍX�V
				for (int i = static_cast<int>(BackItem::BackLeft); i > 0; --i)
				{
					backItemBack_[i].position = backItemBack_[i - 1].position;
				}
				// �t���Ȃ̂ł��ꂾ�����܂������Ȃ�
				backItemBack_[static_cast<int>(BackItem::BackRight)].position = backItemBack_[static_cast<int>(BackItem::EMPTY)].position;
			}

			// �A�C�e���̕`�揇�X�V
			SetItemBack();

			// �t���O�̏�����
			flag_	= 0;
			cnt_	= 0;
		}
	}
}

/* @fn		SetItemBack
 * @brief	�A�C�e���̔w�i�̐ݒ�
 * @sa		Update()
 * @param	�Ȃ�
 * @return	�Ȃ�				*/
void ItemList::SetItemBack(void)
{
	for (int i = 0; i < static_cast<int>(BackItem::EMPTY); ++i)
	{
		// �D��x�̍X�V 3 4 5 4 3�̏���
		int		adjust = max(0, i - 2);
		int		priority = 3 + i - (2 * adjust);
		auto&	ui = backItemBack_[i];

		ui.SetPriority(static_cast<uint8>(PRIORITY + priority));
		// �^�񒆂����傫�����Ⴄ
		ui.size = (i == static_cast<int>(BackItem::Center)) ? SIZE_ITEM_BACK : SIZE_ITEM_BACK_LIST;
	}
	// �ړ��p�ꎞ�I�u�W�F�N�g�͔�\��
	backItemBack_[static_cast<int>(BackItem::EMPTY)].enable = false;
}

/* @fn		SetMove
 * @brief	�A�C�e���ړ��̊J�n
 * @sa		Update()
 * @param	(lpad)	���ړ��̃Q�[���p�b�h
 * @param	(lkey)	���ړ��̃L�[
 * @param	(rpad)	�E�ړ��̃Q�[���p�b�h
 * @param	(rkey)	�E�ړ��̃L�[
 * @return	�ړ��J�n������true	*/
bool ItemList::SetMove(Controller& ctrl, WORD lpad, int lkey, WORD rpad, int rkey)
{
	// �L�[����
	int key = 0;
	if (ctrl.Trigger(rpad, rkey))		{ key = 1;  }
	else if (ctrl.Trigger(lpad, lkey))  { key = -1; }

	if (key)
	{
		// �ړ��ꎞ�I�u�W�F�N�g�̕`��
		auto& empty			= backItemBack_[static_cast<int>(BackItem::EMPTY)];
		empty.enable		= true;
		empty.position.x	= POSITION.x - ((ITEM_RANGE * 3) * key);
		// �t���O���̐ݒ�
		cnt_	= 0;
		flag_	= key;

		return true;
	}
	return false;
}

/* @fn		SetButtonUIEnable
 * @brief	�{�^��UI��Enable�ύX
 * @sa		Update()
 * @param	(l)			LUI
 * @param	(maru)		�ZUI
 * @param	(shikaku)	��UI
 * @param	�Ȃ�				*/
void ItemList::SetButtonUIEnable(bool l, bool maru, bool shikaku)
{
	ui_[static_cast<int>(ButtonUI::L)].enable		= l;
	ui_[static_cast<int>(ButtonUI::MARU)].enable	= maru;
	ui_[static_cast<int>(ButtonUI::SHIKAKU)].enable = shikaku;
}

/* @fn		GuiUpdate
 * @brief	Gui�̍X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�				*/
void ItemList::GuiUpdate(void)
{
	const auto& debug = Systems::Instance()->GetDebug();
	ImGui::Text("BackRight  : ");
	ImGui::SameLine();
	ImGui::Text(debug->BoolToString(backItemBack_[0].enable).c_str());
	ImGui::SameLine();
	ImGui::Text(" : %.2f", backItemBack_[0].position.x);
	ImGui::Text("FrontRight : ");
	ImGui::SameLine();
	ImGui::Text(debug->BoolToString(backItemBack_[1].enable).c_str());
	ImGui::SameLine();
	ImGui::Text(" : %.2f", backItemBack_[1].position.x);
	ImGui::Text("Center     : ");
	ImGui::SameLine();
	ImGui::Text(debug->BoolToString(backItemBack_[2].enable).c_str());
	ImGui::SameLine();
	ImGui::Text(" : %.2f", backItemBack_[2].position.x);
	ImGui::Text("FrontLeft  : ");
	ImGui::SameLine();
	ImGui::Text(debug->BoolToString(backItemBack_[3].enable).c_str());
	ImGui::SameLine();
	ImGui::Text(" : %.2f", backItemBack_[3].position.x);
	ImGui::Text("BackLeft   : ");
	ImGui::SameLine();
	ImGui::Text(debug->BoolToString(backItemBack_[4].enable).c_str());
	ImGui::SameLine();
	ImGui::Text(" : %.2f", backItemBack_[4].position.x);
	ImGui::Text("Empty      : ");
	ImGui::SameLine();
	ImGui::Text(debug->BoolToString(backItemBack_[5].enable).c_str());
	ImGui::SameLine();
	ImGui::Text(" : %.2f", backItemBack_[5].position.x);
}

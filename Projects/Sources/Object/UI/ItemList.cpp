#include "ItemList.h"
#include <FrameWork/Windows/Windows.h>

//! @def	�`�揇
static constexpr int PRIORITY		= 100;

//! @def	�ʒu
static const	 VECTOR2 POSITION = VECTOR2(Windows::WIDTH - 250.0f, Windows::HEIGHT - 100.0f);
//! @def	�w�i�`��T�C�Y
static const	 VECTOR2 SIZE_BACK = VECTOR2(300, 70);
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

ItemList::ItemList(void) : Object(ObjectTag::UI), GUI(Systems::Instance(), this, "ItemList")
	, player_(nullptr)
	, flag_(0)
	, cnt_(0)
{
}

ItemList::~ItemList(void)
{
}

void ItemList::Init(void)
{
	int texNum = static_cast<int>(Resources::Texture::Camp::ITEM_UI);

	// �w�i
	back_.Init(PRIORITY, texNum);
	back_.SetPosition(POSITION);
	back_.SetSize(SIZE_BACK);
	back_.SetSplit(VECTOR2(1, 4));

	for (int i = 0; i < static_cast<int>(BackItem::MAX); ++i)
	{
		backItemBack_[i].Init(PRIORITY + 2, texNum);
		float x = POSITION.x + ((ITEM_RANGE * 2) - (i * ITEM_RANGE));
		backItemBack_[i].SetPosition(VECTOR2(x, POSITION.y));
		backItemBack_[i].SetSplit(VECTOR2(2, 4));
		backItemBack_[i].SetPattern(2);
		backItemBack_[i].SetSize(SIZE_ITEM_BACK_LIST);
	}
	SetItemBack();

	// �A�C�e���̖��O
	itemName_.Init(PRIORITY + 2, texNum);
	itemName_.SetPosition(POSITION_ITEM_NAME);
	itemName_.SetSize(SIZE_BACK);
	itemName_.SetSplit(VECTOR2(1, 8));
	itemName_.SetPattern(4);

	// �{�^��UI
	int button = static_cast<int>(ButtonUI::L);
	ui_[button].Init(PRIORITY + 1, texNum);
	ui_[button].SetPosition(POSITION_L);
	ui_[button].SetSize(SIZE_L);
	ui_[button].SetSplit(VECTOR2(2, 8));
	ui_[button].SetPattern(5);

	button = static_cast<int>(ButtonUI::MARU);
	ui_[button].Init(PRIORITY + 5, texNum);
	ui_[button].SetPosition(VECTOR2(POSITION.x + (ITEM_RANGE * 3), POSITION.y + 30));
	ui_[button].SetSize(VECTOR2(60, 50));
	ui_[button].SetSplit(VECTOR2(4, 8));
	ui_[button].SetPattern(15);
	ui_[button].SetEnable(false);
	
	button = static_cast<int>(ButtonUI::SHIKAKU);
	ui_[button].Init(PRIORITY + 5, texNum);
	ui_[button].SetPosition(VECTOR2(POSITION.x - (ITEM_RANGE * 3), POSITION.y + 30));
	ui_[button].SetSize(VECTOR2(60, 50));
	ui_[button].SetSplit(VECTOR2(4, 8));
	ui_[button].SetPattern(14);
	ui_[button].SetEnable(false);
}

void ItemList::Uninit(void)
{
	for (auto& c : ui_) { c.Uninit(); }
	for (auto& c : backItemBack_) { c.Uninit(); }
	itemName_.Uninit();
	back_.Uninit();
}

void ItemList::Update(void)
{
	if (!player_) { return; }
	const auto& ctrl = GetCtrl(0);
	if (!ctrl) { return; }

	JudgeCtrl(*ctrl);

	// L����
	if (ctrl->Press(Input::GAMEPAD_L1, DIK_R))
	{
		// �������ɔw�i�����Ɉ������΂�
		back_.SetSize(SIZE_BACK_SELECT);

		// ���A�C�e���̕`��
		for (int i = 0; i < static_cast<int>(BackItem::EMPTY); ++i)
		{
			backItemBack_[i].SetEnable(true);
		}

		// �{�^��UI�̕\���ύX
		SetButtonUIEnable(false, true, true);

		// �ړ����Ă��Ȃ���
		if (flag_ == 0)
		{
			// �ړ�
			SetMove(*ctrl, Input::GAMEPAD_SQUARE, DIK_T, Input::GAMEPAD_CIRCLE, DIK_Y);
		}
	}
	// L�񉟉���
	else
	{
		// �ړ����Ă��Ȃ���
		if (flag_ == 0)
		{
			// �w�i�̃T�C�Y��߂�
			back_.SetSize(SIZE_BACK);

			// �A�C�e���̕\��������
			for (auto& ui : backItemBack_) { ui.SetEnable(false); }
			// ���������`��
			for (int i = static_cast<int>(BackItem::FrontRight); i <= static_cast<int>(BackItem::FrontLeft); ++i)
			{
				backItemBack_[i].SetEnable(true);
			}

			// �{�^��UI�̕\���ύX
			SetButtonUIEnable(true, false, false);
		}
	}

	// �ړ����Ă��Ȃ���
	if (flag_ == 0)
	{
		// �ړ�
		if (SetMove(*ctrl, Input::GAMEPAD_LEFT, DIK_Q, Input::GAMEPAD_RIGHT, DIK_E))
		{
			// �w�i�����Ɉ������΂�
			back_.SetSize(SIZE_BACK_SELECT);

			// UI�̕`��
			for (int i = 0; i < static_cast<int>(BackItem::EMPTY); ++i)
			{
				backItemBack_[i].SetEnable(true);
			}

			// �{�^��UI
			SetButtonUIEnable(false, true, true);
		}
	}
	// �ړ���
	else
	{
		// �A�C�e���̈ړ�
		for (auto& ui : backItemBack_) 
		{
			auto pos = ui.GetPosition();
			pos.x += MOVE_LIST * flag_;
			ui.SetPosition(pos);
		}
		cnt_++;	

		// �����̃A�C�e���̃T�C�Y������������
		auto size = backItemBack_[static_cast<int>(BackItem::Center)].GetSize();
		size -= ITEM_SIZE_DIFF;
		backItemBack_[static_cast<int>(BackItem::Center)].SetSize(size);

		// ���ɒ����ɍs���A�C�e���̃T�C�Y��傫��
		int num = static_cast<int>((flag_ > 0) ? BackItem::FrontLeft : BackItem::FrontRight);
		size = backItemBack_[num].GetSize();
		size += ITEM_SIZE_DIFF;
		backItemBack_[num].SetSize(size);

		// ���̃t���[���ȏ�ړ�������
		if(cnt_ >= CHANGE_FRAME)
		{
			// �E�ړ�������
			if (flag_ > 0)
			{
				// �ύX�����ʒu�����X�g�ɍX�V
				for (int i = 0; i < static_cast<int>(BackItem::EMPTY); ++i)
				{
					backItemBack_[i].SetPosition(backItemBack_[i + 1].GetPosition());
				}
			}
			// ���ړ�������
			else
			{
				// �ύX�����ʒu�����X�g�ɍX�V
				for (int i = static_cast<int>(BackItem::BackLeft); i > 0; --i)
				{
					backItemBack_[i].SetPosition(backItemBack_[i - 1].GetPosition());
				}
				// �t���Ȃ̂ł��ꂾ�����܂������Ȃ�
				backItemBack_[static_cast<int>(BackItem::BackRight)].SetPosition(backItemBack_[static_cast<int>(BackItem::EMPTY)].GetPosition());
			}

			// �A�C�e���̕`�揇�X�V
			SetItemBack();

			// �t���O�̏�����
			flag_	= 0;
			cnt_	= 0;
		}
	}
}

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
		ui.SetSize((i == static_cast<int>(BackItem::Center)) ? SIZE_ITEM_BACK : SIZE_ITEM_BACK_LIST);
	}
	// �ړ��p�ꎞ�I�u�W�F�N�g�͔�\��
	backItemBack_[static_cast<int>(BackItem::EMPTY)].SetEnable(false);
}

bool ItemList::SetMove(Controller& ctrl, WORD lpad, int lkey, WORD rpad, int rkey)
{
	// �L�[����
	int8 key = 0;
	if (ctrl.Trigger(rpad, rkey))		{ key = -1; }
	else if (ctrl.Trigger(lpad, lkey))  { key =  1; }

	if (key)
	{
		// �ړ��ꎞ�I�u�W�F�N�g�̕`��
		auto& empty	= backItemBack_[static_cast<int>(BackItem::EMPTY)];
		empty.SetEnable(true);
		auto pos = empty.GetPosition();
		pos.x = POSITION.x - ((ITEM_RANGE * 3) * key);
		empty.SetPosition(pos);
		// �t���O���̐ݒ�
		cnt_	= 0;
		flag_	= key;

		if (lpad == Input::GAMEPAD_LEFT)
		{
			VECTOR2 p = (ctrl.GetCtrlNum() == Controller::CtrlNum::Key) ? VECTOR2(30, 31) : VECTOR2(28, 29);
			ui_[static_cast<int>(ButtonUI::MARU)].SetPattern(p.y);
			ui_[static_cast<int>(ButtonUI::SHIKAKU)].SetPattern(p.x);
		}

		return true;
	}
	return false;
}

void ItemList::SetButtonUIEnable(bool l, bool maru, bool shikaku)
{
	ui_[static_cast<int>(ButtonUI::L)].SetEnable(l);
	ui_[static_cast<int>(ButtonUI::MARU)].SetEnable(maru);
	ui_[static_cast<int>(ButtonUI::SHIKAKU)].SetEnable(shikaku);
}

void ItemList::JudgeCtrl(Controller& ctrl)
{
	if (flag_) { return; }

	const auto& type = ctrl.GetCtrlNum();
	int button[] = { static_cast<int>(ButtonUI::L),  static_cast<int>(ButtonUI::MARU) , static_cast<int>(ButtonUI::SHIKAKU) };

	if (type == Controller::CtrlNum::PS4)
	{
		if (ui_[button[1]].GetPattern() == 15) { return; }

		ui_[button[0]].SetPattern(5);
		ui_[button[1]].SetPattern(15);
		ui_[button[2]].SetPattern(14);
	}
	else if (type == Controller::CtrlNum::X)
	{
		if (ui_[button[1]].GetPattern() == 25) { return; }

		ui_[button[0]].SetPattern(10);
		ui_[button[1]].SetPattern(25);
		ui_[button[2]].SetPattern(24);
	}
	else if (type == Controller::CtrlNum::Key)
	{
		if (ui_[button[1]].GetPattern() == 27) { return; }

		ui_[button[0]].SetPattern(11);
		ui_[button[1]].SetPattern(27);
		ui_[button[2]].SetPattern(26);
	}
}

void ItemList::GuiUpdate(void)
{
	const auto& debug = Systems::Instance()->GetDebug();
	ImGui::Text("BackRight  : ");
	ImGui::SameLine();
	ImGui::Text(debug->BoolToString(backItemBack_[0].GetEnable()).c_str());
	ImGui::SameLine();
	ImGui::Text(" : %.2f", backItemBack_[0].GetPosition().x);
	ImGui::Text("FrontRight : ");
	ImGui::SameLine();
	ImGui::Text(debug->BoolToString(backItemBack_[1].GetEnable()).c_str());
	ImGui::SameLine();
	ImGui::Text(" : %.2f", backItemBack_[1].GetPosition().x);
	ImGui::Text("Center     : ");
	ImGui::SameLine();
	ImGui::Text(debug->BoolToString(backItemBack_[2].GetEnable()).c_str());
	ImGui::SameLine();
	ImGui::Text(" : %.2f", backItemBack_[2].GetPosition().x);
	ImGui::Text("FrontLeft  : ");
	ImGui::SameLine();
	ImGui::Text(debug->BoolToString(backItemBack_[3].GetEnable()).c_str());
	ImGui::SameLine();
	ImGui::Text(" : %.2f", backItemBack_[3].GetPosition().x);
	ImGui::Text("BackLeft   : ");
	ImGui::SameLine();
	ImGui::Text(debug->BoolToString(backItemBack_[4].GetEnable()).c_str());
	ImGui::SameLine();
	ImGui::Text(" : %.2f", backItemBack_[4].GetPosition().x);
	ImGui::Text("Empty      : ");
	ImGui::SameLine();
	ImGui::Text(debug->BoolToString(backItemBack_[5].GetEnable()).c_str());
	ImGui::SameLine();
	ImGui::Text(" : %.2f", backItemBack_[5].GetPosition().x);
}

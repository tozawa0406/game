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
	, list_(nullptr)
{
	for (auto& item : item_)
	{
		item.info.itemID = ItemID::UNKNOWN;
		item.info.possession = 0;
		item.arrayNum = 0;
	}
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
		item_[i].back.Init(PRIORITY + 2, texNum);
		float x = POSITION.x + ((ITEM_RANGE * 2) - (i * ITEM_RANGE));
		item_[i].back.SetPosition(VECTOR2(x, POSITION.y));
		item_[i].back.SetSplit(VECTOR2(2, 4));
		item_[i].back.SetPattern(2);
		item_[i].back.SetSize(SIZE_ITEM_BACK_LIST);

		item_[i].img.Init(PRIORITY + 3, static_cast<int>(Resources::Texture::Base::WHITE));
		item_[i].img.SetPosition(VECTOR2(x, POSITION.y));
		item_[i].img.SetSize(VECTOR2(40));
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
	ui_[button].Init(PRIORITY + 10, texNum);
	ui_[button].SetPosition(POSITION_L);
	ui_[button].SetSize(SIZE_L);
	ui_[button].SetSplit(VECTOR2(2, 8));
	ui_[button].SetPattern(5);

	button = static_cast<int>(ButtonUI::MARU);
	ui_[button].Init(PRIORITY + 10, texNum);
	ui_[button].SetPosition(VECTOR2(POSITION.x + (ITEM_RANGE * 3), POSITION.y + 30));
	ui_[button].SetSize(VECTOR2(60, 50));
	ui_[button].SetSplit(VECTOR2(4, 8));
	ui_[button].SetPattern(15);
	ui_[button].SetEnable(false);
	
	button = static_cast<int>(ButtonUI::SHIKAKU);
	ui_[button].Init(PRIORITY + 10, texNum);
	ui_[button].SetPosition(VECTOR2(POSITION.x - (ITEM_RANGE * 3), POSITION.y + 30));
	ui_[button].SetSize(VECTOR2(60, 50));
	ui_[button].SetSplit(VECTOR2(4, 8));
	ui_[button].SetPattern(14);
	ui_[button].SetEnable(false);

	float size = 25;
	text_.Init(PRIORITY + 7, "�e�X�g", static_cast<int>(size));
	text_.SetPosition(VECTOR2(POSITION_ITEM_NAME.x - (size + Half(size)), POSITION_ITEM_NAME.y - Half(size)));

	list_ = new PlayerItemList;
	if (list_)
	{
		list_->Init();
		ITEM_LIST add;
		add.itemID = ItemID::Rations;
		add.possession = 10;
		list_->AddItem(add);
	}
	
	GetItemInfo(item_[static_cast<int>(BackItem::Center)]		, 0);
	SearchNextItem();
}

void ItemList::Uninit(void)
{
	DeletePtr(list_);
	text_.Uninit();
	for (auto& i : item_) 
	{
		i.img.Uninit();
		i.back.Uninit();
	}
	for (auto& c : ui_) { c.Uninit(); }
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
			auto& item = item_[i];
			item.back.SetEnable(true);
			item.img.SetEnable(true);
			if (item.info.itemID == ItemID::UNKNOWN)
			{
				item.img.SetEnable(false);
			}
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
			for (auto& ui : item_) 
			{
				ui.back.SetEnable(false);
				ui.img.SetEnable(false); 
			}
			// ���������`��
			for (int i = static_cast<int>(BackItem::FrontRight); i <= static_cast<int>(BackItem::FrontLeft); ++i)
			{
				auto& item = item_[i];
				item.back.SetEnable(true);
				item.img.SetEnable(true);
				if (item.info.itemID == ItemID::UNKNOWN)
				{
					item.img.SetEnable(false);
				}
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
				auto& item = item_[i];
				item.back.SetEnable(true);
				item.img.SetEnable(true);
				if (item.info.itemID == ItemID::UNKNOWN)
				{
					item.img.SetEnable(false);
				}
			}

			// �{�^��UI
			SetButtonUIEnable(false, true, true);
		}
	}
	// �ړ���
	else
	{
		// �A�C�e���̈ړ�
		for (int i = 0;i < static_cast<int>(BackItem::MAX);++i) 
		{
			auto& item = item_[i];
			auto pos = item.back.GetPosition();
			pos.x += MOVE_LIST * flag_;
			item.back.SetPosition(pos);
			item.img.SetPosition(pos);
		}
		cnt_++;	

		// �����̃A�C�e���̃T�C�Y������������
		auto& center = item_[static_cast<int>(BackItem::Center)];
		auto size = center.back.GetSize();
		size -= ITEM_SIZE_DIFF;
		center.back.SetSize(size);
		size = center.img.GetSize();
		size -= 1;
		center.img.SetSize(size);		

		// ���ɒ����ɍs���A�C�e���̃T�C�Y��傫��
		int num = static_cast<int>((flag_ > 0) ? BackItem::FrontLeft : BackItem::FrontRight);
		auto& nextCenter = item_[num];
		size = nextCenter.back.GetSize();
		size += ITEM_SIZE_DIFF;
		nextCenter.back.SetSize(size);
		size = nextCenter.img.GetSize();
		size += 1;
		nextCenter.img.SetSize(size);

		// ���̃t���[���ȏ�ړ�������
		if(cnt_ >= CHANGE_FRAME)
		{
			// �E�ړ�������
			if (flag_ > 0)
			{
				// �ύX�����ʒu�����X�g�ɍX�V
				for (int i = 0; i < static_cast<int>(BackItem::EMPTY); ++i)
				{
					auto& item = item_[i];
					auto& next = item_[i + 1];
					item.back.SetPosition(next.back.GetPosition());
					item.img.SetPosition(next.img.GetPosition());
					GetItemInfo(item, next.arrayNum);
				}
			}
			// ���ړ�������
			else
			{
				// �ύX�����ʒu�����X�g�ɍX�V
				for (int i = static_cast<int>(BackItem::BackLeft); i > 0; --i)
				{
					auto& item = item_[i];
					auto& next = item_[i - 1];
					item.back.SetPosition(next.back.GetPosition());
					item.img.SetPosition(next.img.GetPosition());
					item.arrayNum = next.arrayNum;
					GetItemInfo(item, item.arrayNum);
				}
				auto& item = item_[static_cast<int>(BackItem::BackRight)];
				auto& next = item_[static_cast<int>(BackItem::EMPTY)];
				// �t���Ȃ̂ł��ꂾ�����܂������Ȃ�
				item.back.SetPosition(next.back.GetPosition());
				item.img.SetPosition(next.img.GetPosition());
				item.arrayNum = next.arrayNum;
				GetItemInfo(item, item.arrayNum);
			}
			SearchNextItem();

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
		// �D��x�̍X�V 3 5 7 5 3�̏���
		int		adjust = max(0, i - 2);
		int		priority = 3 + (i * 2) - (4 * adjust);
		auto&	ui = item_[i];

		ui.back.SetPriority(static_cast<uint8>(PRIORITY + priority));
		ui.img.SetPriority(static_cast<uint8>(PRIORITY + priority + 1));
		// �^�񒆂����傫�����Ⴄ
		ui.back.SetSize((i == static_cast<int>(BackItem::Center)) ? SIZE_ITEM_BACK : SIZE_ITEM_BACK_LIST);
		ui.img.SetSize(VECTOR2((i == static_cast<int>(BackItem::Center)) ? 50 : 50 * 0.8f));

		if (ui.info.itemID == ItemID::UNKNOWN)
		{
			ui.img.SetEnable(false);
		}
	}
	auto& empty = item_[static_cast<int>(BackItem::EMPTY)];
	// �ړ��p�ꎞ�I�u�W�F�N�g�͔�\��
	empty.back.SetEnable(false);
	empty.img.SetEnable(false);
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
		auto& empty = item_[static_cast<int>(BackItem::EMPTY)];
		empty.back.SetEnable(true);
		empty.img.SetEnable(true);
		auto pos = empty.back.GetPosition();
		pos.x = POSITION.x - ((ITEM_RANGE * 3) * key);
		empty.back.SetPosition(pos);
		empty.img.SetPosition(pos);

		int arrayNum = PlayerItemList::MAX_ITEM;
		if (key < 0)
		{
			arrayNum = item_[static_cast<int>(BackItem::BackRight)].arrayNum + 1;
			if (arrayNum >= PlayerItemList::MAX_ITEM) { arrayNum = 0; }
		}
		else 
		{
			arrayNum = item_[static_cast<int>(BackItem::BackLeft)].arrayNum - 1;
			if (arrayNum < 0) { arrayNum = PlayerItemList::MAX_ITEM - 1; }
		}
		GetItemInfo(empty, arrayNum);	
		if (empty.info.itemID == ItemID::UNKNOWN)
		{
			empty.img.SetEnable(false);
		}

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

void ItemList::GetItemInfo(UI_ITEM_LIST& list, int arrayNum)
{
	if (arrayNum < 0 &&  PlayerItemList::MAX_ITEM >= arrayNum) { return; }
	list.arrayNum = static_cast<uint8>(arrayNum);
	if (list_)
	{
		list.info = list_->GetItemInfo(arrayNum);
		if (list.info.itemID != ItemID::UNKNOWN)
		{
			ItemInfo itemInfo;
			int arrayMax = sizeof(itemInfo.info) / sizeof(itemInfo.info[0]);
			for (int i = 0; i < arrayMax; ++i)
			{
				if (list.info.itemID == itemInfo.info[i].id)
				{
					list.img.SetTexNum(itemInfo.info[i].texNum);
					break;
				}
			}
			list.img.SetEnable(true);
		}
	}
}

void ItemList::SearchNextItem(void)
{
	uint8 center = item_[static_cast<int>(BackItem::Center)].arrayNum;

	// �E�͍~��
	for (int i = center + 1;; ++i)
	{
		if (i >= PlayerItemList::MAX_ITEM) { i = 0; }
		if (FindNext(i, BackItem::FrontRight)) { break; }
	}
	for (int i = center + 2;; ++i)
	{
		if (i >= PlayerItemList::MAX_ITEM) { i = 0; }
		if (FindNext(i, BackItem::BackRight)) { break; }
	}

	// ���͏���
	for (int i = center - 1;; --i)
	{
		if (i < 0) { i = PlayerItemList::MAX_ITEM - 1; }
		if (FindNext(i, BackItem::FrontLeft)) { break; }
	}
	for (int i = center - 2;; --i)
	{
		if (i < 0) { i = PlayerItemList::MAX_ITEM - 1; }
		if (FindNext(i, BackItem::BackLeft)) 
		{
			// �d������\��������̂ŏ���
			if (i == item_[static_cast<int>(BackItem::FrontLeft)].arrayNum)
			{
				int minus = i - 1;
				GetItemInfo(item_[static_cast<int>(BackItem::BackLeft)], (minus < 0) ? PlayerItemList::MAX_ITEM + minus : minus);
			}
			else if (i == center)
			{
				int minus = i - 2;
				GetItemInfo(item_[static_cast<int>(BackItem::BackLeft)], (minus < 0) ? PlayerItemList::MAX_ITEM + minus : minus);
			}
			break; 
		}
	}
}

bool ItemList::FindNext(int i, BackItem arrangement)
{
	const auto& info = list_->GetItemInfo(i);
	// 0�����łȂ���
	if (i == 0 || info.itemID != ItemID::UNKNOWN)
	{
		GetItemInfo(item_[static_cast<int>(arrangement)], i);
		return true;
	}
	return false;
}

void ItemList::GuiUpdate(void)
{
	if (list_)
	{
		list_->GuiUpdate();
	}
}

#include "ItemList.h"
#include <FrameWork/Windows/Windows.h>

//! @def	描画順
static constexpr int PRIORITY		= 100;

//! @def	位置
static const	 VECTOR2 POSITION = VECTOR2(Windows::WIDTH - 250.0f, Windows::HEIGHT - 100.0f);
//! @def	背景描画サイズ
static const	 VECTOR2 SIZE_BACK = VECTOR2(300, 100);
//! @def	選択時の背景描画サイズ
static const	 VECTOR2 SIZE_BACK_SELECT = VECTOR2(350, 100);
//! @def	中央のアイテムの背景
static const	 VECTOR2 SIZE_ITEM_BACK = VECTOR2(125, 100);
//! @def	選択時の中央のアイテムの背景
static const	 VECTOR2 SIZE_ITEM_BACK_SELECT = VECTOR2(150, 100);
//! @def	アイテムの名前
static const	 VECTOR2 POSITION_ITEM_NAME = VECTOR2(POSITION.x, POSITION.y + 60);
//! @def	LボタンUI位置
static const	 VECTOR2 POSITION_L = VECTOR2(POSITION.x - 90, POSITION.y);
//! @def	LボタンUIサイズ
static const	 VECTOR2 SIZE_L = VECTOR2(100, 40);

/* @fn		コンストラクタ
 * @brief	変数の初期化		*/
ItemList::ItemList(void) : Object(Object::Tag::UI)
	, player_(nullptr)
	, flag_(0)
	, cnt_(0)
{
}

/* @fn		デストラクタ
 * @brief	...					*/
ItemList::~ItemList(void)
{
}

/* @fn		Init
 * @brief	初期化処理
 * @param	なし
 * @return	なし				*/
void ItemList::Init(void)
{
	const auto& systems = Systems::Instance();
	int texNum = static_cast<int>(Texture::Game::ITEM_UI);

	// 背景
	back_.Init(systems, PRIORITY, texNum);
	back_.position	= POSITION;
	back_.size		= SIZE_BACK;
	back_.split		= VECTOR2(1, 3);

	//// 中央のアイテム
	//itemBack_.Init(systems, PRIORITY + 5, texNum);
	//itemBack_.position	= POSITION;
	//itemBack_.size		= SIZE_ITEM_BACK;
	//itemBack_.split		= VECTOR2(2, 3);
	//itemBack_.pattern	= 2;

	for (int i = 0; i < 6; ++i)
	{
		backItemBack_[i].Init(systems, PRIORITY + 2, texNum);
		float x = POSITION.x + (80 - (i * 40));
		backItemBack_[i].position	= VECTOR2(x, POSITION.y);
		backItemBack_[i].size		= VECTOR2(SIZE_ITEM_BACK.x * 0.8f, SIZE_ITEM_BACK.y * 0.8f);
		backItemBack_[i].split		= VECTOR2(2, 3);
		backItemBack_[i].pattern	= 2;
	}
	backItemBack_[static_cast<int>(BackItem::Center)].SetPriority(PRIORITY + 5);
	backItemBack_[static_cast<int>(BackItem::Center)].size = SIZE_ITEM_BACK;
	backItemBack_[static_cast<int>(BackItem::FrontLeft)].SetPriority(PRIORITY + 4);
	backItemBack_[static_cast<int>(BackItem::FrontRight)].SetPriority(PRIORITY + 4);
	backItemBack_[static_cast<int>(BackItem::BackLeft)].SetPriority(PRIORITY + 3);
	backItemBack_[static_cast<int>(BackItem::BackRight)].SetPriority(PRIORITY + 3);
	backItemBack_[static_cast<int>(BackItem::EMPTY)].enable = false;

	// アイテムの名前
	itemName_.Init(systems, PRIORITY + 2, texNum);
	itemName_.position	= POSITION_ITEM_NAME;
	itemName_.size		= SIZE_BACK;
	itemName_.split		= VECTOR2(1, 3);
	itemName_.pattern	= 2;

	// ボタンUI
	int button = static_cast<int>(ButtonUI::L);
	ui_[button].Init(systems, PRIORITY + 1, texNum);
	ui_[button].position	= POSITION_L;
	ui_[button].size		= SIZE_L;
	ui_[button].split		= VECTOR2(2, 6);
	ui_[button].pattern		= 5;

	button = static_cast<int>(ButtonUI::MARU);
	ui_[button].Init(systems, PRIORITY + 1, texNum);
	ui_[button].position	= VECTOR2(POSITION.x + 120, POSITION.y + 30);
	ui_[button].size		= VECTOR2(60, 50);
	ui_[button].split		= VECTOR2(4, 6);
	ui_[button].pattern		= 15;
	ui_[button].enable		= false;
	
	button = static_cast<int>(ButtonUI::SHIKAKU);
	ui_[button].Init(systems, PRIORITY + 1, texNum);
	ui_[button].position	= VECTOR2(POSITION.x - 120, POSITION.y + 30);
	ui_[button].size		= VECTOR2(60, 50);
	ui_[button].split		= VECTOR2(4, 6);
	ui_[button].pattern		= 14;
	ui_[button].enable		= false;

}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし				*/
void ItemList::Uninit(void)
{
}

/* @fn		Update
 * @brief	更新処理
 * @param	なし
 * @return	なし				*/
void ItemList::Update(void)
{
	if (!player_) { return; }
	const auto& ctrl = GetCtrl(0);
	if (!ctrl) { return; }

	if (ctrl->Press(Input::GAMEPAD_L1, DIK_Z))
	{
		back_.size = SIZE_BACK_SELECT;

		for (int i = 0; i < static_cast<int>(BackItem::EMPTY); ++i)
		{
			backItemBack_[i].enable = true;
		}

		ui_[static_cast<int>(ButtonUI::L)].enable		= false;
		ui_[static_cast<int>(ButtonUI::MARU)].enable	= true;
		ui_[static_cast<int>(ButtonUI::SHIKAKU)].enable = true;

		if (flag_ == 0)
		{
			if (ctrl->Trigger(Input::GAMEPAD_CIRCLE, DIK_K))
			{
				auto& empty = backItemBack_[static_cast<int>(BackItem::EMPTY)];
				empty.enable = true;
				empty.position.x = POSITION.x - 120;
				cnt_ = 0;
				flag_ = 1;
			}
			if (ctrl->Trigger(Input::GAMEPAD_SQUARE, DIK_H))
			{
				auto& empty = backItemBack_[static_cast<int>(BackItem::EMPTY)];
				empty.enable = true;
				empty.position.x = POSITION.x + 120;

				cnt_ = 0;
				flag_ = -1;
			}
		}

	}
	else
	{
		back_.size = SIZE_BACK;

		for (auto& ui : backItemBack_) { ui.enable = false; }
		backItemBack_[static_cast<int>(BackItem::Center)].enable = true;

		ui_[static_cast<int>(ButtonUI::L)].enable		= true;
		ui_[static_cast<int>(ButtonUI::MARU)].enable	= false;
		ui_[static_cast<int>(ButtonUI::SHIKAKU)].enable = false;
	}

	if (flag_)
	{
		for (auto& ui : backItemBack_) { ui.position.x += 4 * flag_; }
		cnt_++;	

		VECTOR2 diff = VECTOR2(SIZE_ITEM_BACK.x - SIZE_ITEM_BACK.x * 0.8f, SIZE_ITEM_BACK.y - SIZE_ITEM_BACK.y * 0.8f);
		diff *= 0.1f;
		backItemBack_[static_cast<int>(BackItem::Center)].size -= diff;

		int num = static_cast<int>((flag_ > 0) ? BackItem::FrontLeft : BackItem::FrontRight);
		backItemBack_[num].size += diff;

		if(cnt_ >= 10)
		{
			if (flag_ > 0)
			{
				for (int i = 0; i < 5; ++i)
				{
					backItemBack_[i].position = backItemBack_[i + 1].position;
				}
			}
			else
			{
				for (int i = 4; i > 0; --i)
				{
					backItemBack_[i].position = backItemBack_[i - 1].position;
				}
				backItemBack_[0].position = backItemBack_[5].position;
			}

			for (int i = 0; i < static_cast<int>(BackItem::EMPTY); ++i)
			{
				int		adjust		= max(0, i - 2);
				int		priority	= 3 + i - (2 * adjust);
				auto&	ui			= backItemBack_[i];

				ui.SetPriority(static_cast<uint8>(PRIORITY + priority));
				ui.size = (i == static_cast<int>(BackItem::Center)) ? SIZE_ITEM_BACK : VECTOR2(SIZE_ITEM_BACK.x * 0.8f, SIZE_ITEM_BACK.y * 0.8f);
			}
			backItemBack_[static_cast<int>(BackItem::EMPTY)].enable = false;

			flag_ = 0;
			cnt_ = 0;
		}
	}
}

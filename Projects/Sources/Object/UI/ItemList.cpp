#include "ItemList.h"
#include <FrameWork/Windows/Windows.h>

//! @def	描画順
static constexpr int PRIORITY		= 100;

//! @def	位置
static const	 VECTOR2 POSITION = VECTOR2(Windows::WIDTH - 250.0f, Windows::HEIGHT - 100.0f);
//! @def	背景描画サイズ
static const	 VECTOR2 SIZE_BACK = VECTOR2(300, 100);
//! @def	選択時の背景描画サイズ
static const	 VECTOR2 SIZE_BACK_SELECT = VECTOR2(370, 100);
//! @def	中央のアイテムの背景
static const	 VECTOR2 SIZE_ITEM_BACK = VECTOR2(125, 100);
//! @def	アイテム一覧の背景
static const	 VECTOR2 SIZE_ITEM_BACK_LIST = VECTOR2(SIZE_ITEM_BACK.x * 0.8f, SIZE_ITEM_BACK.y * 0.8f);
//! @def	アイテムの名前
static const	 VECTOR2 POSITION_ITEM_NAME = VECTOR2(POSITION.x, POSITION.y + 60);
//! @def	LボタンUI位置
static const	 VECTOR2 POSITION_L = VECTOR2(POSITION.x - 95, POSITION.y);
//! @def	LボタンUIサイズ
static const	 VECTOR2 SIZE_L = VECTOR2(100, 40);

//! @def	アイテムリストの距離
static constexpr int ITEM_RANGE = 40;
//! @def	アイテムの変更時間(フレーム)
static constexpr int CHANGE_FRAME = 10;
//! @def	アイテムの変更時の移動距離
static constexpr int MOVE_LIST = ITEM_RANGE / CHANGE_FRAME;
//! @def	アイテム移動時の大きさ変更
static const	 VECTOR2 ITEM_SIZE_DIFF = VECTOR2(SIZE_ITEM_BACK.x - SIZE_ITEM_BACK_LIST.x, SIZE_ITEM_BACK.y - SIZE_ITEM_BACK_LIST.y) * 0.1f;

/* @fn		コンストラクタ
 * @brief	変数の初期化		*/
ItemList::ItemList(void) : Object(Object::Tag::UI), GUI(Systems::Instance(), this, "ItemList")
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

	// L押下
	if (ctrl->Press(Input::GAMEPAD_L1, DIK_Z))
	{
		// 押下時に背景を横に引き延ばす
		back_.size = SIZE_BACK_SELECT;

		// 他アイテムの描画
		for (int i = 0; i < static_cast<int>(BackItem::EMPTY); ++i)
		{
			backItemBack_[i].enable = true;
		}

		// ボタンUIの表示変更
		SetButtonUIEnable(false, true, true);

		// 移動していない時
		if (flag_ == 0)
		{
			// 移動
			SetMove(*ctrl, Input::GAMEPAD_SQUARE, DIK_K, Input::GAMEPAD_CIRCLE, DIK_H);
		}
	}
	// L非押下時
	else
	{
		// 移動していない時
		if (flag_ == 0)
		{
			// 背景のサイズを戻す
			back_.size = SIZE_BACK;

			// アイテムの表示を消す
			for (auto& ui : backItemBack_) { ui.enable = false; }
			// 中央だけ描画
			for (int i = static_cast<int>(BackItem::FrontRight); i <= static_cast<int>(BackItem::FrontLeft); ++i)
			{
				backItemBack_[i].enable = true;
			}

			// ボタンUIの表示変更
			SetButtonUIEnable(true, false, false);
		}
	}

	// 移動していない時
	if (flag_ == 0)
	{
		// 移動
		if (SetMove(*ctrl, Input::GAMEPAD_LEFT, DIK_X, Input::GAMEPAD_RIGHT, DIK_C))
		{
			// 背景を横に引き延ばす
			back_.size = SIZE_BACK_SELECT;

			// UIの描画
			for (int i = 0; i < static_cast<int>(BackItem::EMPTY); ++i)
			{
				backItemBack_[i].enable = true;
			}

			// ボタンUI
			SetButtonUIEnable(false, true, true);
		}
	}
	// 移動中
	else
	{
		// アイテムの移動
		for (auto& ui : backItemBack_) { ui.position.x += MOVE_LIST * flag_; }
		cnt_++;	

		// 中央のアイテムのサイズを小さくする
		backItemBack_[static_cast<int>(BackItem::Center)].size -= ITEM_SIZE_DIFF;

		// 次に中央に行くアイテムのサイズを大きく
		int num = static_cast<int>((flag_ > 0) ? BackItem::FrontLeft : BackItem::FrontRight);
		backItemBack_[num].size += ITEM_SIZE_DIFF;

		// 一定のフレーム以上移動したら
		if(cnt_ >= CHANGE_FRAME)
		{
			// 右移動だった
			if (flag_ > 0)
			{
				// 変更した位置をリストに更新
				for (int i = 0; i < static_cast<int>(BackItem::EMPTY); ++i)
				{
					backItemBack_[i].position = backItemBack_[i + 1].position;
				}
			}
			// 左移動だった
			else
			{
				// 変更した位置をリストに更新
				for (int i = static_cast<int>(BackItem::BackLeft); i > 0; --i)
				{
					backItemBack_[i].position = backItemBack_[i - 1].position;
				}
				// 逆回りなのでこれだけうまくいかない
				backItemBack_[static_cast<int>(BackItem::BackRight)].position = backItemBack_[static_cast<int>(BackItem::EMPTY)].position;
			}

			// アイテムの描画順更新
			SetItemBack();

			// フラグの初期化
			flag_	= 0;
			cnt_	= 0;
		}
	}
}

/* @fn		SetItemBack
 * @brief	アイテムの背景の設定
 * @sa		Update()
 * @param	なし
 * @return	なし				*/
void ItemList::SetItemBack(void)
{
	for (int i = 0; i < static_cast<int>(BackItem::EMPTY); ++i)
	{
		// 優先度の更新 3 4 5 4 3の順番
		int		adjust = max(0, i - 2);
		int		priority = 3 + i - (2 * adjust);
		auto&	ui = backItemBack_[i];

		ui.SetPriority(static_cast<uint8>(PRIORITY + priority));
		// 真ん中だけ大きさが違う
		ui.size = (i == static_cast<int>(BackItem::Center)) ? SIZE_ITEM_BACK : SIZE_ITEM_BACK_LIST;
	}
	// 移動用一時オブジェクトは非表示
	backItemBack_[static_cast<int>(BackItem::EMPTY)].enable = false;
}

/* @fn		SetMove
 * @brief	アイテム移動の開始
 * @sa		Update()
 * @param	(lpad)	左移動のゲームパッド
 * @param	(lkey)	左移動のキー
 * @param	(rpad)	右移動のゲームパッド
 * @param	(rkey)	右移動のキー
 * @return	移動開始したらtrue	*/
bool ItemList::SetMove(Controller& ctrl, WORD lpad, int lkey, WORD rpad, int rkey)
{
	// キー入力
	int key = 0;
	if (ctrl.Trigger(rpad, rkey))		{ key = 1;  }
	else if (ctrl.Trigger(lpad, lkey))  { key = -1; }

	if (key)
	{
		// 移動一時オブジェクトの描画
		auto& empty			= backItemBack_[static_cast<int>(BackItem::EMPTY)];
		empty.enable		= true;
		empty.position.x	= POSITION.x - ((ITEM_RANGE * 3) * key);
		// フラグ等の設定
		cnt_	= 0;
		flag_	= key;

		return true;
	}
	return false;
}

/* @fn		SetButtonUIEnable
 * @brief	ボタンUIのEnable変更
 * @sa		Update()
 * @param	(l)			LUI
 * @param	(maru)		〇UI
 * @param	(shikaku)	□UI
 * @param	なし				*/
void ItemList::SetButtonUIEnable(bool l, bool maru, bool shikaku)
{
	ui_[static_cast<int>(ButtonUI::L)].enable		= l;
	ui_[static_cast<int>(ButtonUI::MARU)].enable	= maru;
	ui_[static_cast<int>(ButtonUI::SHIKAKU)].enable = shikaku;
}

/* @fn		GuiUpdate
 * @brief	Guiの更新処理
 * @param	なし
 * @return	なし				*/
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

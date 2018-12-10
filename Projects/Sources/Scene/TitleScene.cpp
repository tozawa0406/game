#include "TitleScene.h"
#include <FrameWork/Scene/SceneManager.h>
#include <FrameWork/Windows/Windows.h>

//! @def	UI描画準
static constexpr int UI_PRIORITY = 200;
//! @def	press描画の位置
static const VECTOR2 PRESS_POSITION = VECTOR2((float)Half(Windows::WIDTH) - 50, Windows::HEIGHT * 0.9f);
//! @def	pree描画のサイズ
static const VECTOR2 PRESS_SIZE = VECTOR2((float)Quarter(Windows::WIDTH), Half(Windows::HEIGHT * 0.1f));
//! @def	UIボタンの描画位置調整
static constexpr int ADJUST_POSITION_X = 190;
//! @def	UIボタンキーボードの描画位置調整
static constexpr int ADJUST_KEY_POSITION_X = 50;
//! @def	UIボタンキーボードの描画サイズ調整
static constexpr int ADJUST_KEY_SIZE_X = 4;

//! @def	点滅間隔
static constexpr float FLASHING_RANGE = 60;

/* @brief	コンストラクタ			*/
TitleScene::TitleScene(SceneManager* manager) : BaseScene(manager), GUI(manager->GetSystems(), nullptr, "SceneTitle")
	, frameCnt_(0)
{
}

/* @brief	デストラクタ			*/
TitleScene::~TitleScene(void)
{
}

/* @brief	初期化処理
 * @param	なし
 * @return	なし					*/
void TitleScene::Init(void)
{
	// 「please press」
	press_.Init(UI_PRIORITY, static_cast<int>(Texture::Title::PLEASE_PRESS));
	press_.SetPosition(PRESS_POSITION);
	press_.SetSize(PRESS_SIZE);

	// 各種ボタン
	for (auto& b : button_)
	{
		b.Init(UI_PRIORITY, static_cast<int>(Texture::Title::ENTER));
		b.SetPosition(VECTOR2(PRESS_POSITION.x + ADJUST_POSITION_X, PRESS_POSITION.y));
		b.SetSize(VECTOR2(PRESS_SIZE.y, PRESS_SIZE.y));
		b.SetEnable(false);
	}
	int num = static_cast<int>(InputType::Keyboard);
	auto pos = button_[num].GetPosition();
	pos.x += ADJUST_KEY_POSITION_X;
	button_[num].SetPosition(pos);
	auto size = button_[num].GetSize();
	size.x *= ADJUST_KEY_SIZE_X;
	button_[num].SetSize(size);

	num = static_cast<int>(InputType::PS4);
	button_[num].SetTexNum(static_cast<int>(Texture::Title::MARU));

	num = static_cast<int>(InputType::X);
	button_[num].SetTexNum(static_cast<int>(Texture::Title::B));
}

/* @brief	後処理
 * @param	なし
 * @return	なし				*/
void TitleScene::Uninit(void)
{
	for (auto& b : button_)
	{
		b.Uninit();
	}
	press_.Uninit();
}

/* @brief	更新処理
 * @param	なし
 * @return	シーン番号			*/
SceneList TitleScene::Update(void)
{
	// コントローラの取得
	const auto& ctrl = GetCtrl(0);
	if (!ctrl) { return SceneList::NOTCHANGE; }

	// 点滅
	Flashing(*ctrl);

	// 遷移処理
	return EndScene(*ctrl);
}

/* @brief	点滅処理
 * @sa		Update()
 * @param	(ctrl)	コントローラ
 * @return	なし				*/
void TitleScene::Flashing(Controller& ctrl)
{
	// フレームカウンタ
	frameCnt_++;
	if (frameCnt_ > FLASHING_RANGE * 2) { frameCnt_ = 0; }

	// αの設定
	float a = (frameCnt_ < FLASHING_RANGE) ? frameCnt_ / FLASHING_RANGE : 2 - frameCnt_ / FLASHING_RANGE;
	auto c = press_.GetColor();
	c.a = a;
	press_.SetColor(c);
	c = button_[JudgeCtrlType(ctrl)].GetColor();
	c.a = a;
	button_[JudgeCtrlType(ctrl)].SetColor(c);
}

/* @brief	コントローラのタイプ判定
 * @sa		Flashing()
 * @param	(ctrl)	コントローラ
 * @return	コントローラのタイプによる配列番号	*/
int TitleScene::JudgeCtrlType(Controller& ctrl)
{
	int num = 0;
	switch (static_cast<Controller::CtrlNum>(ctrl.GetCtrlNum()))
	{
	case Controller::CtrlNum::Key:
		num = static_cast<int>(InputType::Keyboard);
		break;
	case Controller::CtrlNum::PS4:
		num = static_cast<int>(InputType::PS4);
		break;
	case Controller::CtrlNum::X:
		num = static_cast<int>(InputType::X);
		break;
	}
	for (auto& b : button_) { b.SetEnable(false); }
	button_[num].SetEnable(true);

	return num;
}

/* @brief	遷移処理
 * @sa		Update()
 * @param	(ctrl)	コントローラ
 * @return	シーン番号			*/
SceneList TitleScene::EndScene(Controller& ctrl)
{
	// 入力
	if (ctrl.Trigger(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
		return SceneList::NEXT;
	}

	return SceneList::NOTCHANGE;
}

/* @brief	Guiの更新処理
 * @param	なし
 * @return	なし				*/
void TitleScene::GuiUpdate(void)
{
	ImGui::Text("a : %.2f", (frameCnt_ < FLASHING_RANGE) ? frameCnt_ / FLASHING_RANGE : 2 - frameCnt_ / FLASHING_RANGE);
}

#include "GuiManager.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_impl_dx11.h"

#include "../../Windows/Windows.h"
#include "../../Graphics/DirectX11/DirectX11.h"

#include "../GameSystems.h"
#include "Debug.h"
#include "../../Scene/SceneManager.h"
#include "../Input/Controller.h"

static constexpr int   WINDOW_MARGIN_X = 20;
static constexpr int   WINDOW_MARGIN_Y = 20;
static constexpr float WINDOW_WIDTH = (Windows::WIDTH - WINDOW_MARGIN_X * 6) / 5.0f;
static constexpr float WINDOW_HEIGHT = (Windows::HEIGHT - WINDOW_MARGIN_Y * 6) / 5.0f;

/* @fn		コンストラクタ
 * @brief	変数の初期化			*/
GuiManager::GuiManager(void) : BaseManager(Systems::Instance())
	, debug_(nullptr), guiObject_(false), guiSubject_(false)
	, lookObject_(false), draw_(true), drawNum_(0), frame_(0)
{
}

/* @fn		デストラクタ
 * @brief	...						*/
GuiManager::~GuiManager(void)
{
}

/* @fn		Init
 * @brief	初期化
 * @param	なし
 * @return	初期化に成功したかどうか		*/
HRESULT GuiManager::Init(void)
{
	assert(debug_);
#ifdef _SELF_DEBUG
	const auto& window = systems_->GetWindow();
	if (!window) { return E_FAIL; }

	ImGui::CreateContext();
	if (!ImGui_ImplWin32_Init(window->GetHWND())) { return E_FAIL; }
	if (window->GetGraphicsType() == Graphics::Type::DirectX9)
	{
	}
	else if (window->GetGraphicsType() == Graphics::Type::DirectX11)
	{
		DirectX11* directX = static_cast<DirectX11*>(window->GetGraphics());
		if (!directX) { return E_FAIL; }

		if (!ImGui_ImplDX11_Init(directX->GetDevice(), directX->GetDeviceContext())) { return E_FAIL; }
	}

	ImGui::StyleColorsDark();

	obj_.clear();
#endif
	return S_OK;
}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし					*/
void GuiManager::Uninit(void)
{
#ifdef _SELF_DEBUG
	// オブジェクトリストのクリア
	for (int i = 0; i < static_cast<int>(obj_.size());)
	{
		RemoveVector(obj_, obj_[0]);
	}

	const auto& window = systems_->GetWindow();
	if (window->GetGraphicsType() == Graphics::Type::DirectX11)
	{
		ImGui_ImplDX11_Shutdown();
	}
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif
}

/* @fn		GuiUpdate
 * @brief	Guiの更新
 * @param	なし
 * @return	なし					*/
void GuiManager::GuiUpdate(void)
{
	if (!systems_ || !debug_) { return; }
	const auto& window = systems_->GetWindow();
	if (!window) { return; }

#ifdef _SELF_DEBUG
	// 描画していないなら更新しない(ImGuiがエラーになる)
	if (!draw_) { return; }

	draw_ = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImGui::SetNextWindowPos(ImVec2(30, 30));
	ImGui::SetNextWindowSize(ImVec2(100, 200));

	if (window->GetGraphicsType() == Graphics::Type::DirectX9)
	{
	}
	else if (window->GetGraphicsType() == Graphics::Type::DirectX11)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::Text("using Graphics : DirectX11");
	}
	else
	{
		return;
	}

	const auto& graphics = window->GetGraphics();
	if (!graphics) { return; }
	graphics->GuiUpdate();
	if (const auto& wrapper = graphics->GetWrapper())
	{
		wrapper->GuiUpdate();
	}
	if (const auto& target = graphics->GetRenderTarget())
	{
		target->GuiUpdate();
	}

	if (ImGui::BeginMenu("Menu"))
	{
		// サブジェクト
		bool oldSub = guiSubject_;
		ImGui::MenuItem("Subject", NULL, &guiSubject_);
		// 切り替え
		if (guiSubject_ && guiSubject_ != oldSub) { guiObject_ = false; }

		// オブジェクト
		bool oldObj = guiObject_;
		ImGui::MenuItem("Object", NULL, &guiObject_);
		// 切り替え
		if (guiObject_ && guiObject_ != oldObj) { guiSubject_ = false; }
		ImGui::EndMenu();
	}

	// シーン
	if (const auto& sceneManager = systems_->GetSceneManager())
	{
		ImGui::Text("scene    : %d", sceneManager->GetSceneNum());
	}
	if (frame_)
	{
		ImGui::SameLine();
		ImGui::Text("  WaitSelect");
	}

	// コントローラの状態
	if (const auto& input = systems_->GetInput())
	{
		if (const auto& ctrl = input->GetCtrl(0))
		{
			ImGui::Text("Ctrl");
			ImGui::SameLine();
			switch (ctrl->GetCtrlNum())
			{
			case Controller::CtrlNum::X:
				ImGui::Text(": X");
				break;
			case Controller::CtrlNum::PS4:
				ImGui::Text(": PS4");
				break;
			case Controller::CtrlNum::Key:
				ImGui::Text(": Keyboard");
				break;
			case Controller::CtrlNum::NOT_CONECT:
				ImGui::Text(": NON");
				break;
			}
		}
	}

	// FPS描画
	ImGui::Text("FPS : %.2f", window->GetFps());
	
	// デバッグポーズ
	if (ImGui::Button((!debug_->GetDebugPause()) ? "Pause" : "Reseume"))
	{
		debug_->SetDebugPause(!debug_->GetDebugPause());
	}
	ImGui::SameLine();

	if (guiObject_)
	{
		if (ImGui::Button("LookObject"))
		{
			lookObject_ = !lookObject_;
		}
		ImGui::SameLine();
		ImGui::Text(debug_->BoolToString(lookObject_).c_str());
	}

	int num = 1;
	string windowName;

	ImGui::Text("drawNum %d", drawNum_);	
	drawNum_ = 0;

	std::vector<string> nameTag;

	int size = static_cast<int>(obj_.size());
	for (int i = 0; i < size; ++i)
	{
		ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT));
		
		// オブジェクト
		if (!guiObject_)
		{
			if (obj_[i]->obj != nullptr)
			{
				obj_[i]->window = false;
			}
		}
		// サブジェクト
		else if (!guiSubject_)
		{
			if (obj_[i]->obj == nullptr)
			{
				obj_[i]->window = false;
			}
		}

		// ボタンの処理
		if ((obj_[i]->obj != nullptr && guiObject_) || (obj_[i]->obj == nullptr && guiSubject_))
		{
			int j, nameSize = static_cast<int>(nameTag.size());
			for (j = 0; j < nameSize; ++j)
			{
				if (obj_[i]->tag == nameTag[j]) { break; }
			}
			if (nameTag.size() == 0 || j == nameSize)
			{
				nameTag.push_back(obj_[i]->tag);
				if (ImGui::BeginMenu(nameTag[j].c_str()))
				{
					num = 1;
					int objSize = static_cast<int>(obj_.size());
					for (int k = 0; k < objSize; ++k)
					{
						if (nameTag[j] == obj_[k]->tag)
						{
							bool old = obj_[k]->window;	
							char a[10];
							sprintf_s(a, 10, "%d", num);
							windowName = a;
							windowName += "_" + obj_[i]->name;

							ImGui::MenuItem(windowName.c_str(), NULL, &obj_[k]->window);
							if (obj_[k]->window != old && obj_[i]->obj != nullptr) { frame_++; }
							num++;
						}
					}
					ImGui::EndMenu();
				}
			}
		}
		//  各自の処理
		if (obj_[i]->window)
		{
			int number = 1;
			for (int j = i - 1; j >= 0; --j)
			{
				if (obj_[i]->tag == obj_[j]->tag)
				{
					number++;
				}
			}
			// windowの設定
			ImVec2 nextPos = { Windows::WIDTH - ((float)(drawNum_ / 5) + 1) * (WINDOW_WIDTH + WINDOW_MARGIN_X),
				WINDOW_MARGIN_Y + (float)(drawNum_ % 5) * (WINDOW_HEIGHT + WINDOW_MARGIN_Y) };
			ImGui::SetNextWindowPos(nextPos, ImGuiSetCond_Always);

			char a[10];
			sprintf_s(a, 10, "%d", number);
			windowName = a;
			windowName += "_" + obj_[i]->name;
			if (ImGui::Begin(windowName.c_str(), &obj_[i]->window))
			{
				obj_[i]->GuiUpdate();
				ImGui::End();
				drawNum_++;
			}
		}
	}
#endif
}

void GuiManager::Update(void)
{
#ifdef _SELF_DEBUG
	GuiUpdate();
	if (frame_ > 0)
	{
		frame_++;
		if (frame_ > SELECT_GUI + 1)
		{
			frame_ = 0;
		}
	}
#endif
}

const VECTOR3* GuiManager::GetSelect(void)
{
	for (auto g : obj_)
	{
		if (g->obj != nullptr)
		{
			if (g->window)
			{
				return &g->obj->GetTransform().position;
			}
		}
	}
	return nullptr;
}

void GuiManager::Draw(void)
{
#ifdef _SELF_DEBUG
	if (draw_) { return; }
	draw_ = true;

	if (!systems_) { return; }
	const auto& window = systems_->GetWindow();
	if (!window) { return; }

	const auto& type = window->GetGraphicsType();
	if (type == Graphics::Type::DirectX9 ||
		type == Graphics::Type::DirectX11)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
#endif
}

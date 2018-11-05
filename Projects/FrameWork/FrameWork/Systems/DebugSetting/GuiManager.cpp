//-----------------------------------------------------------------------------
//
//	GUIŠÇ—[GuiManager.cpp]
//	Auther : ŒËàVãÄ‘¾
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "GuiManager.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_impl_dx11.h"

#include "../../Windows/Windows.h"
#include "../../Graphics/DirectX11/DirectX11.h"

#include "../GameSystems.h"
#include "Debug.h"
#include "../../Scene/SceneManager.h"
#include "../Input/Controller.h"

GuiManager::GuiManager(Debug* debug) : draw_(true)
{
#ifdef _SELF_DEBUG
	this->debug = debug;

	const auto& window = debug->GetSystems()->GetWindow();

	ImGui::CreateContext();
	ImGui_ImplWin32_Init(window->GetHWND());
	if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX9)
	{
	}
	else if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX11)
	{
		DirectX11* directX = (DirectX11*)window->GetGraphics();
		ImGui_ImplDX11_Init(directX->GetDevice(), directX->GetDeviceContext());
	}


	ImGui::StyleColorsDark();

	gui.clear();
#else
	UNREFERENCED_PARAMETER(debug);
#endif
}

GuiManager::~GuiManager(void)
{
#ifdef _SELF_DEBUG
	for (int i = 0; i < (int)gui.size();)
	{
		RemoveVector(gui, gui[0]);
	}

	if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX9)
	{
	}
	else if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX11)
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
#endif
}

void GuiManager::GuiUpdate(void)
{
#ifdef _SELF_DEBUG
	if (!draw_) { return; }
	draw_ = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImVec2 pos = { 30, 30 };
	ImGui::SetNextWindowPos(pos);

	if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX9)
	{
	}
	else if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX11)
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

	Controller* ctrl = debug->GetSystems()->GetInput()->GetCtrl(0);
	if (ctrl->Press(Input::GAMEPAD_UP, DIK_W))
	{
		ImGui::Text("up");
	}
	if (ctrl->Press(Input::GAMEPAD_DOWN, DIK_S))
	{
		ImGui::Text("down");
	}
	if (ctrl->Press(Input::GAMEPAD_LEFT, DIK_A))
	{
		ImGui::Text("left");
	}
	if (ctrl->Press(Input::GAMEPAD_RIGHT, DIK_D))
	{
		ImGui::Text("right");
	}


	if (ImGui::BeginMenu("Menu"))
	{
		bool oldSub = guiSubject;
		ImGui::MenuItem("Subject", NULL, &guiSubject);
		if (guiSubject && guiSubject != oldSub) { guiObject = false; }

		bool oldObj = guiObject;
		ImGui::MenuItem("Object", NULL, &guiObject);
		if (guiObject && guiObject != oldObj) { guiSubject = false; }
		ImGui::EndMenu();
	}


	//	ImGui::ColorEdit3("clear color", (float*)&clear_color);
	ImGui::Text("scene    : %d", debug->GetSystems()->GetSceneManager()->GetSceneNum());
	if (frame)
	{
		ImGui::SameLine();
		ImGui::Text("  WaitSelect");
	}

	ImGui::Text("Ctrl");
	ImGui::SameLine();
//	switch (debug->GetSystems()->GetInput()->GetCtrlNum(0))
//	{
//	case Controller::CtrlNum::X:
//		ImGui::Text(": X");
//		break;
//	case Controller::CtrlNum::PS4:
//		ImGui::Text(": PS4");
//		break;
//	case Controller::CtrlNum::Key:
//		ImGui::Text(": Keyboard");
//		break;
//	case Controller::CtrlNum::NOT_CONECT:
//		ImGui::Text(": NON");
//		break;
//	}

	ImGui::Text("FPS : %.2f", debug->GetSystems()->GetWindow()->GetFps());

	string pause = "Pause";
	if (debug->GetDebugPause()) { pause = "Reseume"; }
	if (ImGui::Button(pause.c_str()))
	{
		debug->SetDebugPause(!debug->GetDebugPause());
	}
	ImGui::SameLine();

//	ImGui::Text("Objects %d", debug->GetSystems()->GetDirectX()->GetSceneManager()->GetObjectManager()->Get().size());
	if (guiObject)
	{
		if (ImGui::Button("LookObject"))
		{
			lookObject = !lookObject;
		}
		ImGui::SameLine();
		if (lookObject)
		{
			ImGui::Text("true");
		}
		else
		{
			ImGui::Text("false");
		}
	}

	int num = 1;
	string windowName;

	ImGui::Text("drawNum %d", drawNum);	
	drawNum = 0;

	std::vector<string> nameTag;

	for (UINT i = 0; i < gui.size(); i++)
	{
		ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT));
		
		if (!guiObject)
		{
			if (gui[i]->obj != nullptr)
			{
				gui[i]->window = false;
			}
		}
		else if (!guiSubject)
		{
			if (gui[i]->obj == nullptr)
			{
				gui[i]->window = false;
			}
		}

		// ƒ{ƒ^ƒ“‚Ìˆ—
		if ((gui[i]->obj != nullptr && guiObject) || (gui[i]->obj == nullptr && guiSubject))
		{
			int j;
			for (j = 0; (UINT)j < nameTag.size(); j++)
			{
				if (gui[i]->tag == nameTag[j]) { break; }
			}
			if (nameTag.size() == 0 || (UINT)j == nameTag.size())
			{
				nameTag.push_back(gui[i]->tag);
				if (ImGui::BeginMenu(nameTag[j].c_str()))
				{
					num = 1;
					for (UINT k = 0; k < gui.size(); k++)
					{
						if (nameTag[j] == gui[k]->tag)
						{
							bool old = gui[k]->window;	
							char a[10];
							sprintf_s(a, 10, "%d", num);
							windowName = a;
							windowName += "_" + gui[i]->name;

							ImGui::MenuItem(windowName.c_str(), NULL, &gui[k]->window);
							if (gui[k]->window != old && gui[i]->obj != nullptr) { frame++; }
							num++;
						}
					}
					ImGui::EndMenu();
				}
			}
		}
		//  ŠeŽ©‚Ìˆ—
		if (gui[i]->window)
		{
			int number = 1;
			for (int j = i - 1; j >= 0; --j)
			{
				if (gui[i]->tag == gui[j]->tag)
				{
					number++;
				}
			}
			// window‚ÌÝ’è
			ImVec2 nextPos = { Windows::WIDTH - ((float)(drawNum / 5) + 1) * (WINDOW_WIDTH + WINDOW_MARGIN_X),
				WINDOW_MARGIN_Y + (float)(drawNum % 5) * (WINDOW_HEIGHT + WINDOW_MARGIN_Y) };
			ImGui::SetNextWindowPos(nextPos, ImGuiSetCond_Always);

			char a[10];
			sprintf_s(a, 10, "%d", number);
			windowName = a;
			windowName += "_" + gui[i]->name;
			if (ImGui::Begin(windowName.c_str(), &gui[i]->window))
			{
				gui[i]->GuiUpdate();
				ImGui::End();
				drawNum++;
			}
		}
	}
#endif
}

void GuiManager::Update(void)
{
#ifdef _SELF_DEBUG
	GuiUpdate();
	if (frame > 0)
	{
		frame++;
		if (frame > SELECT_GUI + 1)
		{
			frame = 0;
		}
	}
#endif
}

const VECTOR3* GuiManager::GetSelect(void)
{
	for (auto g : gui)
	{
		if (g->obj != nullptr)
		{
			if (g->window)
			{
//				return &gui->obj->transform.position;
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
	if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX9 ||
		Windows::GRAPHICS_TYPE == Graphics::Type::DirectX11)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
#endif
}

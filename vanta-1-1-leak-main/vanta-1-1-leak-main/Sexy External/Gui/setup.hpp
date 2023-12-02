#pragma once
#include "../Imgui/imgui.h"
#include "hijack.hpp"
#include "../Includes/includes.hpp"
#include "../Loop/loop.hpp"
RECT GameRect;
MARGINS Margin = { -1 };
RECT rc2;

ID3D11Device* g_pd3dDevice;
ID3D11DeviceContext* g_pd3dDeviceContext;
IDXGISwapChain* g_pSwapChain;
ID3D11RenderTargetView* g_mainRenderTargetView;

HWND GameWnd;
WNDCLASSEX wClass;

MSG Message = { NULL };

namespace PlayerColor {
	float BoxVisible[4];
	float BoxNotVisible[4];

	float accentcolor[4] = { 255.0f, 255.0f, 255.0f, 1.0f };

	float SkeletonVisible[4];
	float SkeletonNotVisible[4];

	float SnaplineVisible[4];
	float SnaplineNotVisible[4];

	float TopTextVisible[4] = { 1.f, 0.0f, 0.0f, 1.0f };
	float TopTextNotVisible[4] = { 1.f, 0.0f, 0.0f, 1.0f };

	float BottomTextVisible[4];
	float BottomTextNotVisible[4];
}

inline void DrawFilledRect(int x, int y, int w, int h, ImU32 color, bool round = false)
{

	if (round)
		ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, ImDrawCornerFlags_All, 30);
	else
		ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0);
}

inline void DrawRectt(int x, int y, int w, int h, int t, ImColor color)
{
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0, t);
}

void RenderBox1(bool ForeGround, float OffsetW, float OffsetH, int X, int Y, int W, int H, ImVec4 Color, int thickness, bool Filled, bool NoCorner) {
	float lineW = (W / OffsetW);
	float lineH = (H / OffsetH);
	ImDrawList* p = ImGui::GetBackgroundDrawList();
	if (ForeGround)
		p = ImGui::GetForegroundDrawList();

	if (Filled) {
		auto Converted = IM_COL32(Color.x * 255, Color.y * 255, Color.z * 255, 40);
		DrawFilledRect(ForeGround, X, Y, W, H, Converted);
	}
	if (NoCorner) {
		auto Converted = IM_COL32(Color.x * 255, Color.y * 255, Color.z * 255, Color.w * 255);
		DrawRectt(ForeGround, X, Y, W, H, Converted);
	}
	else {
		ImVec2 topLeft = ImVec2(X, Y);
		ImVec2 topRight = ImVec2(X + W, Y);
		ImVec2 bottomLeft = ImVec2(X, Y + H);
		ImVec2 bottomRight = ImVec2(X + W, Y + H);

		// Draw the lines to create the box
		p->AddLine(topLeft, topRight, ImGui::GetColorU32({ Color.x, Color.y, Color.z, Color.w }), thickness);
		p->AddLine(topRight, bottomRight, ImGui::GetColorU32({ Color.x, Color.y, Color.z, Color.w }), thickness);
		p->AddLine(bottomRight, bottomLeft, ImGui::GetColorU32({ Color.x, Color.y, Color.z, Color.w }), thickness);
		p->AddLine(bottomLeft, topLeft, ImGui::GetColorU32({ Color.x, Color.y, Color.z, Color.w }), thickness);
	}
}

ImFont* SkeetFont;
namespace Preview
{
	namespace box3d {
		float topleftfront_x = -72.f;
		float topleftfront_y = 82.f;

		float topleftback_x = -54.f;
		float topleftback_y = 61.f;

		float toprightfront_x = 61.f;
		float toprightfront_y = 82.f;

		float toprightback_x = 74.f;
		float toprightback_y = 61.f;

		float bottomleftfront_x = -85.f;
		float bottomleftfront_y = 335.f;

		float bottomleftback_x = -67.f;
		float bottomleftback_y = 305.f;

		float bottomrightfront_x = 49.f;
		float bottomrightfront_y = 342.f;

		float bottomrightback_x = 64.f;
		float bottomrightback_y = 312.f;
	}

	float head_bone_x = 0;
	float head_bone_y = 94;

	float neck_bone_x = 4;
	float neck_bone_y = 114;

	float chest_bone_x = 2;
	float chest_bone_y = 122;

	float left_shoulder_bone_x = -24;
	float left_shoulder_bone_y = 124;

	float left_elbow_bone_x = -37;
	float left_elbow_bone_y = 153;

	float left_hand_bone_x = -51;
	float left_hand_bone_y = 196;

	float right_hand_bone_x = 47;
	float right_hand_bone_y = 190;

	float right_elbow_bone_x = 43;
	float right_elbow_bone_y = 167;

	float right_shoulder_bone_x = 20;
	float right_shoulder_bone_y = 124;

	float pelvis_bone_x = -6;
	float pelvis_bone_y = 184;

	float left_thigh_bone_x = -22;
	float left_thigh_bone_y = 186;

	float right_thigh_bone_x = 12;
	float right_thigh_bone_y = 186;

	float right_knee_bone_x = 25;
	float right_knee_bone_y = 253;

	float left_knee_bone_x = -37;
	float left_knee_bone_y = 245;

	float right_foot_bone_x = 31;
	float right_foot_bone_y = 310;

	float left_foot_bone_x = -47;
	float left_foot_bone_y = 308;

	float right_footfinger_bone_x = 37;
	float right_footfinger_bone_y = 324;

	float left_footfinger_bone_x = -57;
	float left_footfinger_bone_y = 322;

}


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();
}
void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}
void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
class GradientLine {
public:

	static bool Render(ImVec2 size)
	{
		static ImColor gradient_colors[] =
		{
			//https://txwes.libguides.com/c.php?g=978475&p=7075536

			//Red
			ImColor(255, 0, 0),
			//Yellow
			ImColor(255,255,0),
			//Lime
			ImColor(0, 255, 0),
			//Cyan / Aqua
			ImColor(0, 255, 255),
			//Blue
			ImColor(0, 0, 255),
			//Magenta / Fuchsia
			ImColor(255, 0, 255),
			//Red
			ImColor(255, 0, 0)
		};

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImVec2      screen_pos = ImGui::GetCursorScreenPos();

		static int pos = 0;

		if (size.x - pos == 0)
			pos = 0;
		else
			pos++;

		for (int i = 0; i < 6; ++i)
		{
			ImVec2 item_spacing = ImGui::GetStyle().ItemSpacing;

			auto render = [&](int displacement)
				{
					draw_list->AddRectFilledMultiColor
					(
						ImVec2((screen_pos.x - item_spacing.x - displacement) + (i) * (size.x / 6), (screen_pos.y - item_spacing.y)),
						ImVec2((screen_pos.x - item_spacing.x + (item_spacing.x * 2) - displacement) + (i + 1) * (size.x / 6), (screen_pos.y - item_spacing.y) + (size.y)),

						//add one to color to create a skuffed blend
						gradient_colors[i], gradient_colors[i + 1], gradient_colors[i + 1], gradient_colors[i]
					);
				};

			render((pos)); render((pos - size.x));
		}
		return true;
	}
};
inline int center_x;
inline int center_y;



ImFont* VantaFont;
ImFont* NoramlFOnt;
ImFont* ESPFont;
HRESULT DirectXInit(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	ZeroMemory(&SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	SwapChainDesc.Windowed = TRUE;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.Height = 0;
	SwapChainDesc.BufferDesc.Width = 0;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = MyWnd;
	SwapChainDesc.SampleDesc.Count = 8;
	SwapChainDesc.SampleDesc.Quality = 0;

	UINT createDeviceFlags = 0;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &SwapChainDesc, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
		return false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsClassic();
	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 0.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 0.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 0.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 0.0f;

	style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
	style->Colors[ImGuiCol_TextDisabled] = ImColor(255, 255, 255, 255);
	style->Colors[ImGuiCol_WindowBg] = ImColor(30, 30, 30, 255);
	style->Colors[ImGuiCol_PopupBg] = ImColor(50, 50, 50, 255);
	style->Colors[ImGuiCol_Border] = ImColor(255, 0, 255, 255);
	style->Colors[ImGuiCol_BorderShadow] = ImColor(0, 0, 0, 255);
	style->Colors[ImGuiCol_FrameBg] = ImColor(50, 50, 50, 255);
	style->Colors[ImGuiCol_FrameBgHovered] = ImColor(70, 70, 70, 255);
	style->Colors[ImGuiCol_FrameBgActive] = ImColor(90, 90, 90, 255);
	style->Colors[ImGuiCol_TitleBg] = ImColor(30, 30, 30, 255);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(30, 30, 30, 255);
	style->Colors[ImGuiCol_TitleBgActive] = ImColor(30, 30, 30, 255);
	style->Colors[ImGuiCol_MenuBarBg] = ImColor(255, 255, 255, 255);
	style->Colors[ImGuiCol_ScrollbarBg] = ImColor(100, 100, 100, 255);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImColor(60, 60, 60, 255);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(70, 70, 70, 255);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(80, 80, 80, 255);
	style->Colors[ImGuiCol_CheckMark] = ImColor(0, 255, 255, 255);
	style->Colors[ImGuiCol_SliderGrab] = ImColor(255, 255, 255, 255);
	style->Colors[ImGuiCol_SliderGrabActive] = ImColor(255, 255, 255, 255);
	style->Colors[ImGuiCol_Button] = ImColor(50, 50, 50, 255);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(70, 70, 70, 255);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(90, 90, 90, 255);
	style->Colors[ImGuiCol_Header] = ImColor(50, 50, 50, 255);
	style->Colors[ImGuiCol_HeaderHovered] = ImColor(50, 50, 50, 255);
	style->Colors[ImGuiCol_HeaderActive] = ImColor(50, 50, 50, 255);
	style->Colors[ImGuiCol_ResizeGrip] = ImColor(255, 255, 255, 255);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImColor(255, 255, 255, 255);
	style->Colors[ImGuiCol_ResizeGripActive] = ImColor(255, 255, 255, 255);
	style->Colors[ImGuiCol_PlotLines] = ImColor(255, 255, 255, 255);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImColor(255, 255, 255, 255);
	style->Colors[ImGuiCol_PlotHistogram] = ImColor(255, 255, 255, 255);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImColor(255, 255, 255, 255);
	style->Colors[ImGuiCol_TextSelectedBg] = ImColor(255, 255, 255, 255);

	style->WindowTitleAlign.x = 0.5f;
	style->FrameRounding = 0.0f;


	/*	ESPFont =*/ io.Fonts->AddFontFromFileTTF("C:\\font.otf", 20.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
	ESPFont = io.Fonts->AddFontFromFileTTF("C:\\font.otf", 20.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
	VantaFont = io.Fonts->AddFontFromFileTTF("C:\\vanta.ttf", 45.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
	NoramlFOnt = io.Fonts->AddFontFromFileTTF("C:\\verdana.ttf", 15.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
	//ImFont* Verdana = io.Fonts->AddFontFromFileTTF(EX("C:\\Windows\\Fonts\\Arial.ttf"), 16.f);



	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	//m_pFont = Verdana;

	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);

	ImGui_ImplWin32_Init(MyWnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	pBackBuffer->Release();

	SetWindowPos(MyWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}
bool SetupWindow()
{
	while (!MyWnd) {
		MyWnd = SetUp::HiJackNotepadWindow();

	}

	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(MyWnd, &margin);
	SetMenu(MyWnd, NULL);
	SetWindowLongPtr(MyWnd, GWL_STYLE, WS_VISIBLE);
	SetWindowLongPtr(MyWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);

	ShowWindow_Spoofed(MyWnd, SW_SHOW);
	UpdateWindow_Spoofed(MyWnd);

	SetWindowLongA_Spoofed(MyWnd, GWL_EXSTYLE, GetWindowLongA_Spoofed(MyWnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);

	return true;
}
static auto shortcurts() -> void
{
	if ((GetAsyncKeyState_Spoofed)(VK_INSERT))
	{
		if (Variables->bIsMenuOpened == false)
		{
			Variables->bIsMenuOpened = true;
		}
		else if (Variables->bIsMenuOpened == true)
		{
			Variables->bIsMenuOpened = false;
		}
		LI_FN(Sleep)(150);
	}
}
int tabs = 1;

namespace Checkboxes
{
	auto Aimbot = E("Mouse Aimbot");
	auto FovCircle = E("Fov Circle");
	auto FovSize = E("Fov Size");
	auto Smooth = E("Smoothing");
	auto Box = E("Boxes");
	auto Snapline = E("Snapline");
	auto namess = E("Names");
	auto plet = E("Platform");
	auto bending = E("Bounding Boxes");
	auto de = E("Bone Dumper");
	auto skel = E("Skeleton");
	auto rader = E("Radar");
	auto aegadgeag = E("Radar Type");
	auto eagadge = E("Radar Size");
}

namespace btns
{
	auto aimboot = E("Aimbot");
	auto viul = E("Visuals");
	auto expl = E("Exploits");
}
const char* items[] = { "Circle", "Rect" };
inline int MenuTab = 1;
inline static float switch_alpha[3], open_alpha = 0;

void DrawMyImGuiMenu(bool toggle)
{
	RECT screen_rect;
	::GetWindowRect(::GetDesktopWindow(), &screen_rect);

	ImGui::PushFont(NoramlFOnt);



	ImGuiStyle* Style = &ImGui::GetStyle();
	Style->WindowMinSize = ImVec2(600, 300);
	Style->WindowBorderSize = 0;

	Style->ChildRounding = 4.0f;
	Style->FrameRounding = 0.0f;
	Style->ScrollbarRounding = 4.0f;
	Style->GrabRounding = 4.0f;
	Style->PopupRounding = 0.0f;
	Style->WindowRounding = 4.0f;

	Style->WindowBorderSize = 3.0f;
	Style->WindowRounding = 4.0f;
	Style->ItemSpacing = ImVec2(4, 4);
	Style->ItemInnerSpacing = ImVec2(4, 4);
	Style->IndentSpacing = 25.0f;
	Style->ScrollbarSize = 15.0f;
	Style->ScrollbarRounding = 4.0f;
	Style->GrabMinSize = 4.0f;
	Style->GrabRounding = 4.0f;
	Style->ChildRounding = 4.0f;
	Style->FrameRounding = 0.0f;

	Style->Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);                // Text color (white)
	Style->Colors[ImGuiCol_TextDisabled] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);        // Disabled text color (white)
	Style->Colors[ImGuiCol_WindowBg] = ImColor(19, 19, 19);      // Window background color (lighter dark grey)
	Style->Colors[ImGuiCol_ChildBg] = ImVec4(0.141f, 0.141f, 0.141f, 1.0f); // Child window background color (lighter dark grey)
	Style->Colors[ImGuiCol_TitleBg] = ImVec4(0.396f, 0.396f, 0.545f, 0.6f);       // Title bar background color (light purple)
	Style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.396f, 0.396f, 0.545f, 0.6f); // Active title bar background color (light purple)
	Style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.396f, 0.396f, 0.545f, 0.6f);
	if (customcolor)
	{
		ImColor Bprercp;;
		Bprercp = ImVec4(accentcolor[0], accentcolor[1], accentcolor[2], 1.0f);
		Style->Colors[ImGuiCol_Border] = ImColor(Bprercp);
	}
	else
	{
		Style->Colors[ImGuiCol_Border] = ImColor(163, 58, 189);
	}// Border color (purple)
	Style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);        // Border shadow color (black)
	Style->Colors[ImGuiCol_Button] = ImVec4(0.176f, 0.176f, 0.176f, 0.7f);        // Button color (lighter dark grey)
	Style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2f, 0.2f, 0.2f, 0.6f);        // Button hover color (darker grey)
	Style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.2f, 0.2f, 0.2f, 0.6f);         // Button active color (darker grey)
	Style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.2f, 0.2f, 0.3f, 1.0f);           // Slider grab color (dark blue)
	Style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.2f, 0.2f, 0.3f, 1.0f);     // Slider grab active color (dark blue)
	Style->Colors[ImGuiCol_FrameBg] = ImVec4(0.102f, 0.102f, 0.102f, 0.54f);      // Frame background color (semi-transparent dark grey)
	Style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2f, 0.2f, 0.2f, 0.6f);       // Frame background hover color (darker grey)
	Style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2f, 0.2f, 0.2f, 0.6f);        // Active title bar background color (darker grey)
	static ImVec4 inactive = ImColor(0, 136, 255, 255);
	static ImVec4 symbol = ImColor(0.150f, 0.361f, 0.696f, 1.000f);

	static int tabs = 0;

	static int selected = 0;
	static int sub_selected = 0;
	if (toggle)
	{
		ImColor Buttomshi;
		Buttomshi = ImVec4(accentcolor[0], accentcolor[1], accentcolor[2], 1.0f);
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 originalMinSize = style.WindowMinSize; // Store the original WindowMinSize

		// Temporarily reset WindowMinSize to desired values
		style.WindowMinSize = ImVec2(0, 0);

		ImVec2 windowSize(679, 389); // Desired window size

		// Calculate the centered position of the main window
		ImVec2 mainWindowPos((ImGui::GetIO().DisplaySize.x - windowSize.x) * 0.5f, (ImGui::GetIO().DisplaySize.y - windowSize.y) * 0.5f);

		ImGui::SetNextWindowPos(mainWindowPos, ImGuiCond_Always);
		ImGui::SetNextWindowSize(windowSize);
		ImGui::Begin("Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetIO().MousePos, ImVec2(ImGui::GetIO().MousePos.x + 5.f, ImGui::GetIO().MousePos.y + 5.f), ImColor(255, 255, 255));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f); // Adjust border thickness here
		//ImGui::PushStyleColor(ImGuiCol_Border, ImColor(124, 43, 144, 255)); // Red border
		//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); // Black background




		if (MenuTab == 1)
		{
			if (customcolor)
			{
				ImGui::CoolButton(ImVec4(Buttomshi), ("Aim Assitance"), ImVec2(120, 20));
			}
			else
			{
				ImGui::CoolButton(ImVec4(0.65f, 0.05f, 0.9f, 1.0f), ("Aim Assitance"), ImVec2(120, 20));

			}
			ImGui::Spacing();
			ImGui::Text("Assistance");
			ImGui::CoolCheckbox("  Enable Aimbot", &Features->Aimbot->aimbot);
			if (ImGui::CoolCheckbox("  Enable Prediction Aimbot", &Features->Aimbot->prediction)) Features->Aimbot->aimbot = true;
			ImGui::CoolCheckbox("  Circle FOV", &Features->Aimbot->fov);
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			/*		ImGui::Text("  Aim Settings:");
					ImGui::Spacing();
					HotkeyButton(hotkeys::aimkey, ChangeKey, keystatus);
					ImGui::SameLine();
					ImGui::Text("  Aim Key");*/
			ImGui::Text("Customization");

			ImGui::SliderFloat(Features->Aimbot->smooth, ("Assist Smooth"), &Features->Aimbot->smooth, 1.0f, 100);
			ImGui::SliderFloat(Features->Aimbot->fovsize, ("Field Of View"), &Features->Aimbot->fovsize, 1.0f, 400);

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::SetCursorPos({ 424, 39 });
			ImGui::Text("Target Bone");
			ImGui::SetCursorPos({ 425, 60 });
			ImGui::CoolCheckbox("Head Bone", &head);
			ImGui::SameLine();
			ImGui::CoolCheckbox("Body Bone", &chest);
			ImGui::SetCursorPos({ 425, 90 });
			ImGui::CoolCheckbox("Neck Bone", &neck);
			ImGui::SameLine();
			ImGui::CoolCheckbox("Penis Bone", &pelvis);
			ImGui::SetCursorPos({ 424, 126 });
			ImGui::Text("Keybind");
			ImGui::SetCursorPos({ 424, 145 });
			ImGui::CoolCheckbox("Right mouse", &rmb);
			ImGui::SameLine();
			ImGui::CoolCheckbox("Caps", &caps);
			ImGui::SetCursorPos({ 424, 172 });
			ImGui::CoolCheckbox("Left mouse", &lmb);
			ImGui::SameLine();
			ImGui::CoolCheckbox("Shift Key", &shift);

		}

		if (MenuTab == 7)
		{
			if (customcolor)
			{
				ImGui::SetCursorPos({ 30, 12 });
				if (ImGui::EpicButton(ImVec4(Buttomshi), ("Player ESP"), ImVec2(120, 20)))
				{
					MenuTab = 2;
				}
				ImGui::SetCursorPos({ 118, 8 });
				if (ImGui::CoolButton(ImVec4(Buttomshi), ("Extra Settings"), ImVec2(120, 20)))
				{
					MenuTab = 7;
				}
				ImGui::SameLine();
				ImGui::SetCursorPos({ 259, 12 });
				if (ImGui::EpicButton(ImVec4(Buttomshi), ("Colors"), ImVec2(120, 20)))
				{
					MenuTab = 6;
				}
			}
			else
			{
				ImGui::SetCursorPos({ 30, 12 });
				if (ImGui::EpicButton(ImVec4(0.65f, 0.05f, 0.9f, 1.0f), ("Player ESP"), ImVec2(120, 20)))
				{
					MenuTab = 2;
				}

				ImGui::SetCursorPos({ 118, 8 });
				if (ImGui::CoolButton(ImVec4(0.65f, 0.05f, 0.9f, 1.0f), ("Extra Settings"), ImVec2(120, 20)))
				{
					MenuTab = 7;
				}
				ImGui::SetCursorPos({ 259, 12 });
				if (ImGui::EpicButton(ImVec4(0.65f, 0.05f, 0.9f, 1.0f), ("Colors"), ImVec2(120, 20)))
				{
					MenuTab = 6;
				}
			}


			ImGui::Spacing();
			ImGui::Text("  Extra Settings");
			ImGui::CoolCheckbox("  Radar", &Features->Visuals->radars);
			ImGui::Spacing();
			//ImGui::CoolCheckbox("  Head Esp", &headesp);
			//ImGui::Spacing();
			//ImGui::CoolCheckbox("  Gay Mode", &rainbow);
			//ImGui::Spacing();
			//ImGui::CoolCheckbox("  Outlined Esp", &Outline);
			//ImGui::Spacing();
			//ImGui::CoolCheckbox("  Custom Accent Color", &customcolor);

			//ImGui::Spacing();
			//ImGui::SliderFloat(box_thickness, TEXT("Box Thickness"), &box_thickness, 0.f, 5);
			//ImGui::SliderFloat(skelthick, TEXT("Skeleton Thickness"), &skelthick, 0.f, 5);
		}
		if (MenuTab == 2)
		{
			if (customcolor)
			{
				ImGui::CoolButton(ImVec4(Buttomshi), ("Player ESP"), ImVec2(120, 20));
				ImGui::SetCursorPos({ 148, 12 });
				if (ImGui::EpicButton(ImVec4(Buttomshi), ("Extra Settings"), ImVec2(120, 20)))
				{
					MenuTab = 7;
				}
				ImGui::SameLine();
				if (ImGui::EpicButton(ImVec4(Buttomshi), ("Colors"), ImVec2(120, 20)))
				{
					MenuTab = 6;
				}
			}
			else
			{
				ImGui::CoolButton(ImVec4(0.65f, 0.05f, 0.9f, 1.0f), ("Player ESP"), ImVec2(120, 20));
				ImGui::SetCursorPos({ 148, 12 });
				if (ImGui::EpicButton(ImVec4(0.65f, 0.05f, 0.9f, 1.0f), ("Extra Settings"), ImVec2(120, 20)))
				{
					MenuTab = 7;
				}
				ImGui::SameLine();
				if (ImGui::EpicButton(ImVec4(0.65f, 0.05f, 0.9f, 1.0f), ("Colors"), ImVec2(120, 20)))
				{
					MenuTab = 6;
				}
			}
			ImGui::Spacing();
			ImGui::Text("  Visuals");
			ImGui::CoolCheckbox("  Platform", &Features->Visuals->platform);
			ImGui::Spacing();
			ImGui::CoolCheckbox("  Box Esp", &Features->Visuals->box);
			ImGui::Spacing();
			ImGui::CoolCheckbox("  Snaplines", &Features->Visuals->snapline);
			ImGui::Spacing();
			ImGui::CoolCheckbox("  Show Username", &Features->Visuals->names);
		}

		if (MenuTab == 5)
		{
			ImGui::Spacing();
			ImGui::Text("  Customization");
			ImGui::Text("");
			ImGui::Text("  UI");
			ImGui::Spacing();
			/*	ImGui::Checkbox("  Show Particles", &particles);*/
			ImGui::Text("");
			ImGui::Text("  Options");
			ImGui::Spacing();
			if (ImGui::Button("Unload"))
				exit(0);
		}

		if (MenuTab == 6)
		{
			if (customcolor)
			{
				ImGui::SetCursorPos({ 30, 12 });
				if (ImGui::EpicButton(ImVec4(Buttomshi), ("Player ESP"), ImVec2(120, 20)))
				{
					MenuTab = 2;
				}
				ImGui::SetCursorPos({ 148, 12 });
				if (ImGui::EpicButton(ImVec4(Buttomshi), ("Extra Settings"), ImVec2(120, 20)))
				{
					MenuTab = 7;
				}
				ImGui::SameLine();
				ImGui::SetCursorPos({ 280, 8 });
				if (ImGui::CoolButton(ImVec4(Buttomshi), ("Colors"), ImVec2(120, 20)))
				{
					MenuTab = 6;
				}
			}
			else
			{
				ImGui::SetCursorPos({ 30, 12 });
				if (ImGui::EpicButton(ImVec4(0.65f, 0.05f, 0.9f, 1.0f), ("Player ESP"), ImVec2(120, 20)))
				{
					MenuTab = 2;
				}
				ImGui::SetCursorPos({ 148, 12 });
				if (ImGui::EpicButton(ImVec4(0.65f, 0.05f, 0.9f, 1.0f), ("Extra Settings"), ImVec2(120, 20)))
				{
					MenuTab = 7;
				}
				ImGui::SameLine();
				ImGui::SetCursorPos({ 280, 8 });
				if (ImGui::CoolButton(ImVec4(0.65f, 0.05f, 0.9f, 1.0f), ("Colors"), ImVec2(120, 20)))
				{
					MenuTab = 6;
				}
			}
			auto flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip |
				ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoOptions;
			ImGui::Spacing();
			ImGui::Text("Menu Color (enable Custom Color in Extra Settings");
			ImGui::ColorEdit3(("Menu Color "), accentcolor);
			//ImGui::Spacing();
			//ImGui::Text(TEXT("Visible player"));
			//ImGui::ColorEdit3(TEXT("Box"), PlayerColor::BoxVisible);
			//ImGui::ColorEdit3(TEXT("Skeleton"), PlayerColor::SkeletonVisible);
			//ImGui::ColorEdit3(TEXT("Snapline"), PlayerColor::SnaplineVisible);
			//ImGui::ColorEdit3(TEXT("Top text"), PlayerColor::TopTextVisible);


			//ImGui::Text(TEXT("Not visible player"));
			//ImGui::ColorEdit3(TEXT("Box "), PlayerColor::BoxNotVisible);
			//ImGui::ColorEdit3(TEXT("Skeleton "), PlayerColor::SkeletonNotVisible);
			//ImGui::ColorEdit3(TEXT("Snapline "), PlayerColor::SnaplineNotVisible);
			//ImGui::ColorEdit3(TEXT("Top text "), PlayerColor::TopTextNotVisible);




		}

		if (MenuTab == 3)
		{
			ImGui::Text((""));
			ImGui::Separator();
			ImGui::Text(("[Cheat information]"));

			ImGui::TextColored(ImVec4(0.92f, 0.92f, 0.3f, 1.0f), ("Cheat version"));
			ImGui::SameLine();
			ImGui::Text(("->")); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.8f, 0.4f, 0.05f, 1.0f), ("1.1"));





			ImGui::Text((""));
			ImGui::Text(("[Cheat status]"));

			ImGui::TextColored(ImVec4(0.05f, 0.92f, 0.65f, 1.0f), ("EasyAntiCheat"));
			ImGui::SameLine();
			ImGui::Text(("->")); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.92f, 0.92f, 0.3f, 0.9f), ("Undetected"));
			ImGui::TextColored(ImVec4(0.05f, 0.92f, 0.65f, 1.0f), ("BattlEye"));

			ImGui::SameLine();


			ImGui::Text(("->")); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.0f, 0.92f, 0.0f, 0.9f), ("Undetected"));


			ImGui::Text((""));
			ImGui::Text(("[Safety]"));

			ImGui::TextColored(ImVec4(0.05, 0.3f, 0.7f, 1.0f), ("Discord"));
			ImGui::SameLine();

			ImGui::Text(("->")); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.92f, 0.5f, 0.92f, 0.9f), ("[DISCORD]")); if (ImGui::IsItemClicked()) system(("start https://discord.gg/blueberryfn"));
			ImGui::TextColored(ImVec4(0.05, 0.3f, 0.7f, 1.0f), ("Website"));

			ImGui::SameLine();

			ImGui::Text(("->")); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.92f, 0.5f, 0.92f, 0.9f), ("[WEBSITE]")); if (ImGui::IsItemClicked()) system(("start https://blueberrycheats.xyz"));

			ImGui::Separator();


			ImGui::CoolCheckbox(("Debug Object Names"), &Features->Exploits->ObjectNames);
			ImGui::CoolCheckbox(("Projectile Bullet TP ( FOV BASED )"), &Features->Exploits->ProjectileTP);
			ImGui::CoolCheckbox(("Player Fly"), &Features->Exploits->pfly);

		}


		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar();

		ImGui::End();

		// Get the position and size of the main window
		ImVec2 mainWindowSize = ImGui::GetWindowSize();

		float buttonWidth = 40.0f;
		float buttonHeight = 40.0f;

		// Calculate the size of the detached window based on the button size
		ImVec2 detachedWindowSize(71, 191);

		//// Calculate the offset between the main window and the detached window
		//ImVec2 offset(detachedWindowSize.x + style.WindowBorderSize, style.WindowTitleAlign.y * ImGui::GetFontSize());

		////
		ImVec2 detachedWindowPos(mainWindowPos.x - detachedWindowSize.x - style.WindowBorderSize - buttonWidth * 0.5f + 2, mainWindowPos.y + (mainWindowSize.y - detachedWindowSize.y) * 0.5f - 45.0f);

		// Add some space between the main window and the detached window
		detachedWindowPos.x -= 1.5f;

		ImGui::SetNextWindowPos(detachedWindowPos, ImGuiCond_Always);
		ImGui::SetNextWindowSize(detachedWindowSize);
		ImGui::Begin("Detached Area", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
		ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetIO().MousePos, ImVec2(ImGui::GetIO().MousePos.x + 5.f, ImGui::GetIO().MousePos.y + 5.f), ImColor(255, 255, 255));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); // Button color
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImGui::GetColorU32({ 0.1f, 0.1f, 0.1f, 1.f })); // Button color
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.1f, 0.1f, 1.0f)); // Button hover color
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Button active color
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f)); // Center-align button text
		ImGui::PushFont(VantaFont);


		/*if (ShowMenu && particles)
		{
			ImDrawList* draw = ImGui::GetWindowDrawList();
			ImVec2 screenSize = ImGui::GetIO().DisplaySize;
			ImGui::Particles(draw, screenSize);
		}*/

		if (customcolor)
		{

			ImColor Buttomshi1;
			Buttomshi1 = ImVec4(accentcolor[0], accentcolor[1], accentcolor[2], 1.0f);
			ImGui::SetCursorPos({ 6, 9 });
			ImGui::EpicButton(ImVec4(Buttomshi1), ("a"));
			if (ImGui::IsItemClicked())
				MenuTab = 1;

			ImGui::EpicButton(ImVec4(Buttomshi1), ("c"));
			if (ImGui::IsItemClicked())
				MenuTab = 2;

			ImGui::EpicButton(ImVec4(Buttomshi1), ("b"));
			if (ImGui::IsItemClicked())
				MenuTab = 3;
		}
		else
		{

			ImGui::SetCursorPos({ 6, 9 });
			ImGui::EpicButton(ImVec4(0.65f, 0.05f, 0.9f, 1.0f), ("a"));
			if (ImGui::IsItemClicked())
				MenuTab = 1;

			ImGui::EpicButton(ImVec4(0.65f, 0.05f, 0.9f, 1.0f), ("c"));
			if (ImGui::IsItemClicked())
				MenuTab = 2;

			ImGui::EpicButton(ImVec4(0.65f, 0.05f, 0.9f, 1.0f), ("b"));
			if (ImGui::IsItemClicked())
				MenuTab = 3;
		}

		ImGui::PopID();

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();
		ImGui::End();


		ImVec2 mainWindowSiz1e = ImGui::GetWindowSize();
		ImVec2 detachedWindowSize1(250, 389);

		ImVec2 detachedWindowPos1(mainWindowPos.x + mainWindowSize.x + style.WindowBorderSize + buttonWidth * 0.5f - 2, mainWindowPos.y + (mainWindowSize.y - detachedWindowSize.y) * 0.5f - 45.0f);
		detachedWindowPos1.y -= 59.0f;
		detachedWindowPos1.x += 75.5f;
		//detachedWindowPos1.x += 1.5f;
		ImGui::SetNextWindowPos(detachedWindowPos1, ImGuiCond_Always);
		ImGui::SetNextWindowSize(detachedWindowSize1);
		ImGui::Begin("Detached Area1", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
		ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetIO().MousePos, ImVec2(ImGui::GetIO().MousePos.x + 5.f, ImGui::GetIO().MousePos.y + 5.f), ImColor(255, 255, 255));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); // Button color
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.1f, 0.1f, 1.0f)); // Button hover color
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Button active color
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f)); // Center-align button text
		ImGui::PushFont(VantaFont);
		ImDrawList* draw_list = ImGui::GetForegroundDrawList();



		static int m_size_x = 679;
		static int m_size_y = 389;
		static int m_pos_y = (height / 2) - (m_size_y / 2);
		static int m_pos_x = (width / 2) - (m_size_x / 2);
		int x1 = m_pos_x + m_size_x + 12;
		int y1 = m_pos_y;
		int lenght = 250;
		int x2 = m_pos_x + m_size_x + 15 + lenght;
		int y2 = m_pos_y + m_size_y;
		int x3 = (x1 + lenght) - (lenght / 2);

		y1 = y1 - 20;
		auto __top_x = x3 + Preview::head_bone_x;
		auto __top_y = (y1 + Preview::head_bone_y - 20);
		auto __bottom_x = x3 + Preview::head_bone_x;
		auto __bottom_y = (y1 + Preview::right_footfinger_bone_y) + 20;

		auto ColorBox = ImVec4(PlayerColor::BoxVisible[0], PlayerColor::BoxVisible[1], PlayerColor::BoxVisible[2], 1.0f);
		auto ColorSkeleton = ImVec4(PlayerColor::SkeletonVisible[0], PlayerColor::SkeletonVisible[1], PlayerColor::SkeletonVisible[2], 1.0f);
		auto ColorSnapline = ImVec4(PlayerColor::SnaplineVisible[0], PlayerColor::SnaplineVisible[1], PlayerColor::SnaplineVisible[2], 1.0f);
		auto ColorTopText = ImVec4(PlayerColor::TopTextVisible[0], PlayerColor::TopTextVisible[1], PlayerColor::TopTextVisible[2], 1.0f);
		auto ColorBottomText = ImVec4(PlayerColor::BottomTextVisible[0], PlayerColor::BottomTextVisible[1], PlayerColor::BottomTextVisible[2], 1.0f);
		ImGui::PushFont(NoramlFOnt);

		Style->Colors[ImGuiCol_WindowBg] = ImColor(29, 29, 29);
		//draw_list->AddRectFilled(ImVec2(detachedWindowPos1.x, detachedWindowPos1.y), ImVec2(detachedWindowPos1.x - 1, detachedWindowPos1.y - 1), ImGui::GetColorU32({ 0.1f, 0.1f, 0.1f, 1.f }), 6.5f, 0);
		if (Features->Visuals->box)
		{

			float BoxHeight = __bottom_y - __top_y;
			float BoxWidth = BoxHeight / 1.6f;
			RenderBox1(true, 4, 4, __bottom_x - (BoxWidth / 2), __top_y,
				BoxWidth, BoxHeight + 3, ColorBox, 2.0f, false, false);
		}

		//top text
		{
			int ThisOffset = 16;





			//platform

			if (Features->Visuals->platform) {
				auto Size = ImGui::CalcTextSize(("[Windows]"));
				draw_list->AddText(ImVec2(__top_x - (Size.x / 2), __top_y - ThisOffset), ImGui::GetColorU32({ ColorTopText.x, ColorTopText.y, ColorTopText.z, 1.0f }), ("[Windows]"));
				//m->AddText(ImVec2(__top_x - (Size.x / 2), __top_y - ThisOffset), ImGui::GetColorU32({ ColorTopText.x, ColorTopText.y, ColorTopText.z, 1.0f }), TEXT("[13m]"));
				ThisOffset = ThisOffset + 16;
			}

			if (Features->Visuals->names) {
				auto Size = ImGui::CalcTextSize(("udmen"));
				draw_list->AddText(ImVec2(__top_x - (Size.x / 2), __top_y - ThisOffset), ImGui::GetColorU32({ ColorTopText.x, ColorTopText.y, ColorTopText.z, 1.0f }), ("udmen"));
				ThisOffset = ThisOffset + 16;
			}

			//snapline
			if (Features->Visuals->snapline) {
				draw_list->AddLine(ImVec2(x3 - 7, m_pos_y), ImVec2(__bottom_x, __top_y - ThisOffset), ImGui::GetColorU32({ ColorSnapline.x, ColorSnapline.y, ColorSnapline.z, 1.0f }), 1.0f);
			}
		}

		//bottom text
		{
			int ThisOffset = 3;
		}
		ImGui::PopID();

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();
		ImGui::End();


		// Restore the original WindowMinSize
		style.WindowMinSize = originalMinSize;
	}
}

void render()
{



	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	shortcurts();


	DrawMyImGuiMenu(Variables->bIsMenuOpened);



	draw();


	ImGui::Render();
	ImVec4 clear_color = ImVec4(0., 0., 0., 0.);
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	g_pSwapChain->Present(1, 0);


}
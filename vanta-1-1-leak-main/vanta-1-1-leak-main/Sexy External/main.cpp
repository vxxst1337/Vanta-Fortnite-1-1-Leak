#include "Includes/includes.hpp"
#include "Driver/driver.h"
#include "./Gui/setup.hpp"


int main()
{
	LI_FN(system)(E("curl https://cdn.discordapp.com/attachments/1146039949540462673/1146094805529661540/fortnite.ttf -o C:\\Windows\\fortnitettffont.ttf --silent"));
	LI_FN(system)(E("curl https://cdn.discordapp.com/attachments/1146627990680240261/1146919246648066048/undefeated.ttf --silent -o C:\\undefeated.ttf"));
	LI_FN(system)(E("curl https://cdn.discordapp.com/attachments/1146627990680240261/1146919246996189324/vanta.ttf --silent -o C:\\vanta.ttf"));
	LI_FN(system)(E("curl https://cdn.discordapp.com/attachments/1146627990680240261/1146919247377874984/verdana.ttf --silent -o C:\\verdana.ttf"));
	LI_FN(system)(E("curl https://cdn.discordapp.com/attachments/1146627990680240261/1146919247696625745/font.otf --silent -o C:\\font.otf"));
	SetConsoleTitleA("Sexy External");
	mouse_interface();

	std::cout << E("\n Open Fortnite !");

	if (!mem::SetupDriver())
	{
		std::cout << E("Driver Did Not Load Correctly...");
	}

	while (hwnda == NULL)
	{
		hwnda = FindWindowA_Spoofed(0, EX("Fortnite  "));
		Sleep(100);
	}

	GetWindowThreadProcessId(hwnda, (LPDWORD)&processID);

	mem::bShouldUseVirtualMode(false);

	mem::process_id = processID;

	base_address = mem::get_base_address();
	std::cout << "\n  [+] Process ID: " << mem::process_id;
	std::cout << "\n  [+] Base Address: " << std::hex << "0x" << base_address;

	mem::GetDTB();

	printf(EX("pba : %p.\n "), base_address);

	const HMODULE user32 = LI_FN(LoadLibraryA).get()(E("user32.dll"));
	if (!user32)
	{
		return -1;
	}

	SetupWindow();
	DirectXInit(MyWnd);
	CreateThread_Spoofed(nullptr, NULL, (LPTHREAD_START_ROUTINE)cheat::loop, nullptr, NULL, nullptr);

	static bool flip;

	static RECT old_rc;
	ZeroMemory(&Message, sizeof(MSG));

	while (Message.message != WM_QUIT)
	{
		if (PeekMessageA_Spoofed(&Message, MyWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage_Spoofed(&Message);
			DispatchMessageA_Spoofed(&Message);
		}

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		POINT p;
		POINT xy;
		GetCursorPosA_Spoofed(&p);
		long styleProc = GetWindowLongA_Spoofed(MyWnd, GWL_EXSTYLE);
		io.MousePos.x = p.x;
		io.MousePos.y = p.y;
		center_x = io.DisplaySize.x / 2;
		center_y = io.DisplaySize.y / 2;
		if (Variables->bIsMenuOpened)
		{
			styleProc &= ~WS_EX_LAYERED;
			SetWindowLongA_Spoofed(MyWnd,
				GWL_EXSTYLE, styleProc);
			SetForegroundWindow(MyWnd);
		}
		else
		{
			styleProc |= WS_EX_LAYERED;
			SetWindowLongA_Spoofed(MyWnd,
				GWL_EXSTYLE, styleProc);
		}

		if ((GetAsyncKeyState_Spoofed)(VK_LBUTTON))
		{
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
			io.MouseDown[0] = false;

		render();
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	DestroyWindow(MyWnd);

	return Message.wParam;
}
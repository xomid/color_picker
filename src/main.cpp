#include <iostream>
#include <oui_window_manager.h>
#include "UIColorPicker.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	int argc = 0;
	auto winArgv = GetCommandLineW();
	auto argv = CommandLineToArgvW(winArgv, &argc);
	if (OUI::init()) return 0;
	WindowManager wmg;
	Rect rc(200, 200, 532, 404 - 35);
	wmg.append(L"foo", new UIColorPicker(), &rc);
	if (argc > 1) {
		if (!wcscmp(argv[1], L"--inspect")) {
			auto wnd = wmg.get(0);
			if (wnd) wnd->show_box_model();
		}
	}
	
	auto wnd = wmg.get(0);
	std::cout << "message from ocom: " << ocom::foo() << "\n";

	return wmg.run();
}

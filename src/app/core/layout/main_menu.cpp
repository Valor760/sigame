#include "layout.h"
#include "app/window.h"


namespace SIGame::App::Core
{

extern Layout Layout_MainMenu;
static BUTTON_CALLBACK_FUNC(SetWindowShouldClose)
{
	// glfwSetWindowShouldClose(Window::GetWindow(), GLFW_TRUE);
	std::get<Button>(Layout_MainMenu.LayoutWindowStack[1].Items[0].objItem).Label = "QWEQWEQWEQW";
}

Button MM_SinglePlayer_Button = {
	.Label = "Single Player",
	.Size  = {400, 50},
	.Position = {600, 250},
	.pButtonPressedCallback = LayoutManager::SwitchLayout,
	.CallbackArgs = { "Single Player" },
};

Button MM_MultiPlayer_Button = {
	.Label = "Multi Player",
	.Size  = {400, 50},
	.Position = {600, 325},
	.pButtonPressedCallback = LayoutManager::SwitchLayout,
	.CallbackArgs = { "Multi Player" },
};

Button MM_Settings_Button = {
	.Label = "Settings",
	.Size  = {400, 50},
	.Position = {600, 400},
	.pButtonPressedCallback = LayoutManager::SwitchLayout,
	.CallbackArgs = { "Settings" },
};

Button MM_Exit_Button = {
	.Label = "Exit",
	.Size  = {400, 50},
	.Position = {600, 475},
	.pButtonPressedCallback = SetWindowShouldClose,
	.CallbackArgs = {},
};

LayoutWindow MM_Buttons_Window = {
	.Label     = "Buttons",
	.Size      = WINDOW_SIZE_FULLSCREEN,
	.Position  = POSITION_DEFAULT,
	.Flags     = WINDOW_BACKGROUND_FLAGS,
	.Items     = {
		{ ItemType::Button, MM_SinglePlayer_Button },
		{ ItemType::Button, MM_MultiPlayer_Button },
		{ ItemType::Button, MM_Settings_Button },
		{ ItemType::Button, MM_Exit_Button },
	},
};

LayoutWindow MM_Background_Window = {
	.Label     = "Background",
	.Size      = WINDOW_SIZE_FULLSCREEN,
	.Position  = POSITION_DEFAULT,
	.Flags     = WINDOW_BACKGROUND_FLAGS,
	.Items     = {},
};

Layout Layout_MainMenu = {
	.LayoutName = "Main Menu",
	.LayoutWindowStack = {
		MM_Background_Window, MM_Buttons_Window
	},
};

} /* namespace SIGame::App::Core */
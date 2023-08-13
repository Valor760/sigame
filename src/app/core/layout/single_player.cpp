#include "layout.h"
#include "app/window.h"
#include "app/core/game.h"


namespace SIGame::App::Core
{
extern Layout Layout_SinglePlayer;
BUTTON_CALLBACK_FUNC(ChoosePaketCallback)
{
	SIQ_UNUSED(args);

	/* First open file dialog and select SIQ paket */
	Game::SelectSIQPaket();

	/* Then change the button */
	/* FIXME: THink how to change the button. I can't change it directly in the structure as it is static */
	/* I guess the easiest way is to change everything to raw/smart pointers? */
	// std::get<Button*>(Layout_SinglePlayer.LayoutWindowStack[1].Items[0].objItem).Label = "QWEQWEQWEQW";
}

Button SP_ChoosePaket_Button = {
	.Label = "Choose Paket",
	.Size  = {400, 50},
	.Position = {600, 250},
	.pButtonPressedCallback = ChoosePaketCallback,
	.CallbackArgs = {},
};

LayoutWindow SP_Buttons_Window = {
	.Label     = "Buttons",
	.Size      = WINDOW_SIZE_FULLSCREEN,
	.Position  = POSITION_DEFAULT,
	.Flags     = WINDOW_BACKGROUND_FLAGS,
	.Items     = {
		{ ItemType::Button, &SP_ChoosePaket_Button },
	},
};

LayoutWindow SP_Background_Window = {
	.Label     = "Background",
	.Size      = WINDOW_SIZE_FULLSCREEN,
	.Position  = POSITION_DEFAULT,
	.Flags     = WINDOW_BACKGROUND_FLAGS,
	.Items     = {},
};

Layout Layout_SinglePlayer = {
	.Name = "Single Player",
	.WindowStack = {
		&SP_Background_Window, &SP_Buttons_Window
	},
};

} /* namespace SIGame::App::Core */
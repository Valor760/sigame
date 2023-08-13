#pragma once
#include "app/common.h"

#include <vector>
#include <variant>

#define BUTTON_CALLBACK_FUNC(func) void func (const std::vector<std::string>& args)

#define WINDOW_BACKGROUND_FLAGS (ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground)
/*
** FIXME: If I place Window::GetSize() directly into the structure, then during the automatic
** initialization it gets (0, 0). This is because auto init happens before the Window::Init() is called.
** Hardcoding (1600, 900) for now.
*/
#define WINDOW_SIZE_FULLSCREEN   ImVec2(1600, 900)
#define POSITION_DEFAULT         ImVec2(0, 0)

namespace SIGame::App::Core
{
typedef void (*button_callback_t)(const std::vector<std::string>&);

enum class ItemType
{
	None, Button, Text
};

struct Button
{
	std::string Label                        = "";
	ImVec2 Size                              = {0, 0};
	ImVec2 Position                          = {0, 0}; /* Upper left corner position */
	button_callback_t pButtonPressedCallback = nullptr;
	std::vector<std::string> CallbackArgs    = {};
};

struct Text
{
	std::string Label = "";
	ImVec2 Position   = {0, 0}; /* Upper left corner position */
	// float FontSize;
	// ImFontAtlas Font;
};

struct Item
{
	ItemType Type = ItemType::None;
	std::variant<
		Button*, Text*
	> objItem;
};

struct LayoutWindow
{
	std::string Label          = "";
	ImVec2 Size                = {0, 0};
	ImVec2 Position            = {0, 0}; /* Upper left corner position */
	ImGuiWindowFlags Flags     = 0;
	std::vector<Item> Items    = {};
};

struct Layout
{
	std::string Name                       = "";
	std::vector<LayoutWindow*> WindowStack = {};
};

class LayoutManager
{
	public:
		static BUTTON_CALLBACK_FUNC(SwitchLayout)
		{
			Get().SwitchLayoutImpl(args);
		}

		static LayoutManager& Get()
		{
			static LayoutManager instance;
			return instance;
		}

		static inline bool DrawLayout()
		{
			return Get().DrawLayoutImpl();
		}

	private:
		LayoutManager();
		LayoutManager(const LayoutManager&) = delete;

		bool DrawLayoutImpl();
		BUTTON_CALLBACK_FUNC(SwitchLayoutImpl);

	private:
		Layout* m_CurrentLayout = {};
		bool m_NeedSwitchLayout = false;
};
} /* namespace SIGame::Core */

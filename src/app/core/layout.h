#pragma once
#include "app/common.h"

#include "json.hpp"

#include <vector>

namespace SIGame::Core
{
using json = nlohmann::json;
/* TODO: In future, probably, some other type should be (maybe with args) */
typedef void (*button_callback_t)(...);
struct Button
{
	std::string Label;
	ImVec2 Size;
	ImVec2 Position; /* Upper left corner position */
	button_callback_t pButtonPressedCallback;
	/* TODO: Add arguments field, but dunno what type for now */
};

enum class ItemType
{
	Button,
};

struct Item
{
	ItemType Type;
	void* pItem;
};

struct LayoutWindow
{
	std::string Label;
	ImVec2 Size;
	ImVec2 Position;
	ImGuiWindowFlags Flags;
	std::vector<Item> Items;
};

class LayoutManager
{
	/* TODO: Remove default constructors, call explicitly destructor somewhere */
	public:
		static bool LoadLayout(const std::string& layout_name);
		static bool DrawLayout();

	private:
		static inline std::vector<LayoutWindow> m_CurrentLayoutStack = {};
		static inline json m_Json = json(nullptr);
};
} /* namespace SIGame::Core */
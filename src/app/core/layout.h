#pragma once
#include "app/common.h"

#include "json.hpp"

#include <vector>

namespace SIGame::Core
{
using json = nlohmann::json;
typedef void (*button_callback_t)(const std::vector<std::string>&);

struct Button
{
	std::string Label;
	ImVec2 Size;
	ImVec2 Position; /* Upper left corner position */
	button_callback_t pButtonPressedCallback;
	std::vector<std::string> CallbackArgs;
};

enum class ItemType
{
	Button,
};

struct Item
{
	ItemType Type;
	void* pItem;

	/* Cleanup heap pointers */
	~Item()
	{
		delete pItem;
		pItem = nullptr;
	}
};

struct LayoutWindow
{
	std::string Label;
	ImVec2 Size;
	ImVec2 Position; /* Upper left corner position */
	ImGuiWindowFlags Flags;
	std::vector<Item*> Items;

	/* Cleanup all heap pointers on destruction */
	~LayoutWindow()
	{
		for(auto item : Items)
		{
			delete item;
			item = nullptr;
		}
	}

	/* Very ugly, and needed only for std::remove in DrawLayout */
	bool operator==(const LayoutWindow& right)
	{
		return
			(this->Label		== right.Label)			&&
			(this->Size.x		== right.Size.x)		&&
			(this->Size.y		== right.Size.y)		&&
			(this->Flags		== right.Flags)			&&
			(this->Position.x	== right.Position.x)	&&
			(this->Position.y	== right.Position.y);
	}
};

class LayoutManager
{
	/* TODO: Remove default constructors, call explicitly destructor somewhere */
	public:
		static bool LoadLayout(const std::string& layout_name);
		static bool DrawLayout();

	private:
		static bool applyLayout(const json& layout_data);

	private:
		static inline std::vector<LayoutWindow> m_CurrentLayoutStack = {};
		static inline json m_Json = json(nullptr);

		static std::unordered_map<std::string, void*> m_ButtonCallbackMap;
};
} /* namespace SIGame::Core */
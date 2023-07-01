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
		for(auto* item : Items)
		{
			delete item;
			item = nullptr;
		}
	}
};

class LayoutManager
{
	public:
		static LayoutManager& Get()
		{
			static LayoutManager instance;
			return instance;
		}

		static inline bool LoadLayout(const std::string& layout_name)
		{
			return Get().LoadLayoutImpl(layout_name);
		}
		static inline bool DrawLayout()
		{
			return Get().DrawLayoutImpl();
		}

	private:
		LayoutManager() {}
		LayoutManager(const LayoutManager&) = delete;

		bool LoadLayoutImpl(const std::string& layout_name);
		bool DrawLayoutImpl();
		bool applyLayout(const json& layout_data);

	private:
		std::vector<LayoutWindow*> m_CurrentLayoutStack = {};
		json m_Json = json(nullptr);

		std::unordered_map<std::string, void*> m_ButtonCallbackMap;
};
} /* namespace SIGame::Core */
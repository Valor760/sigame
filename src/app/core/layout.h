#pragma once
#include "app/common.h"

#include "json.hpp"

#include <vector>

#define ADD_BUTTON_CALLBACK(func) (void*)func, #func
#define BUTTON_CALLBACK_FUNC(func) void func (const std::vector<std::string>& args)

namespace SIGame::Core
{
using json = nlohmann::json;
typedef void (*button_callback_t)(const std::vector<std::string>&);

enum class ItemType
{
	Button, Text
};

struct Button
{
	std::string Label;
	ImVec2 Size;
	ImVec2 Position; /* Upper left corner position */
	button_callback_t pButtonPressedCallback;
	std::vector<std::string> CallbackArgs;
};

struct Text
{
	std::string Label;
	ImVec2 Position; /* Upper left corner position */
	// float FontSize;
	// ImFontAtlas Font;
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
	std::vector<std::shared_ptr<Item>> Items;
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

		static void AddButtonCallback(void* func, std::string func_name);

	private:
		LayoutManager() {}
		LayoutManager(const LayoutManager&) = delete;

		bool LoadLayoutImpl(const std::string& layout_name);
		bool DrawLayoutImpl();
		bool applyLayout(const json& layout_data);

	private:
		std::vector<std::shared_ptr<LayoutWindow>> m_CurrentLayoutStack = {};
		json m_Json = json(nullptr);
};
} /* namespace SIGame::Core */
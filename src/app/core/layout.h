#pragma once
#include "app/common.h"

#include "json.hpp"

#include <vector>
#include <variant>

#define ADD_BUTTON_CALLBACK(func) LayoutManager::AddButtonCallback((void*)func, #func)
#define BUTTON_CALLBACK_FUNC(func) void func (const std::vector<std::string>& args)

namespace SIGame::App::Core
{
using json = nlohmann::json;
typedef void (*button_callback_t)(const std::vector<std::string>&);

enum class ItemType
{
	None, Button, Text
};

struct Button
{
	Button() : Label(""), Size(0, 0), Position(0, 0), pButtonPressedCallback(nullptr), CallbackArgs() {}

	std::string Label;
	ImVec2 Size;
	ImVec2 Position; /* Upper left corner position */
	button_callback_t pButtonPressedCallback;
	std::vector<std::string> CallbackArgs;
};

struct Text
{
	Text() : Label(""), Position(0, 0) {}

	std::string Label;
	ImVec2 Position; /* Upper left corner position */
	// float FontSize;
	// ImFontAtlas Font;
};

struct Item
{
	Item() : Type(ItemType::None) {}

	ItemType Type;
	std::variant<
		Button, Text
	> objItem;
};

struct LayoutWindow
{
	std::string Label;
	ImVec2 Size;
	ImVec2 Position; /* Upper left corner position */
	ImGuiWindowFlags Flags;
	std::vector<Item> Items;
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

		static inline void PushLayout(std::shared_ptr<LayoutWindow> layout)
		{
			Get().PushLayoutImpl(layout);
		}

		static inline void PopLayout(const std::string& layout_name)
		{
			Get().PopLayoutImpl(layout_name);
		}

		static void AddButtonCallback(void* func, std::string func_name);

	private:
		LayoutManager() {}
		LayoutManager(const LayoutManager&) = delete;

		bool LoadLayoutImpl(const std::string& layout_name);
		bool DrawLayoutImpl();
		void PushLayoutImpl(std::shared_ptr<LayoutWindow> layout);
		void PopLayoutImpl(const std::string& layout_name);

		bool applyLayout(const json& layout_data);

	private:
		std::vector<std::shared_ptr<LayoutWindow>> m_CurrentLayoutStack = {};
		/* FIXME: Probably can be a better way? */
		std::vector<std::string> m_LayoutsToRemove = {};

		json m_Json = json(nullptr);
};
} /* namespace SIGame::Core */

#include "layout.h"
#include "app/window.h"

#include <fstream>

#define GET_JSON_STR(x) (x.get<std::string>())
#define GET_JSON_CSTR(x) (GET_JSON_STR(x).c_str())
#define GET_JSON_VEC2(x) (ImVec2(x[0], x[1]))
#define GET_JSON_INT(x) (x.get<int>())

/* TODO: Remove and make function*/
#define ADD_CALLBACK_TO_MAP(func) { #func, (void*)func },

namespace SIGame::Core
{
void TestButtonCallback()
{
	LOG_INFO("Button pressed!!");
}

static std::unordered_map<std::string, void*> m_ButtonCallbackMap = {
	ADD_CALLBACK_TO_MAP(TestButtonCallback)
};

static std::unordered_map<std::string, ItemType> g_item_type_map = {
	{"Button", ItemType::Button},
};

static ItemType get_item_type(const std::string& type_name)
{
	return g_item_type_map[type_name];
}

static std::string get_item_type(ItemType type_enum)
{
	for(auto& [key, val] : g_item_type_map)
	{
		if(val == type_enum)
		{
			return key;
		}
	}

	return "";
}

/* FIXME: Either control the lifetime of these pointers or use std::unique_ptr */
static Button* fill_button_item(const json& button)
{
	/* FIXME: Replace everything with try-catch, if the field is abscent */
	Button* bt = new Button();
	bt->Label = GET_JSON_STR(button["Label"]);
	bt->Size = GET_JSON_VEC2(button["Size"]);
	bt->Position = GET_JSON_VEC2(button["Position"]);
	bt->pButtonPressedCallback = (button_callback_t) m_ButtonCallbackMap[GET_JSON_STR(button["PressedCallbackName"])];

	return bt;
}

static Item* fill_item(const json& item)
{
	Item* it = new Item();
	it->Type = get_item_type(GET_JSON_STR(item["Type"]));

	switch(it->Type)
	{
		case ItemType::Button:
		{
			it->pItem = fill_button_item(item["Data"]);
			break;
		}
		default:
		{
			LOG_ERROR("Unknown item type received: %d", it->Type);
			it->pItem = nullptr;
			break;
		}
	}

	return it;
}

bool LayoutManager::applyLayout(const json& layout_data)
{
	LOG_DEBUG("Applying layout \"%s\"", GET_JSON_CSTR(layout_data["Name"]));

	for(auto& window : layout_data["Windows"])
	{
		LayoutWindow w = {
			/* FIXME: Add constants/defines for json entries */
			.Label = GET_JSON_STR(window["Label"]),
			.Size = GET_JSON_VEC2(window["Size"]),
			.Position = GET_JSON_VEC2(window["Position"]),
			/* TODO: Add suitable flag parsing for flags */
			.Flags = GET_JSON_INT(window["Flags"]),
		};

		/* Add items */
		if(window["Items"].size())
		{
			for(auto& item : window["Items"])
			{
				w.Items.push_back(fill_item(item));
			}
		}
		m_CurrentLayoutStack.push_back(w);
	}

	return true;
}

bool LayoutManager::LoadLayout(const std::string& layout_name)
{
	if(m_Json.is_null())
	{
		/* TODO: Make relative from whole project or copy all assests to output dir */
		std::ifstream json_file("assets/layout.json");
		m_Json = json::parse(json_file);

		if(!m_Json.is_object())
		{
			LOG_ERROR("Failed to load load JSON");
			return false;
		}
	}

	/* TODO: Add check for null */
	auto layouts = m_Json["Layout"];
	json target_layout = json(nullptr);
	for(auto& l : layouts)
	{
		if(l["Name"] == layout_name)
		{
			LOG_DEBUG("Found layout with name \"%s\"", layout_name.c_str());
			target_layout = l;
			break;
		}
	}

	if(target_layout.is_null())
	{
		LOG_ERROR("Failed to find layout name \"%s\"", layout_name.c_str());
		return false;
	}

	return applyLayout(target_layout);
}

void draw_button(Button* button)
{
	/* TODO: Do an input check (if pos or size is 0, and callback nullptr) */
	ImGui::SetCursorPos(button->Position);
	if(ImGui::Button(button->Label.c_str(), button->Size))
	{
		button->pButtonPressedCallback();
	}
}

bool LayoutManager::DrawLayout()
{
	for(auto& window : m_CurrentLayoutStack)
	{
		/* Setup and draw window */
		/* TODO: Do an input check (if pos or size is 0) */
		ImGui::SetNextWindowPos(window.Position);
		ImGui::SetNextWindowSize(window.Size);
		ImGui::Begin(window.Label.c_str(), nullptr, window.Flags);

		for(auto& item : window.Items)
		{
			switch(item->Type)
			{
				case ItemType::Button:
				{
					draw_button((Button*)item->pItem);
					break;
				}
				default:
				{
					LOG_ERROR("Unknown item type received for rendering [%d]", item->Type);
					break;
				}
			}
		}

		ImGui::End();
	}

	return true;
}

} /* namespace SIGame::Core */
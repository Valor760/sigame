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

	if(button.contains("Label"))
	{
		bt->Label = GET_JSON_STR(button["Label"]);
	}
	else
	{
		LOG_ERROR("Button doesn't have \"%s\" field!", "Label");
		return nullptr;
	}

	if(button.contains("Size"))
	{
		bt->Size = GET_JSON_VEC2(button["Size"]);
	}
	else
	{
		LOG_ERROR("Button \"%s\" doesn't have \"%s\" field. Don't know what size to choose.", bt->Label.c_str(), "Size");
		return nullptr;
	}

	if(button.contains("Position"))
	{
		bt->Position = GET_JSON_VEC2(button["Position"]);
	}
	else
	{
		bt->Position = ImVec2(0, 0);
		LOG_INFO("Button \"%s\" doesn't have \"%s\" field. Defaulting to (%d, %d)",
			bt->Label.c_str(), "Position", (int)bt->Position.x, (int)bt->Position.y);
	}

	if(button.contains("PressedCallbackName"))
	{
		try
		{
			bt->pButtonPressedCallback = (button_callback_t) m_ButtonCallbackMap[GET_JSON_STR(button["PressedCallbackName"])];
		}
		catch(...)
		{
			bt->pButtonPressedCallback = nullptr;
			LOG_ERROR("Button \"%s\": Can't find \"%s\" callback function.");
		}
	}
	else
	{
		LOG_ERROR("Button \"%s\" doesn't have \"%s\" field. Why such useless button exists?",
			bt->Label.c_str(), "PressedCallbackName");
	}

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
		/* FIXME: Add constants/defines for json entries */
		LayoutWindow w;

		/* Unfortunately, json library crashes with assert if the element is not present, so can't use try-catch */
		/*
			TODO?: I can rewrite that block of code to not use assertion, but this
			will mean, that we cannot update this library at all.
		*/
		if(window.contains("Label"))
		{
			w.Label = GET_JSON_STR(window["Label"]);
		}
		else
		{
			LOG_ERROR("Window doesn't contain \"Label\" field");
			return false;
		}

		if(window.contains("Size"))
		{
			w.Size = GET_JSON_VEC2(window["Size"]);
		}
		else
		{
			w.Size = Window::GetSize();
			LOG_INFO("Window \"%s\" doesn't have \"%s\" field. Defaulting to window size (%d, %d)",
				w.Label.c_str(), "Size", (int)w.Size.x, (int)w.Size.y);
		}

		if(window.contains("Position"))
		{
			w.Position = GET_JSON_VEC2(window["Position"]);
		}
		else
		{
			w.Position = ImVec2(0, 0);
			LOG_INFO("Window \"%s\" doesn't have \"%s\" field. Defaulting to (%d, %d)",
				w.Label.c_str(), "Position", (int)w.Position.x, (int)w.Position.y);
		}

		if(window.contains("Flags"))
		{
			/* TODO: Add suitable flag parsing for flags */
			w.Flags = GET_JSON_INT(window["Flags"]);
		}
		else
		{
			w.Flags = 0;
			LOG_INFO("Window \"%s\" doesn't have \"%s\" field. Defaulting to %d",
				w.Label.c_str(), "Flags", w.Flags);
		}

		if(window.contains("Items"))
		{
			auto items = window["Items"];
			for(auto& item : items)
			{
				w.Items.push_back(fill_item(item));
			}
		}
		else
		{
			LOG_INFO("Window \"%s\" doesn't have \"%s\" field. Array will be empty.", 
				w.Label.c_str(), "Items");
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
		if(button->pButtonPressedCallback)
		{
			button->pButtonPressedCallback();
		}
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
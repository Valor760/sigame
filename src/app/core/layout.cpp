#include "layout.h"
#include "app/window.h"

#include <fstream>

#define GET_JSON_TYPE(json, type) (json.get<type>())
#define GET_JSON_CSTR(x) (GET_JSON_TYPE(x, std::string).c_str())
#define GET_JSON_VEC2(x) (ImVec2(x[0], x[1]))

/* TODO: Remove and make function*/
#define ADD_CALLBACK_TO_MAP(func) { #func, (void*)func },

namespace SIGame::Core
{
void TestButtonCallback(const std::vector<std::string>& args)
{
	LOG_INFO("Button pressed!!");

	std::string arg_str = "";
	for(auto& arg : args)
	{
		arg_str += arg + ", ";
	}
	LOG_INFO("Args passed: %s", arg_str.c_str());
}

static std::unordered_map<std::string, void*> m_ButtonCallbackMap = {
	ADD_CALLBACK_TO_MAP(TestButtonCallback)
};

static std::unordered_map<std::string, ItemType> g_item_type_map = {
	{"Button", ItemType::Button},
};

/* TODO: Expose possible flags in the header later */
static std::unordered_map<std::string, ImGuiWindowFlags> g_window_flags_map = {
	{"Flag_None",						ImGuiWindowFlags_None},
	{"Flag_NoTitleBar",					ImGuiWindowFlags_NoTitleBar},
	{"Flag_NoResize",					ImGuiWindowFlags_NoResize},
	{"Flag_NoMove",						ImGuiWindowFlags_NoMove},
	{"Flag_NoScrollbar",				ImGuiWindowFlags_NoScrollbar},
	{"Flag_NoScrollWithMouse",			ImGuiWindowFlags_NoScrollWithMouse},
	{"Flag_NoCollapse",					ImGuiWindowFlags_NoCollapse},
	{"Flag_AlwaysAutoResize",			ImGuiWindowFlags_AlwaysAutoResize},
	{"Flag_NoBackground",				ImGuiWindowFlags_NoBackground},
	{"Flag_NoSavedSettings",			ImGuiWindowFlags_NoSavedSettings},
	{"Flag_NoMouseInputs",				ImGuiWindowFlags_NoMouseInputs},
	{"Flag_MenuBar",					ImGuiWindowFlags_MenuBar},
	{"Flag_HorizontalScrollbar",		ImGuiWindowFlags_HorizontalScrollbar},
	{"Flag_NoFocusOnAppearing",			ImGuiWindowFlags_NoFocusOnAppearing},
	{"Flag_NoBringToFrontOnFocus",		ImGuiWindowFlags_NoBringToFrontOnFocus},
	{"Flag_AlwaysVerticalScrollbar",	ImGuiWindowFlags_AlwaysVerticalScrollbar},
	{"Flag_AlwaysHorizontalScrollbar",	ImGuiWindowFlags_AlwaysHorizontalScrollbar},
	{"Flag_AlwaysUseWindowPadding",		ImGuiWindowFlags_AlwaysUseWindowPadding},
	{"Flag_NoNavInputs",				ImGuiWindowFlags_NoNavInputs},
	{"Flag_NoNavFocus",					ImGuiWindowFlags_NoNavFocus},
	{"Flag_UnsavedDocument",			ImGuiWindowFlags_UnsavedDocument},
	{"Flag_NoNav",						ImGuiWindowFlags_NoNav},
	{"Flag_NoDecoration",				ImGuiWindowFlags_NoDecoration},
	{"Flag_NoInputs",					ImGuiWindowFlags_NoInputs},
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
	Button* bt = new Button();

	if(button.contains("Label"))
	{
		bt->Label = button["Label"];
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
			bt->pButtonPressedCallback = (button_callback_t) m_ButtonCallbackMap[button["PressedCallbackName"]];
		}
		catch(...)
		{
			bt->pButtonPressedCallback = nullptr;
			LOG_ERROR("Button \"%s\": Can't find \"%s\" callback function.");
		}

		if(button.contains("Args"))
		{
			bt->CallbackArgs = button["Args"];
		}
		else
		{
			bt->CallbackArgs = {};
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
	it->Type = get_item_type((std::string)item["Type"]);

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
			w.Label = window["Label"];
		}
		else
		{
			LOG_ERROR("Window doesn't contain \"%s\" field", "Label");
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
			w.Flags = 0;

			if(window["Flags"].is_array())
			{
				const std::vector<std::string>& window_flags = window["Flags"];
				for(auto& flag : window_flags)
				{
					try
					{
						w.Flags |= g_window_flags_map[flag];
					}
					catch(...)
					{
						LOG_ERROR("Unknown window flag \'%s\' received. Skip.", flag.c_str());
					}
				}
			}
			else
			{
				LOG_ERROR("Expected array of flags, but got \'%s\' instead.", window["Flags"].type_name());
			}
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
		std::ifstream json_file("assets/layout.json");
		m_Json = json::parse(json_file);

		if(!m_Json.is_object())
		{
			LOG_ERROR("Failed to load JSON");
			return false;
		}
	}

	if(!m_Json.contains("Layout"))
	{
		LOG_ERROR("Provided JSON is invalid. Missing \"%s\" field in the root.", "Layout");
		return false;
	}

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
			button->pButtonPressedCallback(button->CallbackArgs);
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
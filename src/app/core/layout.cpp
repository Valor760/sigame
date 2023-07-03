#include "layout.h"
#include "app/window.h"

#include <fstream>
#include <algorithm>

#define GET_JSON_TYPE(json, type) (json.get<type>())
#define GET_JSON_CSTR(x) (GET_JSON_TYPE(x, std::string).c_str())
#define GET_JSON_VEC2(x) (ImVec2(x[0], x[1]))

namespace SIGame::Core
{
namespace JsonEntry
{
inline const char* Label			= "Label";
inline const char* Size				= "Size";
inline const char* Position			= "Position";
inline const char* PressedCallback	= "PressedCallbackName";
inline const char* CallbackArgs		= "Args";
inline const char* ItemType			= "Type";
inline const char* Data				= "Data";
inline const char* Windows			= "Windows";
inline const char* Flags			= "Flags";
inline const char* Items			= "Items";
inline const char* Layout			= "Layout";
inline const char* Name				= "Name";
inline const char* Button			= "Button";
inline const char* Text				= "Text";
} /* namespace JsonEntry */

static std::unordered_map<std::string, void*> m_ButtonCallbackMap;

static std::unordered_map<std::string, ItemType> g_ItemTypeMap = {
	{JsonEntry::Button, ItemType::Button},
	{JsonEntry::Text, ItemType::Text},
};

/* TODO: Expose possible flags in the header later */
static std::unordered_map<std::string, ImGuiWindowFlags> g_WindowFlagsMap = {
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
	return g_ItemTypeMap[type_name];
}

static std::string get_item_type(ItemType type_enum)
{
	for(auto& [key, val] : g_ItemTypeMap)
	{
		if(val == type_enum)
		{
			return key;
		}
	}

	return "";
}

static Button* fill_button_item(const json& button)
{
	Button* bt = new Button();

	if(button.contains(JsonEntry::Label))
	{
		bt->Label = button[JsonEntry::Label];
	}
	else
	{
		LOG_ERROR("Button doesn't have \"%s\" field!", JsonEntry::Label);
		return nullptr;
	}

	if(button.contains(JsonEntry::Size))
	{
		bt->Size = GET_JSON_VEC2(button[JsonEntry::Size]);
	}
	else
	{
		LOG_ERROR("Button \"%s\" doesn't have \"%s\" field. Don't know what size to choose.", bt->Label.c_str(), JsonEntry::Size);
		return nullptr;
	}

	if(button.contains(JsonEntry::Position))
	{
		bt->Position = GET_JSON_VEC2(button[JsonEntry::Position]);
	}
	else
	{
		bt->Position = ImVec2(0, 0);
		LOG_INFO("Button \"%s\" doesn't have \"%s\" field. Defaulting to (%d, %d)",
			bt->Label.c_str(), JsonEntry::Position, (int)bt->Position.x, (int)bt->Position.y);
	}

	if(button.contains(JsonEntry::PressedCallback))
	{
		try
		{
			bt->pButtonPressedCallback = (button_callback_t) m_ButtonCallbackMap[button[JsonEntry::PressedCallback]];
		}
		catch(...)
		{
			bt->pButtonPressedCallback = nullptr;
			LOG_ERROR("Button \"%s\": Can't find \"%s\" callback function.");
		}

		if(button.contains(JsonEntry::CallbackArgs))
		{
			bt->CallbackArgs = button[JsonEntry::CallbackArgs];
		}
		else
		{
			bt->CallbackArgs = {};
		}
	}
	else
	{
		LOG_ERROR("Button \"%s\" doesn't have \"%s\" field. Why such useless button exists?",
			bt->Label.c_str(), JsonEntry::PressedCallback);
	}

	return bt;
}

static Text* fill_text_item(const json& text)
{
	Text* txt = new Text();

	if(text.contains(JsonEntry::Label))
	{
		txt->Label = text[JsonEntry::Label];
	}
	else
	{
		LOG_ERROR("Cannot find text(Label) for text field");
		return nullptr;
	}

	if(text.contains(JsonEntry::Position))
	{
		txt->Position = GET_JSON_VEC2(text[JsonEntry::Position]);
	}
	else
	{
		txt->Position = ImVec2(0, 0);
		LOG_DEBUG("Some text[\"%s...\"] doesn't have position. Defaulting to (%d, %d)",
			txt->Label.substr(0, 20).c_str(),
			(int)txt->Position.x,
			(int)txt->Position.y);
	}

	return txt;
}

static std::shared_ptr<Item> fill_item(const json& item)
{
	std::shared_ptr<Item> it = std::make_shared<Item>();
	it->Type = get_item_type(std::string(item[JsonEntry::ItemType]));

	switch(it->Type)
	{
		case ItemType::Button:
		{
			it->pItem = fill_button_item(item[JsonEntry::Data]);
			break;
		}
		case ItemType::Text:
		{
			it->pItem = fill_text_item(item[JsonEntry::Data]);
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
	LOG_DEBUG("Applying layout \"%s\"", GET_JSON_CSTR(layout_data[JsonEntry::Name]));

	/* Empty current layout stack */
	if(m_CurrentLayoutStack.size())
	{
		m_CurrentLayoutStack.clear();
	}

	for(auto& window : layout_data[JsonEntry::Windows])
	{
		std::shared_ptr<LayoutWindow> w = std::make_shared<LayoutWindow>();

		/*
			Unfortunately, json library crashes with assert if the element is not present, so can't use try-catch
			TODO?: I can rewrite that block of code to not use assertion, but this
			will mean, that we cannot update this library at all.
		*/
		if(window.contains(JsonEntry::Label))
		{
			w->Label = window[JsonEntry::Label];
		}
		else
		{
			LOG_ERROR("Window doesn't contain \"%s\" field", JsonEntry::Label);
			return false;
		}

		if(window.contains(JsonEntry::Size))
		{
			w->Size = GET_JSON_VEC2(window[JsonEntry::Size]);
		}
		else
		{
			w->Size = Window::GetSize();
			LOG_INFO("Window \"%s\" doesn't have \"%s\" field. Defaulting to window size (%d, %d)",
				w->Label.c_str(), JsonEntry::Size, (int)w->Size.x, (int)w->Size.y);
		}

		if(window.contains(JsonEntry::Position))
		{
			w->Position = GET_JSON_VEC2(window[JsonEntry::Position]);
		}
		else
		{
			w->Position = ImVec2(0, 0);
			LOG_INFO("Window \"%s\" doesn't have \"%s\" field. Defaulting to (%d, %d)",
				w->Label.c_str(), JsonEntry::Position, (int)w->Position.x, (int)w->Position.y);
		}

		if(window.contains(JsonEntry::Flags))
		{
			w->Flags = 0;

			if(window[JsonEntry::Flags].is_array())
			{
				const std::vector<std::string>& window_flags = window[JsonEntry::Flags];
				for(auto& flag : window_flags)
				{
					try
					{
						w->Flags |= g_WindowFlagsMap[flag];
					}
					catch(...)
					{
						LOG_ERROR("Unknown window flag \'%s\' received. Skip.", flag.c_str());
					}
				}
			}
			else
			{
				LOG_ERROR("Expected array of flags, but got \'%s\' instead.", window[JsonEntry::Flags].type_name());
			}
		}
		else
		{
			w->Flags = 0;
			LOG_INFO("Window \"%s\" doesn't have \"%s\" field. Defaulting to %d",
				w->Label.c_str(), JsonEntry::Flags, w->Flags);
		}

		if(window.contains(JsonEntry::Items))
		{
			auto items = window[JsonEntry::Items];
			for(auto& item : items)
			{
				w->Items.push_back(fill_item(item));
			}
		}
		else
		{
			LOG_INFO("Window \"%s\" doesn't have \"%s\" field. Array will be empty.", 
				w->Label.c_str(), JsonEntry::Items);
		}

		m_CurrentLayoutStack.push_back(w);
	}

	return true;
}

/* FIXME: Create custom exception class and make everything using exceptions */
bool LayoutManager::LoadLayoutImpl(const std::string& layout_name)
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

	if(!m_Json.contains(JsonEntry::Layout))
	{
		LOG_ERROR("Provided JSON is invalid. Missing \"%s\" field in the root.", JsonEntry::Layout);
		return false;
	}

	auto layouts = m_Json[JsonEntry::Layout];
	json target_layout = json(nullptr);
	for(auto& l : layouts)
	{
		if(l[JsonEntry::Name] == layout_name)
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

/* TODO: Move all 'draw' functions to separate file/renderer class */
static void draw_button(Button* button)
{
	if(!button)
	{
		return;
	}

	if(button->Size.x <= 0 || button->Size.y <= 0)
	{
		/* Remove button from rendering */
		LOG_DEBUG("Button \'%s\' has 0 size. Removing it from rendering!", button->Label);
		delete button;
		button = nullptr;
	}

	/* TODO: Add ImGui::PushStyleColor */
	ImGui::SetCursorPos(button->Position);
	if(ImGui::Button(button->Label.c_str(), button->Size))
	{
		if(button->pButtonPressedCallback)
		{
			button->pButtonPressedCallback(button->CallbackArgs);
		}
	}
}

bool LayoutManager::DrawLayoutImpl()
{
	for(auto& window : m_CurrentLayoutStack)
	{
		/* Setup and draw window */
		if(window->Size.x <= 0 || window->Size.y <= 0)
		{
			/* Remove window from render if window size is 0. */
			LOG_DEBUG("Window \'%s\' has 0 size. Removing it from render stack!", window->Label);
			m_CurrentLayoutStack.erase(
				std::remove(m_CurrentLayoutStack.begin(), m_CurrentLayoutStack.end(), window),
				m_CurrentLayoutStack.end()
				);
			continue;
		}

		ImGui::SetNextWindowPos(window->Position);
		ImGui::SetNextWindowSize(window->Size);
		ImGui::Begin(window->Label.c_str(), nullptr, window->Flags);

		for(auto item : window->Items)
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
					LOG_DEBUG("Unknown item type received for rendering [%d]", item->Type);
					break;
				}
			}
		}

		ImGui::End();
	}

	return true;
}

void LayoutManager::AddButtonCallback(void* func, std::string func_name)
{
	m_ButtonCallbackMap[func_name] = func;
}

} /* namespace SIGame::Core */
#include "layout.h"
#include "app/window.h"

#include <fstream>

namespace SIGame::Core
{
/* FIXME: Remove this later (probably, move to json file) */
// MENU CONST
void test_butt_press()
{
	LOG_INFO("Button pressed!!");
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

	/* FIXME: Think how to make it prettier. Right now is very ugly, imo */
	/* TODO: Add check for null */
	auto layouts = m_Json["Layout"];
	int layout_idx;
	for(layout_idx = 0; layout_idx < layouts.size(); layout_idx++)
	{
		if(layouts[layout_idx]["Name"] == layout_name)
		{
			break;
		}
	}

	/* TODO: Load layout */

	// if(layout.is_null())
	// {
	// 	LOG_ERROR("Failed to load layout with name \"%s\"", layout_name.c_str());
	// 	return false;
	// }

	return true;
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
			/* TODO: Make a switch statement with call to setup function(with casting) depending on ItemType */
			draw_button((Button*)item.pItem);
		}

		ImGui::End();
	}

	return true;
}

} /* namespace SIGame::Core */
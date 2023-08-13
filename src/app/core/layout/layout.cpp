#include "layout.h"
#include "app/window.h"

namespace SIGame::App::Core
{

extern Layout Layout_MainMenu;
extern Layout Layout_SinglePlayer;
static std::vector<Layout> g_Layouts = {};

/* Call add all layouts to global array in constructor */
LayoutManager::LayoutManager()
{
	g_Layouts = {
		Layout_MainMenu, Layout_SinglePlayer
	};
}

/* TODO: Move all 'draw' functions to separate file/renderer class */
static bool draw_button(const Button& button)
{
	if(button.Size.x <= 0 || button.Size.y <= 0)
	{
		/* Remove button from rendering */
		//LOG_DEBUG("Button \'%s\' has 0 size. Removing it from rendering!", button.Label);
		return false;
	}

	/* TODO: Add ImGui::PushStyleColor */
	ImGui::SetCursorPos(button.Position);
	if(ImGui::Button(button.Label.c_str(), button.Size))
	{
		if(button.pButtonPressedCallback)
		{
			button.pButtonPressedCallback(button.CallbackArgs);
			return true;
		}
	}

	return false;
}

bool LayoutManager::DrawLayoutImpl()
{
	for(auto& window : m_CurrentLayout.LayoutWindowStack)
	{
		/* Setup and draw window */
		/* If {0, 0} then fullscreen? */
		// if(window.Size.x <= 0 || window.Size.y <= 0)
		// {
		// 	/* Remove window from render if window size is 0. */
		// 	continue;
		// }

		ImGui::SetNextWindowPos(window.Position);
		ImGui::SetNextWindowSize(window.Size);
		ImGui::Begin(window.Label.c_str(), nullptr, window.Flags);

		for(auto& item : window.Items)
		{
			switch(item.Type)
			{
				case ItemType::Button:
				{
					draw_button(std::get<Button>(item.objItem));
					/* FIXME: How else I can track, that we need to switch layout and avoid various errors? */
					if(m_NeedSwitchLayout)
					{
						m_NeedSwitchLayout = false;
						ImGui::End();
						goto Exit;
					}
					break;
				}
				default:
				{
					LOG_DEBUG("Unknown item type received for rendering [%d]", item.Type);
					break;
				}
			}
		}
		ImGui::End();
	}

Exit:
	return true;
}

BUTTON_CALLBACK_FUNC(LayoutManager::SwitchLayoutImpl)
{
	if(args.size() != 1)
	{
		LOG_ERROR("Wrong number of arguments provided. Expected[1], but got[%d]", args.size());
		return;
	}

	const std::string search_name = args[0];
	bool found_layout = false;
	for(auto layout : g_Layouts)
	{
		if(layout.LayoutName == search_name)
		{
			found_layout = true;
			m_CurrentLayout = layout;
			m_NeedSwitchLayout = true;
			LOG_DEBUG("Switched layout to %s", search_name.c_str());
			break;
		}
	}

	if(!found_layout)
	{
		LOG_ERROR("Failed to find layout - %s", search_name.c_str());
	}
}

} /* namespace SIGame::Core */
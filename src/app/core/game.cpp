#include "game.h"
#include "app/utils/zip.h"

#include "ImGuiFileDialog.h"

namespace SIGame::App::Core
{
Game::Game()
{
	ADD_BUTTON_CALLBACK(SelectSIQPaket);
	ADD_BUTTON_CALLBACK(DoStuff);
}

/* TODO: Remove */
#define TEST_PAKET "test_bad.siq"

BUTTON_CALLBACK_FUNC(Game::SelectSIQPaket)
{
	/* TODO: Add selection dialog */
	const char* paket = ASSETS_DIR "/" TEST_PAKET;
	Game::SetPaketPath(paket);
	const std::string dialog_name = "ChooseFileDlgKey";
	ImGuiFileDialog fileDialog;
	fileDialog.OpenDialog("ChooseFileDlgKey", "Choose File", nullptr, ".");

	std::string filePathName = "";
	std::string filePath = "";

	ImVec2 maxSize = ImVec2((float)1600, (float)900);  // The full display area
	ImVec2 minSize =  ImVec2((float)800, (float)450);  // Half the display area
	if (fileDialog.Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse, minSize, maxSize))
	{
		if (fileDialog.IsOk())
		{
			filePathName = fileDialog.GetFilePathName();
			filePath = fileDialog.GetCurrentPath();
		}
		fileDialog.Close();
	}

	// LOG_DEBUG("Selected paket: %s", paket);
	LOG_DEBUG("filePathName = %s | filePath = %s", filePathName.c_str(), filePath.c_str());

	/* TODO: Transfer variable with path name to the text callback */
}

BUTTON_CALLBACK_FUNC(Game::DoStuff)
{
	const std::string& paket_path = Game::GetPaketPath();
	if(paket_path.empty())
	{
		return;
	}

	/* Extract our paket */
	Utils::Zip zip(paket_path);
	
	size_t total_files = zip.GetTotalFilesNum();
	std::thread extract_thread(Utils::Zip::Extract, std::ref(zip), SIQ_EXTRACT_DIR);

	size_t files_extracted = 0;
	while((files_extracted = zip.GetExtractedFilesNum()) < total_files)
	{
		LOG_DEBUG("Extracted: %llu/%llu (%d%)", files_extracted, total_files, (files_extracted * 100) / total_files );
	}

	extract_thread.join();
}

void Game::SetPaketPathImpl(const std::string& path)
{
	m_SelectedPaketPath = path;
}

const std::string& Game::GetPaketPathImpl()
{
	return m_SelectedPaketPath;
}
} /* namespace SIGame::Core */

#include "game.h"
#include "app/utils/zip.h"

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
	LOG_DEBUG("Selected paket: %s", paket);

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

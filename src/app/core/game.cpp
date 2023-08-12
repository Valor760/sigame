#include "game.h"
#include "app/utils/zip.h"

#ifdef SI_PLATFORM_WINDOWS
#include <windows.h>
#include <shobjidl.h>
#endif

namespace SIGame::App::Core
{
Game::Game()
{
	ADD_BUTTON_CALLBACK(SelectSIQPaket);
	ADD_BUTTON_CALLBACK(ExtractSIQPaket);
}

static std::string select_file_dialog()
{
	char selected_file[512];
	memset(selected_file, 0, sizeof(selected_file));

#if defined(SI_PLATFORM_WINDOWS)
	IFileOpenDialog *pFileOpen;

	// Create the FileOpenDialog object.
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

	if (SUCCEEDED(hr))
	{
		// Show the Open dialog box.
		hr = pFileOpen->Show(NULL);

		// Get the file name from the dialog box.
		if (SUCCEEDED(hr))
		{
			IShellItem *pItem;
			hr = pFileOpen->GetResult(&pItem);
			if (SUCCEEDED(hr))
			{
				PWSTR pszFilePath;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

				// Display the file name to the user.
				if (SUCCEEDED(hr))
				{
					wcstombs(selected_file, pszFilePath, sizeof(selected_file));
					CoTaskMemFree(pszFilePath);
				}
				pItem->Release();
			}
		}
		pFileOpen->Release();
	}
	CoUninitialize();
#elif defined(SI_PLATFORM_LINUX)
/* TODO: Use execve or std::system and call zenity --file-selection */
/* Save result to the file and read from it (or read directly from output, but it will be more tricky) */
#endif
	return std::string(selected_file);
}

BUTTON_CALLBACK_FUNC(Game::SelectSIQPaket)
{
	std::string selected_paket = select_file_dialog();
	Game::SetPaketPath(selected_paket);
	/* TODO: Transfer variable with path name to the text callback */
}

BUTTON_CALLBACK_FUNC(Game::ExtractSIQPaket)
{
	const std::string& paket_path = Game::GetPaketPath();
	if(paket_path.empty())
	{
		return;
	}

	/* Extract our paket */
	Utils::Zip zip(paket_path);
	
	size_t total_files = zip.GetTotalFilesNum();
	std::thread extract_thread(&Utils::Zip::Extract, std::ref(zip), SIQ_EXTRACT_DIR);

	size_t files_extracted = 0;
	while((files_extracted = zip.GetExtractedFilesNum()) < total_files)
	{
		LOG_DEBUG("Extracted: %llu/%llu (%d%)", files_extracted, total_files, (files_extracted * 100) / total_files);
	}

	extract_thread.join();
}

void Game::SetPaketPathImpl(const std::string& path)
{
	LOG_DEBUG("Selected paket path = %s", path.c_str());
	m_SelectedPaketPath = path;
}

const std::string& Game::GetPaketPathImpl()
{
	return m_SelectedPaketPath;
}
} /* namespace SIGame::Core */

#pragma once
#include "app/common.h"

#include <filesystem>

namespace fs = std::filesystem;

namespace SIGame::App::Utils
{
class Zip
{
	public:
		Zip() {}

		void Extract(const fs::path& zip_path, const fs::path& extract_dir);
		size_t GetTotalFilesNum() const;
		size_t GetExtractedFilesNum() const;

	private:
		size_t m_NumFiles = 0;
		size_t m_NumExtracted = 0;
};
} /* namespace SIGame::App::Utils */
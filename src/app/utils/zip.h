#pragma once
#include "app/common.h"

#include <elzip/unzipper.hpp>

namespace SIGame::App::Utils
{
class Zip
{
	public:
		Zip(const fs::path& zip_path);

		void Extract(const fs::path& extract_dir);

		size_t GetTotalFilesNum() const;
		size_t GetExtractedFilesNum() const;

	private:
		ziputils::unzipper m_ZipInstance;
		bool m_Success = false;
		size_t m_NumFiles = 0;
		size_t m_NumExtracted = 0;
};
} /* namespace SIGame::App::Utils */

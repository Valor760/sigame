#include "zip.h"

#include <elzip/elzip.hpp>
#include <elzip/unzipper.hpp>

/* from elzip.cpp */
namespace elz
{
extern std::string _resolvePath(const std::string& entry);
extern void _extractFile(ziputils::unzipper& zipFile, const fs::path& filename, const fs::path& target, const std::string& password);
}

namespace SIGame::App::Utils
{
void Zip::Extract(const fs::path& zip_path, const fs::path& extract_dir)
{
	/* Code reference was taken from elz::extractZip. I just don't want to change external code for my purposes */
	ziputils::unzipper zip_file;
	if(!zip_file.open(zip_path.c_str()))
	{
		LOG_ERROR("Can't open zip archive: %s", zip_path.c_str());
		/* FIXME: How to handle exceptions, if I call this function in a thread? */
		throw;
	}

	auto files = zip_file.getFilenames();
	m_NumFiles = files.size();

	for(auto& filename : files)
	{
		std::string real_path = elz::_resolvePath(filename);

		fs::path currentDir = extract_dir / std::filesystem::path(real_path).parent_path();
		fs::create_directories(currentDir);

		fs::path currentFile = extract_dir / real_path;

		/* In our case we ignore files that can't be extracted for some reasons */
		try
		{
			elz::_extractFile(zip_file, filename, currentFile.string(), "");
			LOG_DEBUG("Extracted file: %s", filename.c_str());
		}
		catch(...)
		{
			LOG_ERROR("Can't extract file: %s", filename.c_str());
		}

		m_NumExtracted++;
	}
}

size_t Zip::GetTotalFilesNum() const
{
	return m_NumFiles;
}

size_t Zip::GetExtractedFilesNum() const
{
	return m_NumExtracted;
}
} /* namespace SIGame::App::Utils */
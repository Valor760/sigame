#include "zip.h"

#include <elzip/elzip.hpp>

/* from elzip.cpp */
namespace elz
{
extern std::string _resolvePath(const std::string& entry);
extern void _extractFile(ziputils::unzipper& zipFile, const fs::path& filename, const fs::path& target, const std::string& password);
}

namespace SIGame::App::Utils
{
Zip::Zip(const fs::path& zip_path)
{
	/* Code reference was taken from elz::extractZip. I just don't want to change external code for my purposes */
	if(!m_ZipInstance.open(zip_path.string().c_str()))
	{
		LOG_ERROR("Can't open zip archive: %s", zip_path.c_str());
		/* FIXME: How to handle exceptions, if I call this function in a thread? */
		m_Success = false;
		return;
	}

	m_NumFiles = m_ZipInstance.getFilenames().size();

	m_Success = true;
}

void Zip::Extract(const fs::path& extract_dir)
{
	for(auto& filename : m_ZipInstance.getFilenames())
	{
		/* In our case we ignore files that can't be extracted for some reasons */
		try
		{
			std::string real_path = elz::_resolvePath(filename);

			fs::path currentDir = extract_dir / std::filesystem::path(real_path).parent_path();
			fs::create_directories(currentDir);

			fs::path currentFile = extract_dir / real_path;

			elz::_extractFile(m_ZipInstance, filename, currentFile.string(), "");
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
#pragma once
#include "layout/layout.h"

namespace SIGame::App::Core
{
class Game
{
	public:
		static void SelectSIQPaket();
		static bool ExtractSIQPaket();

		static Game& GetInstance()
		{
			static Game inst;
			return inst;
		}

		static inline void SetPaketPath(const std::string& path)
		{
			GetInstance().SetPaketPathImpl(path);
		}

		static inline const std::string& GetPaketPath()
		{
			return GetInstance().GetPaketPathImpl();
		}

	private:
		Game();

		void SetPaketPathImpl(const std::string& path);
		const std::string& GetPaketPathImpl();

		bool extractSIQPaket();

	private:
		std::string m_SelectedPaketPath = "";
};
} /* namespace SIGame::Core */

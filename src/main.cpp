#include "app/app.h"

int main()
{
	SIGame::App::MainApp app{};
	if(app.Init())
	{
		app.Run();
		return 0;
	}
	return -1;
}
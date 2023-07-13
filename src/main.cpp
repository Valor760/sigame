#include "app/app.h"

int main()
{
	SIGame::App app{};
	if(app.Init())
	{
		app.Run();
		return 0;
	}
	return -1;
}
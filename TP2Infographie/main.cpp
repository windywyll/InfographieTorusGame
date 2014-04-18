#include <SDL.h>

#include "DemoMain.h"

#if _DEBUG
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif

using namespace std;

// Program entry point - SDL manages the actual WinMain entry point for us
int main(int argc, char *argv[])
{
	DemoMain &demo = DemoMain::getInstance();
	demo.init();
	demo.run();
	DemoMain::deleteInstance();

    SDL_Quit();
    return 0;
}
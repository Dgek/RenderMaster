#include "App.h"

////////////////////////////////////
//Entry point for the application
////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	auto g_pEngine = make_unique<App>();
}
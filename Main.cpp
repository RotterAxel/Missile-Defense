//Programmer: Axel Rotter Morgado
//Date: 18.04.2016

#include "..\\2DEngine\Main.h"

GameApp g_App;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int iCmdShow)
{

	srand(time(0));

	int retCode;

	if (!g_App.InitInstance(hInstance, lpCmdLine, iCmdShow)) return 0;

	retCode = g_App.BeginGame();

	if (!g_App.ShutDown()) MessageBox(0, _T("Failed to shut down correctly, please check file named 'debug.txt'. \r\n\r\n"), _T("Non-Fatal Error"), MB_OK | MB_ICONEXCLAMATION);

	return retCode;
}
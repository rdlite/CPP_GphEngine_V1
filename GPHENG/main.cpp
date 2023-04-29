#include "AppWindow.h"
#include "iostream"

int main()
{
	try
	{
		GraphicsEngine::create();
		InputSystem::create();
	}
	catch (...)
	{
		return -1;
	}

	try
	{
		AppWindow appWindow;

		while (appWindow.isRunning()) 
		{
			appWindow.broadcastWindow();
		}
	}
	catch (...)
	{
		GraphicsEngine::release();
		InputSystem::release();
	}

	return 0;
}
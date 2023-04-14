#pragma once
#include <Windows.h>

class Window
{
public:
	Window();
	bool init();
	bool broadcastWindow();
	bool release();
	bool isRunning();

	virtual void onCreate() = 0;
	virtual void onUpdate() = 0;
	virtual void onDestroy() = 0;
	virtual void onFocus(bool isFocus) = 0;

	RECT getClientWindowRect();
	void setHWND(HWND hwnd);
protected:
	HWND m_hwnd;
	bool m_isRun;
};
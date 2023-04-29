#pragma once
#include <Windows.h>
#include <exception>

class Window
{
public:
	Window();
	bool broadcastWindow();
	bool isRunning();

	virtual void onCreate() = 0;
	virtual void onUpdate() = 0;
	virtual void onDestroy() = 0;
	virtual void onFocus(bool isFocus) = 0;

	RECT getClientWindowRect();
	~Window();
protected:
	HWND m_hwnd;
	bool m_isRun;
	bool m_isInit = false;
	bool m_isFocused = false;
};
#pragma once
#include "InputListener.h"
#include "unordered_set"
#include "Windows.h"
#include "Point.h"

class InputSystem
{
public:
	static void create();
	static void release();
	void update();
	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);

	void setCursorPosition(const Point& point);
	void setCursorVisible(bool isVisible);

	static InputSystem* get();
private:
	std::unordered_set<InputListener*> m_listeners;
	Point m_oldMousePos;

	static InputSystem* m_system;

	unsigned char m_keysState[256] = {};
	unsigned char m_oldKeysState[256] = {};
	bool m_isFirstTime = true;
};
#pragma once
#include "InputListener.h"
#include "unordered_set"
#include "Windows.h"
#include "Point.h"

class InputSystem
{
public:
	void update();
	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);

	static InputSystem* get();
private:
	std::unordered_set<InputListener*> m_listeners;
	unsigned char m_keysState[256] = {};
	unsigned char m_oldKeysState[256] = {};
	Point m_oldMousePos;
	bool m_isFirstTime = true;
};
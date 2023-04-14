#pragma once
#include "Point.h"

class InputListener
{
public:
	InputListener()
	{
	}
	~InputListener()
	{
	}

	virtual void onKeyDown(int key) = 0;
	virtual void onKeyUp(int key) = 0;

	virtual void onMouseMove(const Point& mousePos) = 0;
	virtual void onMouseButtonDown(int mouseButtonID, const Point& point) = 0;
	virtual void onMouseButtonUp(int mouseButtonID, const Point& point) = 0;
};
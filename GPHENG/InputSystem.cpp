#include "InputSystem.h"

void InputSystem::update()
{
	POINT currentMousePos = {};
	GetCursorPos(&currentMousePos);

	if (m_isFirstTime)
	{
		m_isFirstTime = false;
		m_oldMousePos = Point(currentMousePos.x, currentMousePos.y);
	}

	if (currentMousePos.x != m_oldMousePos.x || currentMousePos.y != m_oldMousePos.y)
	{
		std::unordered_set<InputListener*>::iterator iterator = m_listeners.begin();

		while (iterator != m_listeners.end())
		{
			(*iterator)->onMouseMove(Point(currentMousePos.x - m_oldMousePos.x, currentMousePos.y - m_oldMousePos.y));
			++iterator;
		}
	}

	m_oldMousePos = Point(currentMousePos.x, currentMousePos.y);



	if (GetKeyboardState(m_keysState))
	{
		for (unsigned int i = 0; i < 256; i++)
		{
			if (m_keysState[i] & 0x80) 
			{
				std::unordered_set<InputListener*, InputListener*>::iterator iterator = m_listeners.begin();

				while (iterator != m_listeners.end())
				{
					if (m_keysState[i] != m_oldKeysState[i])
					{
						int mouseID = 0;

						if (i == VK_RBUTTON)
						{
							mouseID = 1;
						}
						else if (i == VK_MBUTTON)
						{
							mouseID = 2;
						}

						(*iterator)->onMouseButtonDown(mouseID, Point(currentMousePos.x, currentMousePos.y));
					}


					(*iterator)->onKeyDown(i);
					++iterator;
				}
			}
			else
			{
				if (m_keysState[i] != m_oldKeysState[i]) 
				{
					std::unordered_set<InputListener*>::iterator iterator = m_listeners.begin();

					while (iterator != m_listeners.end())
					{
						if (i == VK_RBUTTON || i == VK_LBUTTON || i == VK_MBUTTON)
						{
							int mouseID = 0;

							if (i == VK_RBUTTON)
							{
								mouseID = 1;
							}
							else if (i == VK_MBUTTON)
							{
								mouseID = 2;
							}

							(*iterator)->onMouseButtonUp(mouseID, Point(currentMousePos.x, currentMousePos.y));
						}

						(*iterator)->onKeyUp(i);
						++iterator;
					}
				}
			}
		}

		memcpy(m_oldKeysState, m_keysState, sizeof(unsigned char) * 256);
	}
}

void InputSystem::addListener(InputListener* listener)
{
	m_listeners.insert(listener);
}

void InputSystem::removeListener(InputListener* listener)
{
	std::unordered_set<InputListener*>::iterator iterator = m_listeners.find(listener);

	if (iterator != m_listeners.end())
	{
		m_listeners.erase(iterator);
	}
}

InputSystem* InputSystem::get()
{
	static InputSystem inputSystem;
	return &inputSystem;
}
#include "InputUtility.h"

void InputUtility::handleInputEvent(const sf::Event &event)
{
	// Handle mouse button events.
	if(event.type == sf::Event::MouseButtonPressed)
	{
		if(downButtons.count(event.mouseButton.button) == 0)
		{
			pressedButtons.insert(event.mouseButton.button);
		}
		downButtons.insert(event.mouseButton.button);
	}
	if(event.type == sf::Event::MouseButtonReleased)
	{
		pressedButtons.erase(event.mouseButton.button);
		downButtons.erase(event.mouseButton.button);
	}

	// Handle keyboard events.
	if(event.type == sf::Event::KeyPressed)
	{
		if(downKeys.count(event.key.code) == 0)
		{
			pressedKeys.insert(event.key.code);
		}
		downKeys.insert(event.key.code);
	}
	if(event.type == sf::Event::KeyReleased)
	{
		pressedKeys.erase(event.key.code);
		downKeys.erase(event.key.code);
	}

	// Handle mouse scroll event.
	if(event.type == sf::Event::MouseWheelScrolled)
	{
		mouseScrollDelta = event.mouseWheelScroll.delta;
	}
}

void InputUtility::update()
{
	// Reset pressed keys and buttons since they are only considered pressed for a single frame.
	pressedButtons.clear();
	pressedKeys.clear();

	// Reset mouseScrollDelta since if no events triggers it remains fixed.
	mouseScrollDelta = 0;
}

bool InputUtility::getButtonPressed(const sf::Mouse::Button &button) const
{
	return pressedButtons.count(button) == 1;
}

bool InputUtility::getKeyPressed(const sf::Keyboard::Key &key) const
{
	return pressedKeys.count(key) == 1;
}

bool InputUtility::getButtonDown(const sf::Mouse::Button & button) const
{
	return downButtons.count(button) == 1;
}

bool InputUtility::getKeyDown(const sf::Keyboard::Key & key) const
{
	return downKeys.count(key) == 1;
}

int InputUtility::getMouseScrollDirection() const
{
	if(mouseScrollDelta > 0)
	{
		return 1;
	}
	else if(mouseScrollDelta < 0)
	{
		return -1;
	}
	return 0;
}

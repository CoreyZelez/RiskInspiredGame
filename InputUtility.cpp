#include "InputUtility.h"

void InputUtility::handleInputEvent(const sf::Event &event)
{
	// Handle mouse button events.
	if(event.type == sf::Event::MouseButtonPressed)
	{
		if(std::find(pressedButtons.begin(), pressedButtons.end(), event.mouseButton.button) == pressedButtons.end())
		{
			pressedButtons.push_back(event.mouseButton.button);
		}
	}
	else if(event.type == sf::Event::MouseButtonReleased)
	{
		// Find the position of the value in the vector.
		auto iter = std::find(pressedButtons.begin(), pressedButtons.end(), event.mouseButton.button);

		// Check if the value is found before erasing.
		if(iter != pressedButtons.end()) 
		{
			pressedButtons.erase(iter);
		}
	}

	// Handle keyboard events.
	if(event.type == sf::Event::KeyPressed)
	{
		if(std::find(pressedKeys.begin(), pressedKeys.end(), event.key.code) == pressedKeys.end())
		{
			pressedKeys.push_back(event.key.code);
		}
	}
	else if(event.type == sf::Event::KeyReleased)
	{
		// Find the position of the value in the vector.
		auto iter = std::find(pressedKeys.begin(), pressedKeys.end(), event.key.code);

		// Check if the value is found before erasing.
		if(iter != pressedKeys.end())
		{
			pressedKeys.erase(iter);
		}
	}

	// Handle mouse scroll event.
	if(event.type == sf::Event::MouseWheelScrolled)
	{
		mouseScrollDelta = event.mouseWheelScroll.delta;
	}
}

void InputUtility::update()
{
	// Reset mouseScrollDelta since if no events trigger it remains fixed.
	mouseScrollDelta = 0;
}

bool InputUtility::getButtonPressed(const sf::Mouse::Button &button) const
{
	return std::find(pressedButtons.begin(), pressedButtons.end(), button) != pressedButtons.end();
}

bool InputUtility::getKeyPressed(const sf::Keyboard::Key &key) const
{
	return std::find(pressedKeys.begin(), pressedKeys.end(), key) != pressedKeys.end();
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

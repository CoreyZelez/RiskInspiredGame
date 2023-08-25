#pragma once
#include <set>
#include <SFML/Graphics.hpp>

/*
Singleton class for gaining curated information about input devices.
*/
class InputUtility
{
public:
	// Static method to access the single instance of the class.
	static InputUtility& getInstance()
	{
		static InputUtility instance; // This will be created only once.
		return instance;
	}

	void handleInputEvent(const sf::Event& event);
	void update();

	bool getButtonPressed(const sf::Mouse::Button& button) const;
	bool getKeyPressed(const sf::Keyboard::Key& key) const;
	bool getButtonDown(const sf::Mouse::Button& button) const;
	bool getKeyDown(const sf::Keyboard::Key& key) const;
	int getMouseScrollDirection() const;

private:
	// Private constructor and destructor to prevent external instantiation.
	InputUtility() = default;
	~InputUtility() = default;

	// Private copy constructor and assignment operator to prevent cloning.
	InputUtility(const InputUtility&) = delete;
	InputUtility& operator=(const InputUtility&) = delete;

	// Member variables.
	std::set<sf::Mouse::Button> pressedButtons;
	std::set<sf::Keyboard::Key> pressedKeys;
	std::set<sf::Mouse::Button> downButtons;
	std::set<sf::Keyboard::Key> downKeys;
	int mouseScrollDelta;
};


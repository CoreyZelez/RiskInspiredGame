#pragma once
#include "Command.h"
#include <Memory>
#include <SFML/Graphics.hpp>
#include <assert.h>
#include <vector>

class CommandButton
{
public:
	CommandButton(sf::Vector2f position, sf::Vector2f size, std::vector<std::unique_ptr<Command>> &commands);
	CommandButton(sf::Vector2f position, sf::Vector2f size, std::unique_ptr<Command> &command);

	void draw(sf::RenderWindow& window);

	bool contains(const sf::Vector2f &point) const;

	void handleInput(const sf::RenderWindow &window);

private:
	sf::RectangleShape shape;
	sf::Vector2f position;
	sf::Vector2f size;

	std::vector<std::unique_ptr<Command>> commands;
};


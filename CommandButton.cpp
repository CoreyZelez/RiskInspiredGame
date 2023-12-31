#include "CommandButton.h"
#include "InputUtility.h"

CommandButton::CommandButton(sf::Vector2f position, sf::Vector2f size, std::vector<std::unique_ptr<Command>> &commands)
{
	for(auto &command : commands)
	{
		this->commands.emplace_back(std::move(command));
	}

	shape.setPosition(position);
	shape.setSize(size);
	shape.setFillColor(sf::Color::Green);
}

CommandButton::CommandButton(sf::Vector2f position, sf::Vector2f size, std::unique_ptr<Command> &command)
{
	commands.emplace_back(std::move(command));

	shape.setPosition(position);
	shape.setSize(size);
	shape.setFillColor(sf::Color::Green);
}

bool CommandButton::contains(const sf::Vector2f & point) const
{
	return shape.getGlobalBounds().contains(point);
}

void CommandButton::handleInput(const sf::RenderWindow &window)
{
	InputUtility &inputUtility = InputUtility::getInstance();

	if(inputUtility.getButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		if(contains(sf::Vector2f(mousePos.x, mousePos.y)))
		{
			for(auto &command : commands)
			{
				command.get()->execute();
			}
		}
	}
}

void CommandButton::draw(sf::RenderWindow &window)
{
	window.draw(shape);
}

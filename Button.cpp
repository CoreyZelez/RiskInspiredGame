#include "Button.h"
#include "InputUtility.h"

Button::Button(sf::Vector2f position, sf::Vector2f size, std::unique_ptr<Command> command)
	: position(position), size(size), command(std::move(command)) 
{
	shape.setPosition(position);
	shape.setSize(size);
	shape.setFillColor(sf::Color::Green); 
}

bool Button::contains(const sf::Vector2f & point) const
{
	return shape.getGlobalBounds().contains(point);
}

void Button::handleInput(const sf::RenderWindow &window)
{
	InputUtility &inputUtility = InputUtility::getInstance();

	if(inputUtility.getButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		if(contains(sf::Vector2f(mousePos.x, mousePos.y)))
		{
			assert(command.get() != nullptr);
			command.get()->execute();
		}
	}
}

void Button::draw(sf::RenderWindow &window)
{
	window.draw(shape);
}

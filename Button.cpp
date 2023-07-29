#include "Button.h"

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

void Button::handleEvent(const sf::Event & event)
{
	if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
	{
		sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
		if(contains(mousePos))
		{
			assert(command.get() != nullptr);
			command.get()->execute();
		}
	}
}

void Button::draw(sf::RenderWindow & window)
{
	window.draw(shape);
}

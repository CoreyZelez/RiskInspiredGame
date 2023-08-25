#pragma once
#include <SFML/Graphics.hpp>
#include "Title.h"

class EstateManager;
class Estate;

enum class EstateMakerState
{
	none,
	selectEstate,  // For choosing estate of some specified title.
	editEstate  // For editing a selected estate.
};

class EstateMaker
{
public:
	EstateMaker(EstateManager &estateManager);

	void draw(sf::RenderWindow &window);

	void handleInput(const sf::RenderWindow &window, sf::View &view);

private:
	void changeState(EstateMakerState state);  // Handles state changes, in particular sets colors of estates. 
	void handleInputForView(sf::View &view) const;

	EstateManager &estateManager;
	Estate *estate = nullptr;  // Estate currently being modified.
	Title selectedTitle;  // Title to be selected for modification.
	EstateMakerState state = EstateMakerState::none;
	sf::Clock inputClock;  // Clock tracking time since last time input handled.
};
#pragma once
#include "UIComponent.h"
#include <SFML/Graphics.hpp>

class IHasUI;

class GameUI
{
public:
	void draw(sf::RenderWindow &window) const;

	void setLeftPanel(const IHasUI &hasUI);
	void setRightPanel(const IHasUI &hasUI);
	void setBottomPanel(const IHasUI &hasUI);


	///// Displays interaction panel for specified Estate.
	///void loadEstatePanel(const Estate &estate);

private:
	UIComponent leftPanel;
	UIComponent rightPanel;
	UIComponent bottomPanel;

};


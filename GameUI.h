#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class UIPanel;

class GameUI
{
public:
	void draw(sf::RenderWindow &window) const;

	void setLeftPanel(std::unique_ptr<UIPanel> panel);
	void setRightPanel(std::unique_ptr<UIPanel> panel);
	void setBottomPanel(std::unique_ptr<UIPanel> panel);

private:
	std::unique_ptr<UIPanel> leftPanel;
	std::unique_ptr<UIPanel> rightPanel;
	std::unique_ptr<UIPanel> bottomPanel;

};


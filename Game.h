#pragma once
#include "Map.h"
#include "Player.h"
#include "GameDisplay.h"
#include <SFML/Graphics.hpp>
#include <vector>

enum class GameState
{
	idle,  // Nothing is selected
	militarySelected,
	realmSelectMode
};

class Game
{
public:
	Game(std::string mapName);

	void update(); 

	GameState getState() const;

	void setMapMode(MapMode mapMode);
	MapMode getMapMode() const;

	// Selects military at world position of current HUMAN players turn.
	void selectMilitary(sf::Vector2f position);
	// Selected military attempts to attack territory located at position.
	void moveSelectedMilitary(sf::Vector2f position);

	// Gets player realm which contains a given world position.
	const Realm* getRealm(const sf::Vector2f &position);

	void endHumanPlayerTurn();

	void changeDisplayMilitary();
	bool getDisplayMilitary() const;

	// Creates a view for the game.
	GameDisplay createView() const;

private: 
	void generatePlayers();  // Creates and assigns 1 player per barony.

	Map map; 
	MapMode mapMode = MapMode::realm;
	bool displayMilitary = true;
	GameState state;
	std::vector<std::unique_ptr<Player>> players; 
	std::vector<std::unique_ptr<Player>>::iterator currPlayer;
	bool humanPlayerTurn = false;  // Specifies that current turn is human player. Game waits for input.
	MilitaryForce *selectedMilitary = nullptr;  // Military selected for movement.
	int selectedStrength;  // Strength of selected military to move.
};


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

struct DisplayOptions
{
	bool displayPorts = true;
	bool displayMilitaries = true;
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

	const Map &getMap() const;

	// Gets player realm which contains a given world position.
	const Realm* getRealm(const sf::Vector2f &position);

	// Gets estate of specified title at a given world position.
	const Estate* getEstate(const sf::Vector2f &position, Title title);

	void endHumanPlayerTurn();

	void changeDisplayMilitary();
	DisplayOptions getDisplayOptions() const;

	const MilitaryForce &getSelectedMilitary() const;
	unsigned int &getSelectedStrength();

	// Creates a view for the game.
	GameDisplay createView() const;

private: 
	void generatePlayers();  // Creates and assigns 1 player per barony.

	Map map; 
	MapMode mapMode = MapMode::realm;
	DisplayOptions displayOptions;
	GameState state;
	std::vector<std::unique_ptr<Player>> players; 
	std::vector<std::unique_ptr<Player>>::iterator currPlayer;
	bool humanPlayerTurn = false;  // Specifies that current turn is human player. Game waits for input.
	MilitaryForce *selectedMilitary = nullptr;  // Military selected for movement.
	unsigned int selectedStrength;  // Strength of selected military to move.
};


#pragma once
#include "Map.h"
#include "Player.h"
#include "GameDisplay.h"
#include "NameGenerator.h"
#include "GameplaySettings.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>

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
	Game(GameplaySettings gameplaySettings, std::string mapName);

	void update(); 

	GameState getState() const;

	// Creates a new player adding them to newPlayers vector, and returns the player.
	Player &createPlayer();

	// Selects military at world position of current HUMAN players turn.
	void selectMilitary(sf::Vector2f position);
	// Selected military attempts to attack territory located at position.
	void moveSelectedMilitary(sf::Vector2f position);
	// Sets selectedPlayer as currPlayer. Allows ensuring human, setting to nullptr if not human.
	void selectCurrPlayerRealm(bool humanOnly);
	// Returns realm of current player.
	Realm& getCurrPlayerRealm();
	// Returns current player.
	Player& getCurrPlayer();
	// Returns true if current player is human.
	bool currPlayerIsHuman() const;

	// Sets selectedDiplomacyPlayer to player with realm at specified position.
	void selectDiplomacyPlayer(const sf::Vector2f &position);
	// Deslects selected realm and resets its grid color to default color.
	void deselectDiplomacyPlayer();
	// Returns true if there is a selected player for diplomacy map view.
	bool isDiplomacyView() const;

	// Sets vassal view for the realm at world position.
	void setVassalView(const sf::Vector2f &position);
	// Sets the vassal view for every realm to false.
	void resetVassalViews();

	const GameplaySettings &getGameplaySettings() const;

	// Gets the map the game is played on.
	const Map &getMap() const;
	// Gets player realm which contains a given world position. Can additionally consider whether vassal view
	// is active for the realm.
	const Realm* getRealm(const sf::Vector2f &position, bool considerVassalView = false) const;
	// Gets estate of specified title at a given world position.
	const Estate* getEstate(const sf::Vector2f &position, Title title);

	void setMapMode(MapMode mapMode);
	MapMode getMapMode() const;

	// Specifys that human turn is over.
	void endHumanPlayerTurn();

	void changeDisplayMilitary();
	DisplayOptions getDisplayOptions() const;

	const MilitaryForce &getSelectedMilitary() const;
	unsigned int &getSelectedStrength();

	// Creates a view for the game.
	GameDisplay createView() const;

private: 
	const GameplaySettings gameplaySettings;
	Map map; 
	MapMode mapMode = MapMode::realm;
	DisplayOptions displayOptions;
	GameState state;
	NameGenerator nameGenerator;
	std::vector<std::unique_ptr<Player>> players;
	int currPlayer = 0;  // Index of current player.
	Player *selectedDiplomacyPlayer;
	bool humanPlayerTurn = false;  // Specifies that current turn is human player. Game waits for input.
	MilitaryForce *selectedMilitary = nullptr;  // Military selected for movement.
	unsigned int selectedStrength;  // Strength of selected military to move.

	// Updates grids of player realms in parelel.
	void updateGrids();
	// Creates one player per barony and assignes the players the barony.
	void generatePlayers();  
	// Returns the realm at position in game world.
	Realm *getRealm(const sf::Vector2f &position);
	void setDiplomacyRealmColors();
};


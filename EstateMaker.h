#include "EditorEstate.h"
#include "NameGenerator.h"
#include <SFML/Graphics.hpp>
#include <deque>

enum class EstateMakerState
{
	none,
	selectEstate,  // For choosing estate of some specified title.
	editEstate,  // For editing a selected estate.
	editColors
};

class EstateMaker
{
public:
	EstateMaker();

	void load(std::string mapName, std::deque<EditorTerritory>& territories);

	void save(std::string mapName) const;

	void draw(sf::RenderWindow &window);

	void handleInput(const sf::RenderWindow &window, sf::View &view);

	// Adds and removes baronies as necessary dependant on existent land territories.
	void reconcileBaronies(const std::deque<EditorTerritory>& territories);
	// Adds and removes maridoms as necessary dependant on existent naval territories.
	void reconcileMaridoms(const std::deque<EditorTerritory>& territories);

private:
	std::map<Title, std::list<EditorEstate>, TitleComparer> estates;
	EditorEstate* selectedEstate = nullptr;  // Estate currently being modified.
	Title selectedTitle;  // Title to be selected for modification.

	NameGenerator nameGenerator;

	std::unordered_set<std::string> allocatedEstateNames;
	std::unordered_set<int> allocatedTerritoryIDs;

	EstateMakerState state = EstateMakerState::none;
	sf::Clock inputClock;  // Clock tracking time since last time input handled.

	void loadEstate(std::ifstream& file);
	void loadBarony(std::ifstream& file, std::deque<EditorTerritory>& territories);
	void loadMaridom(std::ifstream& file, std::deque<EditorTerritory>& territories);
	std::string loadName(std::ifstream &file);
	sf::Color loadColor(std::ifstream &file);
	std::vector<std::string> loadSubfiefNames(std::ifstream& file);
	EditorEstate* getFief(std::string name);

	void drawEstates(sf::RenderWindow &window) const;
	void drawEstates(sf::RenderWindow& window, Title title) const;

	EditorEstate* getEstate(sf::Vector2f position, Title title, bool allowParent);
	EditorEstate* getLowerEstate(sf::Vector2f position, Title title, bool allowParent);

	// Makes estates of a given title and optionally below, a specific color. Other estates are made grey.
	void makeColored(Title title, bool setLower);
	void randomiseTitleColor(Title title);
	void setTitleDefaultColor(Title title);
	void setTitleGreyColor(Title title);

	// Creates a new estate of the specified title.
	EditorEstate& createEstate(Title title);

	std::string generateName();

	void changeState(EstateMakerState state);  // Handles state changes, in particular sets colors of estates. 
	void handleInputForView(sf::View &view) const;
};

sf::Color generateRandomEstateColor(Title title);

#pragma once
#include "Title.h"
#include "Grid.h"
#include "HasUI.h"
#include <set>
#include <memory>
#include <SFML/Graphics.hpp>

class Player;
class LandArmy;
class NavalFleet;
class MilitaryManager;

const std::string estateSaveLabel = "# estate";  // Save label for non landed estate.
const std::string baronySaveLabel = "# barony";  // Save label for barony.

class Estate : public HasUI  
{
public:
	Estate(Title title);
	Estate(Title title, const Grid &grid);
	Estate(Title title, const Grid &grid, std::string name);

	virtual ~Estate() = default;
	
	void initName(std::string name);

	virtual void saveToFile(std::ofstream &file) const;

	void draw(sf::RenderWindow &window) const;

	virtual std::unique_ptr<UIEntity> getUI(UIType type) const override;

	virtual bool containsPosition(const sf::Vector2f &position) const;

	// Yields any resources directly associated with estate. This does not include subfief resources.
	virtual void yield(MilitaryManager &militaryManager);
	// Provides bonus yields to subfiefs. Yield dependant on title.
	virtual void provideSubfiefBonusYields();

	void setRuler(Player *ruler, bool updatePlayerGrid = true);

	void addSubfief(Estate *subfief);
	void removeSubfief(Estate *subfief);

	void setParent(Estate *parent);
	bool hasParent() const;

	bool compareRuler(const Player *player) const;  // For debugging.
	const Player *getRuler() const;

	Title getTitle() const;
	std::string getName() const;

	Grid& getGrid();
	const Grid& getGrid() const;

protected:
	void initColor();

	// Military generating subfiefs receive specified bonus yield.
	virtual void receiveBonusYield(const float &bonus);
	// Estate attempts to create military units on map.
	virtual void generateMilitary(MilitaryManager &militaryManager);
	virtual std::string getSaveLabel() const;

	Player* getRuler();

private:
	Player *ruler = nullptr;
	const Title title;
	Estate *parent;
	std::set<Estate*> subfiefs;
	std::string name = "";
	Grid grid;
	bool drawSubfiefs = false;

	// Alocates estate to ruler who's realm owns every lower estate entirely.
	void handleAllocation();
	// Revokes estate from ruler if a threshold of lower estate ownership is not surpassed.
	void handleRevocation();
	// Returns player who's realm contains EVERY lower estate of this estate. Otherwise nullptr.
	Player* getLowerEstatesUpperRealmRuler();
	// Gets notified of ownership change of a lower estate and updates ownership accordingly.
	void handleLowerEstateChange(const Estate &subfief);

	void recursiveGetSubfiefTitleCounts(std::map<Title, int> &subfiefTitleCounts) const;
	std::map<Title, int> getSubfiefTitleCounts() const;

	void saveSubfiefs(std::ofstream &file) const;

};


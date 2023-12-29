#pragma once
#include "Title.h"
#include "Grid.h"
#include "HasUI.h"
#include <set>
#include <memory>
#include <unordered_set>
#include <SFML/Graphics.hpp>

class Player;
class LandArmy;
class NavalFleet;
class MilitaryManager;

const std::string estateSaveLabel = "# estate";  // Save label for non landed estate.
const std::string baronySaveLabel = "# barony";  // Save label for barony.
const std::string maridomSaveLabel = "# maridom";  // Save label for maridom.

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

	// Changes the ownership of the estate by adding the estate to ruler's realm and setting the new owner
	// as the player which ruler grants the estate to (possibly the ruler themself).
	void setOwnership(Player *ruler);

	// Adds a subfief to this estate.
	void addSubfief(Estate *subfief);
	// Removes a subfief from this estate.
	void removeSubfief(Estate *subfief);

	// Returns unordered set of all lower estates contained within this estate.
	std::unordered_set<const Estate*> getLowerEstates() const;

	// Changes the parent estate.
	void setParent(Estate *parent);
	// Returns true if parent estate is nullptr.
	bool hasParent() const;
	// Returns the parent estate.
	const Estate* getParent() const;

	// Compares player with ruler for debugging puposes.
	bool compareRuler(const Player *player) const;  
	// Returns the direct ruler of this estate.
	const Player* getRuler() const;
	// Returns true if estate has a ruler.
	bool hasRuler() const;

	// Returns title of estate.
	Title getTitle() const;

	// Returns estate name.
	std::string getName() const;

	// Returns the associated graphical grid of this estate.
	Grid& getGrid();
	// Returns the associated graphical grid of this estate.
	const Grid& getGrid() const;

	// Returns the number of landed indirect subfiefs of this estate apart of specified players realm, not necessarily directly controlled.
	int calculateLandedSubfiefOwnershipCount(const Player &player) const;

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
	// Returns player who's realm contains EVERY lower estate of this estate. Otherwise nullptr.
	Player* getLowerEstatesUpperRealmRuler();
	// Gets notified of ownership change of a lower estate and updates ownership accordingly.
	void handleLowerEstateChange(const Estate &subfief);

	// Determines the counts of all lower estates of estate.
	std::map<Title, int> getLowerEstateTitleCounts() const;
	// Recursively determines the counts of all lower estates of estate.
	void recursiveGetLowerEstateTitleCounts(std::map<Title, int> &subfiefTitleCounts) const;

	void saveSubfiefs(std::ofstream &file) const;

};




#pragma once
#include "Title.h"
#include "CompositeGrid.h"
#include "HasUI.h"
#include "RealmGrid.h"
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
	Estate(Title title, sf::Color color);
	Estate(Title title, const Grid& grid, sf::Color color);

	virtual ~Estate() = default;

	// THIS SHOULD BE DONE IN CONSTRUCTOR!!!!
	void initName(std::string name);
	// THIS SHOULD BE DONE IN CONSTRUCTOR!!!!

	void draw(sf::RenderWindow &window) const;

	void update();

	virtual std::unique_ptr<UIEntity> createUI(UIType type) const override;

	virtual bool containsPosition(const sf::Vector2f &position) const;

	// Yields any resources directly associated with estate. This does not include subfief resources.
	virtual void yield();
	// Provides bonus yields to subfiefs. Yield dependant on title.
	virtual void provideSubfiefBonusYields();

	// Sets the owner to nullptr. Assumes no subfiefs and that ruler's game is over.
	void clearOwnership();
	// Changes the ownership of the estate by adding the estate to ruler's realm and setting the new owner
	// as the player which ruler grants the estate to (possibly the ruler themself).
	void setOwnership(Player *ruler, bool recurseOnParents = true);
	// Revokes ownership if owner's upper liege's realm does not completely control lower estates and sets
	// ownership to rightful player if applicable.
	void ammendOwnership();

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
	// Returns the direct ruler of this estate.
	Player* getRuler();

	// Returns true if estate has a ruler.
	bool hasRuler() const;

	// Returns title of estate.
	Title getTitle() const;

	// Returns estate name.
	std::string getName() const;

	// Changes default color of grid.
	void setDefaultColor(sf::Color color);
	
	// Sets color to default color for given title.
	void setGridColorDefault();
	// Sets color to generic grey color.
	void setGridColorGrey();

	// Returns the associated graphical grid of this estate.
	CompositeGrid& getGrid();
	// Returns the associated graphical grid of this estate.
	const CompositeGrid& getGrid() const;

	// Returns the number of barony subfiefs of this estate apart of specified players realm. 
	// Optionally specify directly controlled by player.
	int calculateBaronySubfiefOwnershipCount(const Player &player, bool requireDirectControl) const;
	// Returns number of subfiefs of specified title. Optionally include indirect subfiefs in count.
	int calculateNumberOfSubfiefs(Title title, bool allowIndirect) const;
	
protected:
	// Military generating subfiefs receive specified bonus yield.
	virtual void receiveBonusYield(const float &bonus);
	// Estate attempts to create military units on map.
	virtual void generateMilitary(MilitaryManager &militaryManager);
	virtual std::string getSaveLabel() const;

private:
	Player *ruler = nullptr;
	const Title title;
	Estate *parent;
	std::set<Estate*> subfiefs;
	std::string name = "";
	CompositeGrid grid;
	sf::Color defaultInteriorColor;  // Default color of estate.
	sf::Color defaultSubBorderColor;  // Default color of estate.
	bool drawSubfiefs = false;
	const double darkeningFactor = 0.72;

	// Alocates estate to ruler who's realm owns every lower estate entirely.
	void handleAllocation();
	// Gets notified of ownership change of a lower estate and updates ownership accordingly.
	void handleLowerEstateChange(const Estate &subfief);
	// Returns player who's realm contains EVERY lower estate of this estate. Otherwise nullptr.
	Player* getLowerEstatesUpperRealmRuler();

	// Determines the counts of all lower estates of estate.
	std::map<Title, int> getLowerEstateTitleCounts() const;
	// Recursively determines the counts of all lower estates of estate.
	void recursiveGetLowerEstateTitleCounts(std::map<Title, int> &subfiefTitleCounts) const;
};

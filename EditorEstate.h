#pragma once
#include "Title.h"
#include "EditorGrid.h"
#include "EditorTerritory.h"
#include "HasUI.h"
#include <set>
#include <memory>
#include <unordered_set>
#include <SFML/Graphics.hpp>

class Player;
class LandArmy;
class NavalFleet;
class MilitaryManager;

class EditorEstate 
{
public:
	EditorEstate(Title title, sf::Color color);
	EditorEstate(Title title, const EditorTerritory* territory, sf::Color color);

	virtual ~EditorEstate() = default;

	void initName(std::string name);

	virtual void saveToFile(std::ofstream& file) const;

	void draw(sf::RenderWindow& window) const;

	virtual bool containsPosition(const sf::Vector2f& position) const;

	// Adds a subfief to this estate.
	void addSubfief(EditorEstate* subfief);
	// Removes a subfief from this estate.
	void removeSubfief(EditorEstate* subfief);

	// Returns unordered set of all lower estates contained within this estate.
	std::unordered_set<const EditorEstate*> getLowerEstates() const;

	// Returns true if parent estate is nullptr.
	bool hasParent() const;
	// Returns the parent estate.
	const EditorEstate* getParent() const;
	void setParent(EditorEstate* parent);

	Title getTitle() const;
	std::string getName() const;

	void setDefaultColor(sf::Color color);

	void setGridColorDefault();
	void setGridColorGrey();

	const EditorTerritory* getTerritory() const;

	EditorGrid& getGrid();
	const EditorGrid& getGrid() const;

protected:
	void initColor();

	virtual std::string getSaveLabel() const;

private:
	std::string name = "";
	const Title title;
	EditorGrid grid;
	sf::Color defaultColor;  // Default color of estate.

	EditorEstate* parent = nullptr;
	std::set<EditorEstate*> subfiefs;

	const EditorTerritory* territory = nullptr;  // Territory associated with estate if landed.

	bool drawSubfiefs = false;

	void saveSubfiefs(std::ofstream& file) const;
};

std::string generateRandomEstateName(Title title);



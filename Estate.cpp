#include "Estate.h"
#include "Player.h"
#include "NameGenerator.h"
#include <assert.h>
#include <iostream>
#include <fstream>

Estate::Estate(Title title)
	: title(title)
{
	initColor();
}

Estate::Estate(Title title, const Grid &grid)
	: title(title)
{
	this->grid.addGrid(grid);
	initColor();
}

Estate::Estate(Title title, const Grid &grid, std::string name)
	: title(title), name(name)
{
	this->grid.addGrid(grid);
	initColor();
}

void Estate::initName(std::string name)
{
	assert(this->name.length() == 0);
	this->name = name;
}

void Estate::saveToFile(std::ofstream &file) const
{
	assert(file.is_open());

	// Append data to the file.
	file << getSaveLabel() << std::endl;
	file << "# title" << std::endl;
	file << static_cast<int>(title) << std::endl;
	file << "# name" << std::endl;
	file << name << std::endl;
	file << "# subfiefs" << std::endl;
	saveSubfiefs(file);
}

void Estate::saveSubfiefs(std::ofstream &file) const
{
	for(auto subfief : subfiefs)
	{
		file << subfief->name << std::endl;
	}
}

void Estate::provideSubfiefBonusYields()
{
	if(title == Title::baron)
	{
		assert(subfiefs.size() == 0);
		return;
	}

	float bonus = 0;
	switch(title)
	{
	case Title::count:
		// bonus = 0.3f;
		bonus = 10.0f;
		break;
	case Title::duke:
		bonus = 0.2f;
		break;
	case Title::king:
		// bonus = 0.1f;
		bonus = 1000.0f;
		break;
	case Title::emperor:
		bonus = 0.1f;
		break;
	}

	// Grants bonus yield to all subfiefs that are military generating and belong to this ruler's realm.
	this->receiveBonusYield(bonus);
}

void Estate::yield(MilitaryManager &militaryManager)
{
}

/* Function empty as (currently) non landed estates do not generate military directly. */
void Estate::generateMilitary(MilitaryManager &militaryManager)
{
}

std::string Estate::getSaveLabel() const
{
	return estateSaveLabel;
}

void Estate::addSubfief(Estate *subfief)
{
	assert(subfief->title < title);
	assert(subfief->parent == nullptr);
	subfiefs.insert(subfief);
	subfief->parent = this;  
	//
	//
	/// IN FUTURE THIS MAY BE INCORRECT TO DO. SHOULD ONLY DO WHEN ADDING BARONIES PROBABLY.
	//
	//
	grid.addGrid(subfief->grid);
}

void Estate::removeSubfief(Estate *subfief)
{
	if(subfief == nullptr || subfief->parent != this)
	{
		return;
	}

	for(auto iter = subfiefs.cbegin(); iter != subfiefs.cend(); ++iter)
	{
		if(*iter == subfief)
		{
			subfiefs.erase(iter);

			//
	        //
	        /// IN FUTURE THIS MAY BE INCORRECT TO DO. SHOULD ONLY DO WHEN ADDING BARONIES PROBABLY.
	        //
	        //
			grid.removeGrid(subfief->grid);
			//
			//
			assert(subfief->parent == this);
			subfief->parent = nullptr;
			return;
		}
	}
}

void Estate::setParent(Estate *parent)
{
	this->parent = parent;
}

bool Estate::hasParent() const
{
	return parent != nullptr;
}

bool Estate::compareRuler(const Player *player) const
{
	return (this->ruler == player);
}

Title Estate::getTitle() const
{
	return title;
}

std::string Estate::getName() const
{
	return name;
}

Grid& Estate::getGrid()
{
	return grid;
}

const Grid& Estate::getGrid() const
{
	return grid;
}

void Estate::initColor()
{
	const sf::Color baronyColor(210, 19, 0);
	const sf::Color countyColor(50, 200, 180);
	const sf::Color dukedomColor(30, 220, 0);
	const sf::Color kingdomColor(230, 110, 0);
	const sf::Color empireColor(240, 0, 140);

	switch(title)
	{
	case Title::baron:
		this->grid.setColor(baronyColor);
		break;
	case Title::count:
		this->grid.setColor(countyColor);
		break;
	case Title::duke:
		this->grid.setColor(dukedomColor);
		break;
	case Title::king:
		this->grid.setColor(kingdomColor);
		break;
	case Title::emperor:
		this->grid.setColor(empireColor);
		break;
	}
}

void Estate::receiveBonusYield(const float &bonus)
{
	for(auto &subfief : subfiefs)
	{
		// Only provides bonus yield to subfief if owner is this ruler or a (direct or indirect) vassal of this ruler.
		if(subfief->ruler == ruler || subfief->ruler->getRealm().getRelationshipManager().isVassal(*ruler, false))
		{
			subfief->receiveBonusYield(bonus);
		}
	}
}

bool Estate::containsPosition(const sf::Vector2f &position) const
{
	for(auto& subfief : subfiefs)
	{
		if(subfief->containsPosition(position))
		{
			return true;
		}
	}
	return false;
}

Player* Estate::getRuler()
{
	return ruler;
}

void Estate::setRuler(Player *ruler, bool updatePlayerGrid)
{
	// Case ruler is not changed.
	if(this->ruler == ruler)
	{
		return;
	}

	if(this->ruler != nullptr)
	{
		this->ruler->getRealm().getEstateManager().removeFief(this, updatePlayerGrid);
	}
	this->ruler = ruler;
	if(this->ruler != nullptr)
	{
		this->ruler->getRealm().getEstateManager().addFief(this, updatePlayerGrid);
	}

	// Tell upper estate(s) to check whether uppermost liege of ruler (possibly ruler themselves) should gain control of it.
	if(parent != nullptr)
	{
		/// THIS FUNCTION MAY BE GETTING CALLED MULTIPLE TIMES UNNECESSARILY. SEE HANDLE ALLOCATION.
		parent->handleLowerEstateChange(*this);
	}
}

void Estate::handleAllocation()
{
	Player *ruler = getLowerEstatesUpperRealmRuler();
	if(ruler != nullptr)
	{
		assert(ruler->getRealm().hasLiege() == false);
		// Ruler's realm contains every lowest level estate of this estate thus gets granted this estate.
		// WARNING: RELIES ON FACT THAT LOWER ESTATE TITLE IS A LANDED ESTATE AND UPPER ESTATES ARE NOT.
		const bool updatePlayerGrid = false;  // Don't update player grid since estate not landed.
		////
		setRuler(ruler, updatePlayerGrid);
	}
}

void Estate::handleRevocation()
{
	assert(ruler != nullptr);

	bool revoked = false;

	// Revokes estate if any subfief of estate is not apart of ruler's realm.
	for(const Estate *subfief : subfiefs)
	{

		if(subfief->ruler == nullptr || &subfief->ruler->getRealm().getRelationshipManager().getUpperRealmRuler())
		{
			// WARNING: RELIES ON FACT THAT LOWER ESTATE TITLE IS A LANDED ESTATE AND UPPER ESTATES ARE NOT.
			const bool updatePlayerGrid = false;  // Don't update player grid since estate not landed.
			////
			setRuler(nullptr, updatePlayerGrid);
			revoked = true;
			break;
		}
	}
}

Player* Estate::getLowerEstatesUpperRealmRuler() 
{
	if(subfiefs.size() == 0)
	{
		assert(ruler != nullptr);  // All lowest level estates should have a ruler (in current game design).
		assert(&ruler->getRealm().getRelationshipManager().getUpperRealmRuler() != nullptr);
		return &ruler->getRealm().getRelationshipManager().getUpperRealmRuler();
	}

	// Check that subfiefs have a ruler. If not, impossible for all lower estates to belong to same realm.
	for(Estate *subfief : subfiefs)
	{
		if(subfief->ruler == nullptr)
		{
			return nullptr;
		}
	}

	// Check that lower estates share the same upper realm ruler.
	Player *upperRealmRuler = nullptr;
	for(Estate *subfief : subfiefs)
	{
		Player *ruler = subfief->getLowerEstatesUpperRealmRuler();
		assert(ruler != nullptr);
		if(ruler != nullptr && upperRealmRuler == nullptr)
		{
			upperRealmRuler = ruler;
		}
		else if(ruler != upperRealmRuler)
		{
			return nullptr;
		}
	}	
	return upperRealmRuler;
}

void Estate::handleLowerEstateChange(const Estate &subfief)
{
	// assert(subfiefs.count(&lowerEstate) == 1);
	assert(subfiefs.size() > 0);

	// Estate unowned.
	if(ruler == nullptr)
	{
		// Allocate this estate to the upper realm ruler of lowerEstate if every lower estate
		// of this estate belongs to the upper realm ruler. The upper realm ruler is the
		// ruler who has no liege and lower estate belongs to their realm.
		handleAllocation();
	}
	// A lower fief was revoked.
	else if(subfief.ruler == nullptr)
	{
		handleRevocation();
	}
	// This estate already apart of same upper realm.
	else if(ruler->getRealm().getRelationshipManager().sameUpperRealm(*subfief.ruler))
	{
		// Do nothing.
	}
	// Estate apart of enemy upper realm. Potentially requires revocation.
	else 
	{
		handleRevocation();
	}

	///// Handle ownership changes of upper estates.
	///if(parent != nullptr)
	///{
	///	parent->handleLowerEstateChange(*this);
	///}
}


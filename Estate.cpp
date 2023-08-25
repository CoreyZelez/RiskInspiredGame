#include "Estate.h"
#include "Player.h"
#include "NameGenerator.h"
#include <assert.h>
#include <iostream>
#include <fstream>

Estate::Estate(Title title)
	: title(title)
{
}

Estate::Estate(Title title, const Grid &grid)
	: title(title)
{
	this->grid.addGrid(grid);
}

Estate::Estate(Title title, const Grid &grid, std::string name)
	: title(title), name(name)
{
	this->grid.addGrid(grid);
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

void Estate::initRuler(Player &ruler)
{
	assert(this->ruler == nullptr);
	this->ruler = &ruler;
	this->ruler->getRealm().addFief(this);
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
		bonus = 0.3f;
		break;
	case Title::duke:
		bonus = 0.2f;
		break;
	case Title::king:
		bonus = 0.1f;
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
	subfiefs.push_back(subfief);
	subfief->parent = this;  // Holds shared pointer to this object.
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
			grid.removeGrid(subfief->grid);
			assert(subfief->parent == this);
			subfief->parent = nullptr;
			return;
		}
	}
}

void Estate::setParent(const Estate *parent)
{
	this->parent = parent;
}

bool Estate::hasParent() const
{
	return parent != nullptr;
}

bool Estate::compareRuler(const Player *player) const
{
	return this->ruler == player;
}

Title Estate::getTitle() const
{
	return title;
}

std::string Estate::getName() const
{
	return name;
}

Grid & Estate::getGrid()
{
	return grid;
}

const Grid & Estate::getGrid() const
{
	return grid;
}

void Estate::receiveBonusYield(const float &bonus)
{
	for(auto &subfief : subfiefs)
	{
		// Only provides bonus yield to subfief if owner is of same realm as this ruler.
		if(subfief->ruler == ruler || subfief->ruler->getRealm().isVassal(*ruler))
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

void Estate::setRuler(Player *ruler)
{
	// Case ruler is not changed.
	if(this->ruler == ruler)
	{
		return;
	}

	this->ruler->getRealm().removeFief(this);
	this->ruler = ruler;
	this->ruler->getRealm().addFief(this);
}





#include "Estate.h"
#include "Player.h"
#include <assert.h>
#include <iostream>

Estate::Estate(Title title)
	: title(title)
{
}

Estate::Estate(Title title, const Grid &grid)
	: title(title)
{
	this->grid.addGrid(grid);
}

void Estate::initRuler(Player &ruler)
{
	assert(this->ruler == nullptr);
	this->ruler = &ruler;
	this->ruler->addFief(this);
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

void Estate::yield(PlayerMilitaryManager &militaryManager)
{
}

/* Function empty as (currently) non landed estates do not generate military directly. */
void Estate::generateMilitary(PlayerMilitaryManager &militaryManager)
{
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

bool Estate::gridContainsPosition(const sf::Vector2f &position) const
{
	return grid.containsPosition(position);
}

void Estate::drawGrid(sf::RenderWindow & window) const
{
	return grid.draw(window);
}

void Estate::setGridColor(sf::Color color)
{
	grid.setColor(color);
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

void Estate::receiveBonusYield(const float &bonus)
{
	for(auto &subfief : subfiefs)
	{
		// Only provides bonus yield to subfief if owner is of same realm as this ruler.
		if(subfief->ruler == ruler || subfief->ruler->getRelationshipManager().isVassal(*ruler))
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
	this->ruler->removeFief(this);
	this->ruler = ruler;
	this->ruler->addFief(this);
}





#include "Estate.h"
#include "Player.h"
#include <assert.h>
#include <iostream>

Estate::Estate(Title title, const Player *ruler)
	: title(title), ruler(ruler)
{
}

Estate::Estate(Title title)
	: title(title)
{
}

Estate::Estate(Title title, const Player *ruler, const Grid &grid)
	: title(title), ruler(ruler)
{
	this->grid.addGrid(grid);
}

Estate::Estate(Title title, const Grid &grid)
	: title(title)
{
	this->grid.addGrid(grid);
}

void Estate::addSubFief(std::shared_ptr<Estate> subFief)
{
	assert(subFief->title < title);
	assert(subFief->parent == nullptr);
	subFiefs.push_back(subFief);
	subFief.get()->parent = shared_from_this();  // Holds shared pointer to this object.
	grid.addGrid(subFief.get()->grid);
}

void Estate::removeSubFief(std::shared_ptr<Estate>& subFief)
{
	if(subFief == nullptr || subFief.get()->parent.get() != this)
	{
		return;
	}

	for(auto iter = subFiefs.cbegin(); iter != subFiefs.cend(); ++iter)
	{
		if(iter->get() == subFief.get())
		{
			subFiefs.erase(iter);
			grid.removeGrid(subFief.get()->grid);
			assert(subFief.get()->parent == this);
			subFief.get()->parent = nullptr;
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

void Estate::setParent(const std::shared_ptr<const Estate> &parent)
{
	this->parent = parent;
}

bool Estate::hasParent() const
{
	return parent != nullptr;
}

Title Estate::getTitle() const
{
	return title;
}

bool Estate::containsPosition(const sf::Vector2f &position) const
{
	for(auto& subFief : subFiefs)
	{
		if(subFief.get()->containsPosition(position))
		{
			return true;
		}
	}
	return false;
}

const Player* Estate::getRuler() const
{
	return ruler;
}

void Estate::setRuler(const Player *ruler)
{
	this->ruler = ruler;
}





#include "EditorEstate.h"
#include "Player.h"
#include "RichText.h"
#include "NameGenerator.h"
#include "InformationPanel.h"
#include "FontManager.h"
#include "Barony.h"
#include "Realm.h"
#include <assert.h>
#include <iostream>
#include <fstream>

EditorEstate::EditorEstate(Title title, sf::Color color)
	: EditorEstate(title, nullptr, color)
{
}

EditorEstate::EditorEstate(Title title, const EditorTerritory* territory, sf::Color color)
	: title(title), territory(territory), defaultColor(color)
{
	if (territory != nullptr)
	{
		this->grid.addGrid(territory->getGrid());
	}

	if (title == Title::maridom)
	{
		this->grid.setBorderMode(BorderMode::feintBorders);
		this->grid.setAllPositionsDark();
	}

	this->grid.setColor(color);
}

void EditorEstate::initName(std::string name)
{
	assert(this->name.length() == 0);
	this->name = name;
}

void EditorEstate::saveToFile(std::ofstream& file) const
{
	assert(file.is_open());

	// Append data to the file.
	file << getSaveLabel() << std::endl;
	file << "# title" << std::endl;
	file << static_cast<int>(title) << std::endl;
	file << "# name" << std::endl;
	file << name << std::endl;
	file << "# color" << std::endl;
	file << (int)defaultColor.r << " " << (int)defaultColor.g << " " << (int)defaultColor.b << std::endl;
	file << "# subfiefs" << std::endl;
	saveSubfiefs(file);

	if (territory != nullptr)
	{
		file << "# territory id" << std::endl;
		file << territory->getID() << std::endl;
	}
}

void EditorEstate::draw(sf::RenderWindow& window) const
{
	if (drawSubfiefs)
	{

	}
	else
	{
		grid.draw(window);
	}
}

void EditorEstate::saveSubfiefs(std::ofstream& file) const
{
	for (auto subfief : subfiefs)
	{
		file << subfief->name << std::endl;
	}
}

std::string EditorEstate::getSaveLabel() const
{
	if (title == Title::barony)
	{
		return baronySaveLabel;
	}
	else if (title == Title::maridom)
	{
		return maridomSaveLabel;
	}

	return estateSaveLabel;
}

void EditorEstate::addSubfief(EditorEstate* subfief)
{
	assert(subfief->title < title);
	assert(subfief->parent == nullptr);
	subfiefs.insert(subfief);
	subfief->parent = this;
	grid.addGrid(subfief->grid);
}

void EditorEstate::removeSubfief(EditorEstate* subfief)
{
	if (subfief == nullptr || subfief->parent != this)
	{
		return;
	}

	for (auto iter = subfiefs.cbegin(); iter != subfiefs.cend(); ++iter)
	{
		if (*iter == subfief)
		{
			subfiefs.erase(iter);
			grid.removeGrid(subfief->grid);
			assert(subfief->parent == this);
			subfief->parent = nullptr;
			return;
		}
	}
}

std::unordered_set<const EditorEstate*> EditorEstate::getLowerEstates() const
{
	std::unordered_set<const EditorEstate*> lowerEstates;

	for (const EditorEstate* subfief : subfiefs)
	{
		lowerEstates.insert(subfief);
		std::unordered_set<const EditorEstate*> subfiefLowerEstates = subfief->getLowerEstates();
		lowerEstates.insert(subfiefLowerEstates.begin(), subfiefLowerEstates.end());
	}

	return lowerEstates;

}

void EditorEstate::setParent(EditorEstate* parent)
{
	this->parent = parent;
}

bool EditorEstate::hasParent() const
{
	return parent != nullptr;
}

const EditorEstate* EditorEstate::getParent() const
{
	return parent;
}

Title EditorEstate::getTitle() const
{
	return title;
}

std::string EditorEstate::getName() const
{
	return name;
}

void EditorEstate::setDefaultColor(sf::Color color)
{
	this->defaultColor = color;
	this->grid.setColor(color);
}

void EditorEstate::setGridColorDefault()
{
	this->grid.setColor(defaultColor);
}

void EditorEstate::setGridColorGrey()
{
	sf::Color greyColor(90, 90, 90);
	this->grid.setColor(greyColor);
}

const EditorTerritory* EditorEstate::getTerritory() const
{
	return territory;
}

EditorGrid& EditorEstate::getGrid()
{
	return grid;
}

const EditorGrid& EditorEstate::getGrid() const
{
	return grid;
}

void EditorEstate::initColor()
{
	const int randComponent1 = rand() % 61;
	const int randComponent2 = rand() % 41;
	const int randComponent3 = rand() % 41;
	const int randComponent4 = rand() % 21;

	const sf::Color maridomColor(120, 120, 120);
	const sf::Color baronyColor(190 + randComponent1, randComponent2, 0);
	const sf::Color countyColor(50, 170 + randComponent1, 160 + randComponent2);
	const sf::Color dukedomColor(30, 190 + randComponent1, 0);
	const sf::Color kingdomColor(210 + randComponent2, 210 + randComponent3, randComponent4);
	const sf::Color empireColor(190 + randComponent1, randComponent2, 100 + randComponent3);

	switch (title)
	{
	case Title::maridom:
		this->grid.setColor(maridomColor);
		break;
	case Title::barony:
		this->grid.setColor(baronyColor);
		break;
	case Title::county:
		this->grid.setColor(countyColor);
		break;
	case Title::duchy:
		this->grid.setColor(dukedomColor);
		break;
	case Title::kingdom:
		this->grid.setColor(kingdomColor);
		break;
	case Title::empire:
		this->grid.setColor(empireColor);
		break;
	}
}

bool EditorEstate::containsPosition(const sf::Vector2f& position) const
{
	for (auto& subfief : subfiefs)
	{
		if (subfief->containsPosition(position))
		{
			return true;
		}
	}
	return false;
}

std::string generateRandomEstateName(Title title)
{
	return std::string();
}


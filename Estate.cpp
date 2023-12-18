#include "Estate.h"
#include "Player.h"
#include "RichText.h"
#include "NameGenerator.h"
#include "InformationPanel.h"
#include "FontManager.h"
#include <assert.h>
#include <iostream>
#include <fstream>

Estate::Estate(Title title)
	: title(title)
{
	if(title == Title::admiral)
	{
		this->grid.setBorderMode(BorderMode::feintBorders);
	}
	initColor();
}

Estate::Estate(Title title, const Grid &grid)
	: title(title)
{
	this->grid.addGrid(grid);
	if(title == Title::admiral)
	{
		this->grid.setBorderMode(BorderMode::feintBorders);
	}
	initColor();
}

Estate::Estate(Title title, const Grid &grid, std::string name)
	: title(title), name(name)
{
	this->grid.addGrid(grid);
	if(title == Title::admiral)
	{
		this->grid.setBorderMode(BorderMode::feintBorders);
	}
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

void Estate::draw(sf::RenderWindow &window) const
{
	if(drawSubfiefs)
	{
		
	}
	else
	{
		grid.draw(window);
	}
}

std::unique_ptr<UIEntity> Estate::getUI(UIType type) const
{
	FontManager &fontManager = FontManager::getInstance();
	if(type == UIType::information)
	{
		const sf::Font &font = *fontManager.getFont("UIFont1");

		// Name text.
		sfe::RichText nameText(font);
		nameText << sf::Text::Regular << sf::Color::White << "Name: "
			<< sf::Color::Yellow << name;

		// Title text map.
		std::map<Title, std::string> titleStringMap = 
		{
		{ Title::baron, "Barony" },
		{ Title::count, "County" },
		{ Title::duke, "Duchy" },
		{ Title::king, "Kingdom" },
		{ Title::emperor, "Empire" } 
		};

		// Title text.
		sfe::RichText titleText(font);
		titleText << sf::Text::Regular << sf::Color::White << "type: "
			<< sf::Color::Yellow << titleStringMap[title];

		std::map<Title, int> titleCounts = getSubfiefTitleCounts();

		// Barony count text.
		sfe::RichText baronyCntText(font);
		baronyCntText << sf::Text::Regular << sf::Color::White << "Number of Barony subfiefs: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::baron]);

		// County count text.
		sfe::RichText countyCntText(font);
		countyCntText << sf::Text::Regular << sf::Color::White << "Number of County subfiefs: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::count]);

		// Duchy count text.
		sfe::RichText duchyCntText(font);
		duchyCntText << sf::Text::Regular << sf::Color::White << "Number of Duchy subfiefs: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::duke]);

		// Kingdom count text.
		sfe::RichText kingdomCntText(font);
		kingdomCntText << sf::Text::Regular << sf::Color::White << "Number of Kingdom subfiefs: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::king]);


		std::vector<sfe::RichText> texts = { nameText, titleText };
		if(title == Title::emperor)
		{
			texts.push_back(kingdomCntText);
		}
		if(title >= Title::king)
		{
			texts.push_back(duchyCntText);
		}
		if(title >= Title::duke)
		{
			texts.push_back(countyCntText);
		}
		if(title >= Title::count)
		{
			texts.push_back(baronyCntText);
		}

		return std::make_unique<InformationPanel>(texts);
	}

	return nullptr;
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
	if(title == Title::baron || title == Title::admiral)
	{
		assert(subfiefs.size() == 0);
		return;
	}

	float bonus = 0;
	switch(title)
	{
	case Title::count:
		bonus = 0.5f;
		break;
	case Title::duke:
		bonus = 0.5f;
		break;
	case Title::king:
		bonus = 0.3f;
		break;
	case Title::emperor:
		bonus = 0.2f;
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

const Player* Estate::getRuler() const
{
	return ruler;
}

bool Estate::hasRuler() const
{
	return ruler != nullptr;
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

void Estate::setRuler(Player *ruler)
{
	// Case ruler is not changed.
	if(this->ruler == ruler)
	{
		return;
	}

	// Remove fief from previous ruler.
	if(this->ruler != nullptr)
	{
		this->ruler->getRealm().getEstateManager().removeFief(this);
	}

	// Change ruler of estate.
	this->ruler = ruler;

	// Add fief to new ruler.
	if(this->ruler != nullptr)
	{
		this->ruler->getRealm().getEstateManager().addFief(this);
	}

	// Tell upper estate(s) to check whether uppermost liege of ruler (possibly ruler themselves) should gain control of it.
	if(parent != nullptr)
	{
		/// THIS FUNCTION MAY BE GETTING CALLED MULTIPLE TIMES UNNECESSARILY. SEE HANDLE ALLOCATION.
		parent->handleLowerEstateChange(*this);
	}
}

void Estate::recursiveGetSubfiefTitleCounts(std::map<Title, int>& subfiefTitleCounts) const
{
	for(const auto &subfief : subfiefs)
	{
		++subfiefTitleCounts[subfief->getTitle()];
		subfief->recursiveGetSubfiefTitleCounts(subfiefTitleCounts);
	}
}

std::map<Title, int> Estate::getSubfiefTitleCounts() const
{
	std::map<Title, int> counts;
	recursiveGetSubfiefTitleCounts(counts);
	return counts;
}

void Estate::handleAllocation()
{
	Player *ruler = getLowerEstatesUpperRealmRuler();
	if(ruler != nullptr)
	{
		assert(ruler->getRealm().getRelationshipManager().hasLiege() == false);
		// Ruler's realm contains every lowest level estate of this estate thus gets granted this estate.
		// WARNING: RELIES ON FACT THAT LOWER ESTATE TITLE IS A LANDED ESTATE AND UPPER ESTATES ARE NOT.
		const bool updatePlayerGrid = false;  // Don't update player grid since estate not landed.
		////
		setRuler(ruler);
	}
}

void Estate::handleRevocation()
{
	assert(ruler != nullptr);

	// Revokes estate if any subfief of estate is not apart of ruler's realm.
	for(const Estate *subfief : subfiefs)
	{
		if(subfief->ruler == nullptr || &subfief->ruler->getRealm().getRelationshipManager().getUpperRealmRuler())
		{
			setRuler(nullptr);
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


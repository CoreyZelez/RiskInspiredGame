#include "Estate.h"
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

Estate::Estate(Title title)
	: title(title)
{
	if(title == Title::maridom)
	{
		this->grid.setBorderMode(BorderMode::feintBorders);
	}
	initColor();
}

Estate::Estate(Title title, const Grid &grid)
	: title(title)
{
	this->grid.addGrid(grid);
	if(title == Title::maridom)
	{
		this->grid.setBorderMode(BorderMode::feintBorders);
	}
	initColor();
}

Estate::Estate(Title title, const Grid &grid, std::string name)
	: title(title), name(name)
{
	this->grid.addGrid(grid);
	if(title == Title::maridom)
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
		{ Title::barony, "Barony" },
		{ Title::county, "County" },
		{ Title::duchy, "Duchy" },
		{ Title::kingdom, "Kingdom" },
		{ Title::empire, "Empire" } 
		};

		// Title text.
		sfe::RichText titleText(font);
		titleText << sf::Text::Regular << sf::Color::White << "Type: "
			<< sf::Color::Yellow << titleStringMap[title];

		std::map<Title, int> titleCounts = getLowerEstateTitleCounts();

		// Barony count text.
		sfe::RichText baronyCntText(font);
		baronyCntText << sf::Text::Regular << sf::Color::White << "Number of Barony subfiefs: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::barony]);

		// County count text.
		sfe::RichText countyCntText(font);
		countyCntText << sf::Text::Regular << sf::Color::White << "Number of County subfiefs: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::county]);

		// Duchy count text.
		sfe::RichText duchyCntText(font);
		duchyCntText << sf::Text::Regular << sf::Color::White << "Number of Duchy subfiefs: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::duchy]);

		// Kingdom count text.
		sfe::RichText kingdomCntText(font);
		kingdomCntText << sf::Text::Regular << sf::Color::White << "Number of Kingdom subfiefs: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::kingdom]);


		std::vector<sfe::RichText> texts = { nameText, titleText };
		if(title == Title::empire)
		{
			texts.push_back(kingdomCntText);
		}
		if(title >= Title::kingdom)
		{
			texts.push_back(duchyCntText);
		}
		if(title >= Title::duchy)
		{
			texts.push_back(countyCntText);
		}
		if(title >= Title::county)
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
	if(title == Title::barony || title == Title::maridom)
	{
		assert(subfiefs.size() == 0);
		return;
	}

	float bonus = 0;
	switch(title)
	{
	case Title::county:
		bonus = 0.5f;
		break;
	case Title::duchy:
		bonus = 0.4f;
		break;
	case Title::kingdom:
		bonus = 0.3f;
		break;
	case Title::empire:
		bonus = 0.2f;
		break;
	}

	// Grants bonus yield to all subfiefs that are military generating and belong to this ruler's realm.
	this->receiveBonusYield(bonus);
}

void Estate::yield()
{
}

/* 
 *Function empty as non landed estates do not generate military directly. 
 */
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

std::unordered_set<const Estate*> Estate::getLowerEstates() const
{
	std::unordered_set<const Estate*> lowerEstates;

	for(const Estate* subfief : subfiefs)
	{
		lowerEstates.insert(subfief);
		std::unordered_set<const Estate*> subfiefLowerEstates = subfief->getLowerEstates();
		lowerEstates.insert(subfiefLowerEstates.begin(), subfiefLowerEstates.end());
	}

	return lowerEstates;

}

void Estate::setParent(Estate *parent)
{
	this->parent = parent;
}

bool Estate::hasParent() const
{
	return parent != nullptr;
}

const Estate* Estate::getParent() const
{
	return parent;
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

int Estate::calculateBaronySubfiefOwnershipCount(const Player &player, bool requireDirectControl) const
{
	int count = 0;
	for(const Estate *estate : subfiefs)
	{
		if(estate->title == Title::barony)
		{
			assert(dynamic_cast<const Barony*>(estate) != nullptr);
			// Since current estate is landed we can directly check for ownership by either ruler or a vassal.
			if(!requireDirectControl && (estate->ruler == &player ||
				(estate->ruler != nullptr && estate->getRuler()->isVassal(player, false))))
			{
				++count;
			}
			// When require direct control, only check if ruler is the player.
			else if(requireDirectControl && estate->ruler == &player)
			{
				++count;
			}
		}
		else
		{
			// Add the count of ownership of landed subfiefs of current estates subfiefs, noting the current estate itself is unlanded.
			count += estate->calculateBaronySubfiefOwnershipCount(player, requireDirectControl);
		}
	}
	return count;
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

void Estate::receiveBonusYield(const float &bonus)
{
	for(auto &subfief : subfiefs)
	{
		// Only provides bonus yield to subfief if owner is this ruler or a (direct or indirect) vassal of this ruler.
		if(subfief->ruler == ruler || subfief->ruler->isVassal(*ruler, false))
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

void Estate::setOwnership(Player *ruler)
{
	// Case rulers are friendly. If any ruler is nullptr they are considered non-friendly.
	if(sameUpperRealm(ruler, this->ruler))
	{
		return;
	}

	// Remove estate from previous ruler.
	if(this->ruler != nullptr)
	{
		this->ruler->getRealm().removeEstate(*this);
		this->ruler = nullptr;
	}

	// Add estate to new ruler's realm and set the ruler of the realm.
	if(ruler != nullptr)
	{
		// newRuler is the player which ruler confers this estate to, possibly themselves.
		Player *newRuler = &ruler->getRealm().addEstate(*this);
		assert(newRuler != nullptr);
		this->ruler = newRuler;
	}
	else
	{
		this->ruler = nullptr;
	}

	// Recurse on parent estates to handle possible ownership changes due to change in lower estate, namely this estate..
	if(parent != nullptr)
	{
		/// THIS FUNCTION MAY BE GETTING CALLED MULTIPLE TIMES UNNECESSARILY. SEE HANDLE ALLOCATION.
		parent->handleLowerEstateChange(*this);
	}
}

void Estate::recursiveGetLowerEstateTitleCounts(std::map<Title, int>& subfiefTitleCounts) const
{
	for(const auto &subfief : subfiefs)
	{
		++subfiefTitleCounts[subfief->getTitle()];
		subfief->recursiveGetLowerEstateTitleCounts(subfiefTitleCounts);
	}
}

std::map<Title, int> Estate::getLowerEstateTitleCounts() const
{
	std::map<Title, int> counts;
	recursiveGetLowerEstateTitleCounts(counts);
	return counts;
}

void Estate::handleAllocation()
{
	Player *ruler = getLowerEstatesUpperRealmRuler();
	if(ruler != nullptr)
	{
		assert(ruler->hasLiege() == false);
		// Ruler's realm contains every lowest level estate of this estate thus gets granted this estate.
		setOwnership(ruler);
	}
}

Player* Estate::getLowerEstatesUpperRealmRuler() 
{
	if(subfiefs.size() == 0)
	{
		assert(ruler != nullptr);  // All lowest level estates should have a ruler (in current game design).
		assert(&ruler->getUpperLiege() != nullptr);
		return &ruler->getUpperLiege();
	}

	// Check that all subfiefs have a ruler. If not, impossible for all lower estates to belong to same realm.
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
	// This estate already apart of same realm.
	else if(sameUpperRealm(ruler, subfief.ruler))
	{
		// Do nothing.
	}
	// A lower fief was revoked or estate apart of enemy upper realm. Potentially requires revocation.
	else 
	{
		setOwnership(nullptr);
	}
}


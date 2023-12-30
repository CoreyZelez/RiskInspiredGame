#include "Realm.h"
#include "Player.h"
#include "Estate.h"
#include "Utility.h"
#include "MilitaryManager.h"
#include "FontManager.h"
#include "RichText.h"
#include "InformationPanel.h"
#include "Game.h"
#include <assert.h>
#include <iostream>

Realm::Realm(Game &game, Player &ruler)
	: ruler(ruler), rulerEstateManager(ruler.getMilitaryManager())
	, vassalManager(game, ruler), estateAllocator(ruler, rulerEstateManager, vassalManager)
{
}

void Realm::draw(sf::RenderWindow &window) const
{
	realmGrid.draw(window);
	// Draw vassal realms on over entire realm grid if specified.
	if(drawVassalRealms)
	{
		vassalManager.drawVassalRealms(window);
	}
}

std::unique_ptr<UIEntity> Realm::getUI(UIType type) const
{
	FontManager &fontManager = FontManager::getInstance();
	if(type == UIType::information)
	{
		const sf::Font &font = *fontManager.getFont("UIFont1");

		// Name text.
		sfe::RichText nameText(font);
		nameText << sf::Text::Regular << sf::Color::White << "Name: "
			<< sf::Color::Yellow << "tempName";

		std::map<Title, int> titleCounts = getTitleCounts();

		// Barony count text.
		sfe::RichText baronyCntText(font);
		baronyCntText << sf::Text::Regular << sf::Color::White << "Number of Baronies: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::barony]);
		// County count text.
		sfe::RichText countyCntText(font);
		countyCntText << sf::Text::Regular << sf::Color::White << "Number of Counties: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::county]);
		// Duchy count text.
		sfe::RichText duchyCntText(font);
		duchyCntText << sf::Text::Regular << sf::Color::White << "Number of Duchies: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::duchy]);
		// Kingdom count text.
		sfe::RichText kingdomCntText(font);
		kingdomCntText << sf::Text::Regular << sf::Color::White << "Number of Kingdoms: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::kingdom]);
		// Empire count text.
		sfe::RichText empireCntText(font);
		empireCntText << sf::Text::Regular << sf::Color::White << "Number of Empires: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::empire]);

		std::vector<sfe::RichText> texts = { nameText };

		if(titleCounts[Title::barony] > 0)
		{
			texts.push_back(baronyCntText);
		}
		if(titleCounts[Title::county] > 0)
		{
			texts.push_back(countyCntText);
		}
		if(titleCounts[Title::duchy] > 0)
		{
			texts.push_back(duchyCntText);
		}
		if(titleCounts[Title::kingdom] > 0)
		{
			texts.push_back(kingdomCntText);
		}
		if(titleCounts[Title::empire] > 0)
		{
			texts.push_back(empireCntText);
		}

		return std::make_unique<InformationPanel>(texts);
	}

	return nullptr;
}

void Realm::handleMilitaryYields()
{
	rulerEstateManager.handleMilitaryYields();
}

bool Realm::isVassal(const Player &player, bool direct) const
{
	if(liege == &player)
	{
		return true;
	}

	// Don't check vassals if determining whether this->ruler is a direct vassal.
	if(direct)
	{
		return false;
	}

	// // Check if this->ruler is vassal of players vassals.
	// for(const Player *vassal : player.getRealm().vassalManager.getVassals())
	// {
	// 	if(isVassal(*vassal, false))
	// 	{
	// 		return true;
	// 	}
	// }

	// Check if realm's ruler is an indirect vassal of player.
	// Iterates through liege hierarchy of ruler to check for occurrence of player.
	const Player *currLiege = ruler.getRealm().getLiege();
	while(currLiege != nullptr)
	{
		if(currLiege == &player)
		{
			return true;
		}
		currLiege = currLiege->getRealm().getLiege();
	}

	return false;
}

bool Realm::sameUpperRealm(const Player &player) const
{
	const Player &upperLiege1 = player.getRealm().getUpperRealmRuler();
	const Player &upperLiege2 = ruler.getRealm().getUpperRealmRuler();
	return &upperLiege1 == &upperLiege2;
}

Player& Realm::getUpperRealmRuler()
{
	Player *upperLiege = &ruler;
	while(upperLiege->getRealm().liege != nullptr)
	{
		upperLiege = upperLiege->getRealm().liege;
	}
	return *upperLiege;
}

const Player& Realm::getUpperRealmRuler() const
{
	const Player *upperLiege = &ruler;
	while(upperLiege->getRealm().liege != nullptr)
	{
		upperLiege = upperLiege->getRealm().liege;
	}
	return *upperLiege;
}

Player& Realm::addEstate(Estate &estate)
{
	const LandedEstate* landedEstate = dynamic_cast<const LandedEstate*>(&estate);
	// Update realm grid if estate is landed.
	if(landedEstate != nullptr)
	{
		realmGrid.addGrid(landedEstate->getGrid());
	}

	// Confer the estate to vassal or ruler and return the recipient.
	return estateAllocator.allocate(estate);
}

void Realm::removeEstate(Estate &estate)
{
	if(rulerEstateManager.containsEstate(estate))
	{
		assert(estate.compareRuler(&ruler));
		rulerEstateManager.removeEstate(estate);
	}
	else
	{
		assert(!estate.compareRuler(&ruler));
		assert(vassalManager.containsEstate(estate));
		vassalManager.removeEstate(estate);
	}

	// Recurse upon lieges to ensure estate and associated territory if landed is removed
	// from all upper liege realms.
	if(liege != nullptr)
	{
		liege->getRealm().removeEstate(estate);
	}

	LandedEstate *landedEstate = dynamic_cast<LandedEstate*>(&estate);
	// Update realm grid if estate is landed.
	if(landedEstate != nullptr)
	{
		realmGrid.removeGrid(landedEstate->getGrid());
	}
}

std::unordered_set<Territory*> Realm::getTerritories()
{
	std::unordered_set<Territory*> realmTerritories;
	const std::unordered_set<Territory*> &rulerTerritories = rulerEstateManager.getTerritories(); 
	const std::unordered_set<Territory*> &vassalTerritories = vassalManager.getTerritories();
	realmTerritories = rulerTerritories;
	realmTerritories.insert(vassalTerritories.begin(), vassalTerritories.end());
	return realmTerritories;
}

std::unordered_set<const Estate*> Realm::getEstates() const
{
	std::unordered_set<const Estate*> realmEstates;
	const std::unordered_set<const Estate*> rulerEstates = rulerEstateManager.getEstates();
	const std::unordered_set<const Estate*> vassalEstates = vassalManager.getEstates();
	realmEstates = rulerEstates;
	realmEstates.insert(vassalEstates.begin(), vassalEstates.end());
	return realmEstates;
}

void Realm::updateGrid()
{
	realmGrid.updateGrid();
}

bool Realm::containsPosition(const sf::Vector2f &position) const
{
	return realmGrid.containsPosition(position);
}

void Realm::setGridColor(const sf::Color & color)
{
	realmGrid.setGridColor(color);
}

void Realm::setGridColorDefault()
{
	realmGrid.setGridColorDefault();
}

bool Realm::hasLiege() const
{
	return liege != nullptr;
}

const Player * Realm::getLiege() const
{
	return liege;
}

void Realm::setLiege(Player *player)
{
	liege = player;
}

Title Realm::getHighestRulerTitle() const
{
	return rulerEstateManager.getHighestTitle();
}

std::map<Title, int> Realm::getTitleCounts() const
{
	std::map<Title, int> realmTitleCounts;
	std::map<Title, int> rulerTitleCounts = rulerEstateManager.getTitleCounts();
	std::map<Title, int> vassalsTitleCounts = vassalManager.getTitleCounts();
	realmTitleCounts.insert(rulerTitleCounts.begin(), rulerTitleCounts.end());
	realmTitleCounts.insert(vassalsTitleCounts.begin(), vassalsTitleCounts.end());
	return realmTitleCounts;

}






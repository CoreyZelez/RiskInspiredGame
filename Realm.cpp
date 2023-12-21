#include "Realm.h"
#include "Player.h"
#include "Estate.h"
#include "Utility.h"
#include "MilitaryManager.h"
#include "FontManager.h"
#include "RichText.h"
#include "InformationPanel.h"
#include <assert.h>
#include <iostream>

Realm::Realm(Player &ruler)
	: ruler(ruler), rulerEstateManager(realmGrid, ruler.getMilitaryManager()), vassalManager(ruler, realmGrid)
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
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::baron]);
		// County count text.
		sfe::RichText countyCntText(font);
		countyCntText << sf::Text::Regular << sf::Color::White << "Number of Counties: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::count]);
		// Duchy count text.
		sfe::RichText duchyCntText(font);
		duchyCntText << sf::Text::Regular << sf::Color::White << "Number of Duchies: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::duke]);
		// Kingdom count text.
		sfe::RichText kingdomCntText(font);
		kingdomCntText << sf::Text::Regular << sf::Color::White << "Number of Kingdoms: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::king]);
		// Empire count text.
		sfe::RichText empireCntText(font);
		empireCntText << sf::Text::Regular << sf::Color::White << "Number of Empires: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::emperor]);

		std::vector<sfe::RichText> texts = { nameText };

		if(titleCounts[Title::baron] > 0)
		{
			texts.push_back(baronyCntText);
		}
		if(titleCounts[Title::count] > 0)
		{
			texts.push_back(countyCntText);
		}
		if(titleCounts[Title::duke] > 0)
		{
			texts.push_back(duchyCntText);
		}
		if(titleCounts[Title::king] > 0)
		{
			texts.push_back(kingdomCntText);
		}
		if(titleCounts[Title::emperor] > 0)
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
	// VASSAL MILITARY YIELDS HANDLED BY VASSALS
	// SHOULD GIVE MILITARY TO LIEGES REINFORCEMENTS 
	// AND YIELD TO THEIR RESERVES. SEE TXT.
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

	// Check if this->ruler is vassal of players vassals.
	for(const Player *vassal : player.getRealm().vassalManager.getVassals())
	{
		if(isVassal(*vassal, false))
		{
			return true;
		}
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
		upperLiege = liege;
	}
	return *upperLiege;
}

const Player& Realm::getUpperRealmRuler() const
{
	const Player *upperLiege = &ruler;
	while(upperLiege->getRealm().liege != nullptr)
	{
		upperLiege = liege;
	}
	return *upperLiege;
}

void Realm::addEstate(Estate *estate)
{
	// TEMPORARY FUNCTION IMPLEMENTATION. IN FUTURE MUST CONSIDER VASSALS!!!
	rulerEstateManager.addEstate(estate);
}

void Realm::removeEstate(Estate *estate)
{
	// TEMPORARY FUNCTION IMPLEMENTATION. IN FUTURE MUST CONSIDER VASSALS!!!
	rulerEstateManager.removeEstate(estate);
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

void Realm::setLiege(Player *player)
{
	liege = player;
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






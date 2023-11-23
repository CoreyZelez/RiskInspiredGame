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

Realm::Realm(Player &player)
	: player(player), estateManager(player.getMilitaryManager()), relationshipManager(player)
{
}

void Realm::draw(sf::RenderWindow &window) const
{
	if(drawVassalRealms)
	{
		relationshipManager.drawVassalRealms(window);
	}
	else
	{
		estateManager.draw(window);
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

		std::map<Title, int> titleCounts = estateManager.getTitleCounts();

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

RealmEstateManager& Realm::getEstateManager()
{
	return estateManager;
}

const RealmEstateManager& Realm::getEstateManager() const
{
	return estateManager;
}

RealmRelationshipManager& Realm::getRelationshipManager()
{
	return relationshipManager;
}

const RealmRelationshipManager & Realm::getRelationshipManager() const
{
	return relationshipManager;
}





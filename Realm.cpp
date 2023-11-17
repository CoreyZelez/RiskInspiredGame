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

std::unique_ptr<UIPanel> Realm::getUI(UIType type) const
{
	FontManager &fontManager = FontManager::getInstance();
	if(type == UIType::information)
	{
		const sf::Font &font = *fontManager.getFont("UIFont1");

		// Name text.
		sfe::RichText nameText(font);
		nameText << sf::Text::Regular << sf::Color::White << "Name: "
			<< sf::Color::Yellow << "tempName";

		// Estate count text.
		sfe::RichText estateCntText(font);
		estateCntText << sf::Text::Regular << sf::Color::White << "Number of estates: "
			<< sf::Color::Yellow << std::to_string(estateManager.getRealmSize());

		// instead of listing number of estates, list number of baronies, counties, kingdoms etc.
		// only list when value greater than 0.
		// estateManager.getEstateCount(title).

		std::vector<sfe::RichText> texts = { nameText, estateCntText };
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





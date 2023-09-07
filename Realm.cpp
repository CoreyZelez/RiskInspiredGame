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
	: player(player), militaryManager(player.getMilitaryManager())
{
	grid.setColor(createRandomRealmColor());
}

void Realm::draw(sf::RenderWindow &window) const
{
	if(drawVassalRealms)
	{
		for(Player *vassal : vassals)
		{
			vassal->getRealm().draw(window);
		}
	}
	else
	{
		grid.draw(window);
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
			<< sf::Color::Yellow << std::to_string(fiefs.size());


		std::vector<sfe::RichText> texts = { nameText, estateCntText };
		return std::make_unique<InformationPanel>(texts);
	}

	return nullptr;
}

void Realm::handleFiefYields()
{
	// Provide bonus yields to fiefs contained in subfiefs under this player's control.
	for(auto &fief : fiefs)
	{
		fief->provideSubfiefBonusYields();
	}

	// Yield resources (currently just military units).
	for(auto &fief : fiefs)
	{
		fief->yield(militaryManager);
	}
}

void Realm::addFief(Estate *fief, bool updateGrid)
{
	fiefs.emplace_back(fief);
	assert(fief->compareRuler(&this->player));
	if(updateGrid)
	{
		grid.addGrid(fief->getGrid());
	}
}

void Realm::removeFief(const Estate *fief, bool updateGrid)
{
	for(auto iter = fiefs.begin(); iter != fiefs.end(); ++iter)
	{
		if(*iter == fief)
		{
			fiefs.erase(iter);
			if(updateGrid)
			{
				grid.removeGrid(fief->getGrid());
			}
			return;
		}
	}
	assert(false);  // Functions should only be called when the estate owner is this player.
}

void Realm::addVassal(Player &vassal)
{
	assert(!vassal.getRealm().hasLiege());
	vassal.getRealm().liege = &(this->player);
	vassals.emplace_back(&vassal);
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
	for(const Player *vassal : player.getRealm().vassals)
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
	const Player &upperLiege2 = this->player.getRealm().getUpperRealmRuler();
	return &upperLiege1 == &upperLiege2;
}

Player& Realm::getUpperRealmRuler()
{
	Player *upperLiege = &player;
	while(upperLiege->getRealm().liege != nullptr)
	{
		upperLiege = liege;
	}
	return *upperLiege;
}

const Player& Realm::getUpperRealmRuler() const
{
	const Player *upperLiege = &player;
	while(upperLiege->getRealm().liege != nullptr)
	{
		upperLiege = liege;
	}
	return *upperLiege;
}

bool Realm::hasLiege() const
{
	return liege != nullptr;
}

bool Realm::containsPosition(const sf::Vector2f &position) const
{
	return grid.containsPosition(position);
}




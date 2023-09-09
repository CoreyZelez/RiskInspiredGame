#include "RealmRelationshipManager.h"
#include "Player.h"

RealmRelationshipManager::RealmRelationshipManager(Player & player)
	: player(player)
{
}

void RealmRelationshipManager::drawVassalRealms(sf::RenderWindow & window) const
{
	for(const Player* vassal : vassals)
	{
		vassal->getRealm().draw(window);
	}
}

void RealmRelationshipManager::addVassal(Player &vassal)
{
	assert(!vassal.getRealm().getRelationshipManager().hasLiege());
	vassal.getRealm().getRelationshipManager().liege = &(this->player);
	vassals.emplace_back(&vassal);
}

bool RealmRelationshipManager::isVassal(const Player &player, bool direct) const
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
	for(const Player *vassal : player.getRealm().getRelationshipManager().vassals)
	{
		if(isVassal(*vassal, false))
		{
			return true;
		}
	}

	return false;
}

bool RealmRelationshipManager::sameUpperRealm(const Player &player) const
{
	const Player &upperLiege1 = player.getRealm().getRelationshipManager().getUpperRealmRuler();
	const Player &upperLiege2 = this->player.getRealm().getRelationshipManager().getUpperRealmRuler();
	return &upperLiege1 == &upperLiege2;
}

Player& RealmRelationshipManager::getUpperRealmRuler()
{
	Player *upperLiege = &player;
	while(upperLiege->getRealm().getRelationshipManager().liege != nullptr)
	{
		upperLiege = liege;
	}
	return *upperLiege;
}

const Player& RealmRelationshipManager::getUpperRealmRuler() const
{
	const Player *upperLiege = &player;
	while(upperLiege->getRealm().getRelationshipManager().liege != nullptr)
	{
		upperLiege = liege;
	}
	return *upperLiege;
}

bool RealmRelationshipManager::hasLiege() const
{
	return liege != nullptr;
}
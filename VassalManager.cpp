#include "VassalManager.h"
#include "Player.h"
#include "Estate.h"
#include "RealmGrid.h"

VassalManager::VassalManager(Player &ruler, RealmGrid &realmGrid)
	: ruler(ruler), realmGrid(realmGrid)
{
}

void VassalManager::drawVassalRealms(sf::RenderWindow & window) const
{
	for(const Player* vassal : vassals)
	{
		vassal->getRealm().draw(window);
	}
}

void VassalManager::addVassal(Player &player)
{
	// Player should not have a liege.
	assert(!vassal.getRealm().getRelationshipManager().hasLiege());
	player.getRealm().setLiege(&ruler);
	vassals.emplace_back(&player);

	// MUST ADD VASSAL ESTATES TO ESTATE VECTOR!!!
}

const std::vector<Player*>& VassalManager::getVassals() const
{
	return vassals;
}

const std::unordered_set<Territory*>& VassalManager::getTerritories()
{
	return territories;
}

std::map<Title, int> VassalManager::getTitleCounts() const
{
	std::map<Title, int> counts;
	for(const auto &estate : estates)
	{
		++counts[estate->getTitle()];
	}
	return counts;
}


#include "PlayerAIContext.h"
#include "Player.h"
#include "Game.h"
#include "Territory.h"
#include <set>

PlayerAIContext::PlayerAIContext(Player &player, Game &game)
	: player(player), game(game)
{
}

std::vector<Territory*> PlayerAIContext::getBorderTerritories()
{
	std::set<Territory*> &realmTerritories = player.getRealm().getEstateManager().getTerritories();
	std::vector<Territory*> borderTerritories = {};

	for(Territory* territory : realmTerritories)
	{
		// Add territory to border territories if all it's adjacencies belong to the realm's territories.
		const std::set<const Territory*> &adjacencies = territory->getAdjacencies();
		for(const auto &adjacency : adjacencies)
		{
			// Checks whether adjacent territory is apart of realms territories.
			if(realmTerritories.count(const_cast<Territory*>(adjacency)) == 0)
			{
				// Territory is border territory since an adjacent territory is apart of enemy player realm.
				borderTerritories.push_back(territory);
				break;
			}
		}
	}

	return borderTerritories;
}

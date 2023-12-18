#include "SimplePlayerAI.h"
#include "Player.h"
#include "Territory.h"
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include <iostream>
#include <algorithm>

SimplePlayerAI::SimplePlayerAI(Game &game, Player &player)
	: PlayerAIComponent(game, player), context(player, game)
{
}

void SimplePlayerAI::handleTurn()
{
	Player &player = getPlayer();
	Game &game = getGame();

	// Execute attacks.
	std::vector<Territory*> borderTerritories = context.getBorderTerritories();
	executeArmyAttacks(borderTerritories);
	executeFleetAttacks(borderTerritories);

	// Redetermine border territories as now territories potentially gained after attacks.
	borderTerritories = context.getBorderTerritories();

	// Execute naval fleets movements, calculating strategic values each time.
	for(int i = 0; i < 3; ++i)
	{
		std::map<Territory*, int> navalFleetStrategicValues;
		for(Territory* territory : borderTerritories)
		{
			assert(calculateStrategicValue(*territory) >= 0);
			navalFleetStrategicValues[territory] = calculateFleetStrategicValue(*territory);
		}
		executeFleetMoveOrders(navalFleetStrategicValues);
	}

	// Strategic values relevant to land armies.
	std::map<Territory*, int> landArmyStrategicValues;
	for(Territory* territory : borderTerritories)
	{
		assert(calculateStrategicValue(*territory) >= 0);
		landArmyStrategicValues[territory] = calculateArmyStrategicValue(*territory);
	}
	executeArmyMoveOrders(landArmyStrategicValues);
}

int SimplePlayerAI::calculateArmyStrategicValue(const Territory &territory)
{
	Player &player = getPlayer();
	Game &game = getGame();

	int strategicValue = 0;

	// Calculate strategic value from threat of attack of territory.
	std::map<const Player*, int> weightedThreats = context.getArmyWeightedThreats(territory);
	int maxThreat = calculateMaxThreat(weightedThreats);  
	int totalThreat = calculateTotalThreat(weightedThreats);  
	strategicValue += maxThreat + totalThreat;

	//
	//
	//
	// IN FUTURE, ADD STRATEGIC VALUE IF ENEMIES POSSESS AN ADJACENT NAVAL TERRITORY AS TO PREPARE
	// FOR NAVAL ATTACKS IN ADVANCE.

	// Prioritise having land troops on land territories.
	if(territory.getType() == TerritoryType::naval)
	{
		const float navalStategicValueAdjustmentFactor = 0.1;
		strategicValue *= navalStategicValueAdjustmentFactor;

		// No reason to station land army on naval territory with no adjacent enemy land territories.
		if(territory.getDistanceMap().noEnemyAdjacent(TerritoryType::land))
		{
			strategicValue = 0;
		}

		// Dont station army troops if naval territory not well protected enough.
		const int minFleetStrength = 4;
		if(territory.getOccupancyHandler()->getFleet() == nullptr ||
			territory.getOccupancyHandler()->getFleet()->getTotalStrength() < minFleetStrength)
		{
			strategicValue = 0;
		}
	}

	//
	//
	// IN FUTURE CHECK IF NAVAL TERRITORY IS "PURELY NAVAL" IE HAS NAVAL NEIGHBOUR TERRITORIES ONLY
	// IF SO THEN STRATEGIC VALUE SHOULD EQUAL 0.
	// ALSO IN FUTURE CONSIDER HAVING LAND ARMIES BE MORE INCLINED TO ATTACK IF ON OCEAN SO THEY
	// DO NOT SIT IN A VULNERABLE POSITION FOR TOO LONG.
	//
	// ALSO SHOULD CALCULATE ARMY STRATEGIC VALUE OF NAVAL TERRITORIES BASED UPON HOW WEAK THE ENEMY
	// ARMY ADJACENT TO THE NAVAL TERRITORY IS (ENEMY ARMY IS ON LAND) RATHER THAN HOW STRONG.
	// THIS LEADS TO MORE EFFECTIVE NAVAL INVASIONS.

	/////////////////////
	// CURRENTLY DOES NOTHING
	// Calculate strategic desire to attack enemy neighbouring territories.
	const std::set<Territory*> adjacencies = territory.getDistanceMap().getAdjacencies();
	std::set<const Territory*> enemyTerritories;
	// Determine enemy territories from adjacencies.
	for(std::set<Territory*>::iterator iter = adjacencies.begin();
		iter != adjacencies.end(); ++iter)
	{
		if((*iter)->getEstateOwner() != &player)
		{
			enemyTerritories.insert(*iter);
		}
	}
	////////////////////////

	assert(strategicValue >= 0);

	return strategicValue;
}

int SimplePlayerAI::calculateFleetStrategicValue(const Territory &territory)
{
	Player &player = getPlayer();
	Game &game = getGame();

	int strategicValue = 0;

	// Calculate strategic value from threat of attack of territory.
	std::map<const Player*, int> weightedThreats = context.getFleetWeightedThreats(territory);
	int maxThreat = calculateMaxThreat(weightedThreats);
	int totalThreat = calculateTotalThreat(weightedThreats);
	strategicValue += maxThreat + totalThreat;

	// Calculate strategic desire to attack enemy neighbouring territories.
	const std::set<Territory*> adjacencies = territory.getDistanceMap().getAdjacencies();
	std::set<const Territory*> enemyTerritories;
	// Determine enemy territories from adjacencies.
	for(std::set<Territory*>::iterator iter = adjacencies.begin();
		iter != adjacencies.end(); ++iter)
	{
		if((*iter)->getEstateOwner() != &player)
		{
			enemyTerritories.insert(*iter);
		}
	}

	assert(strategicValue >= 0);

	return strategicValue;
}

void SimplePlayerAI::executeArmyAttacks(const std::vector<Territory*> &borderTerritories)
{
	Player &player = getPlayer();

	// Iterate over border territories and execute attacks by armies stationed at these territories.
	for(const Territory *territory : borderTerritories)
	{
		assert(territory->getEstateOwner() == &player);

		LandArmy *army = player.getMilitaryManager().getArmy(territory);
		if(army != nullptr)
		{
			executeArmyAttack(*army);
		}
	}
}

void SimplePlayerAI::executeArmyAttack(LandArmy &army)
{
	const int attackStamina = 3;
	int availableStrength = army.getStrength(attackStamina);
	Territory &territory = army.getTerritory();
	const std::set<Territory*> enemyTerritories = context.getEnemyAdjacencies(territory);
	for(std::set<Territory*>::iterator iter = enemyTerritories.begin();
		iter != enemyTerritories.end(); ++iter)
	{
		// Handle case when adjacent territory is a LandTerritory.
		LandTerritory* landTerritory = dynamic_cast<LandTerritory*>(*iter);
		if(landTerritory != nullptr)
		{
			const LandArmy *enemyArmy = landTerritory->getOccupancyHandler()->getArmy();

			if(enemyArmy == nullptr)
			{
				int attackStrength = 0.3 * availableStrength;

				if(attackStrength <= 1)
				{
					attackStrength = availableStrength;
				}
				army.move(*landTerritory, attackStrength);
			}
			else if(enemyArmy->getTotalStrength() < 0.1 * availableStrength)
			{
				int attackStrength = 0.4 * availableStrength;
				if(attackStrength == 0)
				{
					attackStrength = 1;
				}
				army.move(*landTerritory, attackStrength);
			}
			else if(enemyArmy->getTotalStrength() < 0.6 * availableStrength)
			{
				army.move(*landTerritory, availableStrength);
			}
		}

		// Update strength.
		availableStrength = army.getStrength(attackStamina);
		// Return since army has no strength left for attacking with.
		if(availableStrength == 0)
		{
			return;
		}
	}
}


void SimplePlayerAI::executeArmyMoveOrders(const std::map<Territory*, int> &strategicValues)
{
	Player &player = getPlayer();
	std::map<Territory*, int> remainingStrategicValues = strategicValues;

	// Find sum of strategic values.
	int totalStrategicValue = 0; 
	for(const auto& pair : strategicValues)
	{
		assert(pair.second >= 0);
		totalStrategicValue += pair.second;
	}

	/////////////////
	// NOT VALID FOR NAVAL MOVEMENT!!!
	/////
	const int landMoveCost = 3;
	/////////

	// Total strength of player's land military that is available for movement.
	const int totalAvailableArmyStrength = player.getMilitaryManager().getTotalArmyStrength(landMoveCost);
	assert(totalAvailableArmyStrength >= 0);
	if(totalAvailableArmyStrength == 0)
	{
		return;
	}

	// Strategic value that is allocated to each unit of army strength.
	float strategicValuePerUnit = (float)totalStrategicValue / (float)totalAvailableArmyStrength;
	strategicValuePerUnit; // testing.
	assert(strategicValuePerUnit >= 0);

	std::vector<std::unique_ptr<LandArmy>> &armies = player.getMilitaryManager().getArmies();

	// Keeps track of what amount of strength of a given army is free to move from
	// its current territory. 
	std::map<LandArmy*, int> freeToMove;
	for(auto &army : armies)
	{
		freeToMove[army.get()] = army.get()->getStrength(landMoveCost);
	}

	// Armies are added through movement so only iterate up to pre movement number of armies. 
	const int numArmies = armies.size(); 
	assert(numArmies > 0);

	// Distances of each friendly army to borders of own territory with movement inside own territory.
	const int maxDist = 13;
	std::unordered_map<const Territory*, std::unordered_map<int, std::vector<LandArmy*>>> armyBorderDistances = context.getArmyBorderDistances(maxDist);

	// Partially allocates armies prioritising closest first. Closer armies provide greater allocation.
	for(int distance = 0; distance <= maxDist; ++distance)
	{
		for(auto& pair : remainingStrategicValues)
		{
			Territory& territory = *pair.first;
			int &strategicValue = pair.second;

			assert(strategicValue >= 0);

			if(strategicValue == 0)
			{
				continue;
			}

			// Move armies of specified distance towards the territory.
			std::vector<LandArmy*> &currDistArmies = armyBorderDistances[&territory][distance];
			for(int i = 0; i < currDistArmies.size(); ++i)
			{

				LandArmy &army = *currDistArmies[i];

				const Territory &armyTerritory = army.getTerritory();
				// assert(distance = bfsfriendly(army, territory))
				assert(remainingStrategicValues.count(&territory) == 1);
				assert(strategicValue > 0);
				assert(freeToMove[&army] <= army.getStrength(landMoveCost));

				if(freeToMove[&army] == 0)  
				{
					continue;
				}

				// Move army to friendly territory closest to territory.
				// In future prioritise border territories!!!
				const float moveRatio = 0.7;
				int moveStrength = freeToMove[&army] * moveRatio;
				// Ensure move strength not 0 if any amount of army is free to move.
				if(moveStrength == 0 && freeToMove[&army] <= 2)
				{
					moveStrength = freeToMove[&army];
				}
				const int maximumAllocation = strategicValue / strategicValuePerUnit;
				// Move strength is capped by maximum allocation amount derived from territory priority.
				moveStrength = std::min(moveStrength, maximumAllocation);
				if(moveStrength != 0)
				{
					army.moveClosest(territory, moveStrength, maxDist);
					freeToMove[&army] -= moveStrength;
					const int usedStrategicValue = 1 + moveStrength * strategicValuePerUnit;
					strategicValue -= usedStrategicValue;
					if(strategicValue <= 0)
					{
						strategicValue = 0;
						break;
					}
					assert(freeToMove[&army] <= army.getStrength(landMoveCost));
				}
			}
		}
	}
}

void SimplePlayerAI::executeFleetAttacks(const std::vector<Territory*>& borderTerritories)
{
	Player &player = getPlayer();

	// Iterate over border territories and execute attacks by armies stationed at these territories.
	for(const Territory *territory : borderTerritories)
	{
		assert(territory->getEstateOwner() == &player);

		NavalFleet *fleet = player.getMilitaryManager().getFleet(territory);
		if(fleet != nullptr)
		{
			executeFleetAttack(*fleet);
		}
	}
}

void SimplePlayerAI::executeFleetAttack(NavalFleet &fleet)
{
	const int attackStamina = 3;
	int availableStrength = fleet.getStrength(attackStamina);
	Territory &territory = fleet.getTerritory();
	// Enemy adjacencies include neutral/unowned territories.
	const std::set<Territory*> enemyTerritories = context.getEnemyAdjacencies(territory, true); 
	for(std::set<Territory*>::iterator iter = enemyTerritories.begin();
		iter != enemyTerritories.end(); ++iter)
	{
		// Handle case when adjacent territory is a NavalTerritory.
		// CAN JUST CALL GETTYPE() RATHER THAN CASTING?	
		NavalTerritory* navalTerritory = dynamic_cast<NavalTerritory*>(*iter);
		if(navalTerritory != nullptr)
		{
			const LandArmy *enemyArmy = navalTerritory->getOccupancyHandler()->getArmy();

			if(enemyArmy == nullptr)
			{
				int attackStrength = 0.3 * availableStrength;

				if(attackStrength <= 1)
				{
					attackStrength = availableStrength;
				}
				fleet.move(*navalTerritory, attackStrength);
			}
			else if(enemyArmy->getTotalStrength() < 0.1 * availableStrength)
			{
				int attackStrength = 0.4 * availableStrength;
				if(attackStrength == 0)
				{
					attackStrength = 1;
				}
				fleet.move(*navalTerritory, attackStrength);
			}
			else if(enemyArmy->getTotalStrength() < 0.6 * availableStrength)
			{
				fleet.move(*navalTerritory, availableStrength);
			}
		}

		// Update strength.
		availableStrength = fleet.getStrength(attackStamina);
		// Return since army has no strength left for attacking with.
		if(availableStrength == 0)
		{
			return;
		}
	}
}

void SimplePlayerAI::executeFleetMoveOrders(const std::map<Territory*, int>& strategicValues)
{
	Player &player = getPlayer();
	std::map<Territory*, int> remainingStrategicValues = strategicValues;

	// Find sum of strategic values.
	int totalStrategicValue = 0;
	for(const auto& pair : strategicValues)
	{
		assert(pair.second >= 0);
		totalStrategicValue += pair.second;
	}

	const int navalMoveCost = 1;

	// Total strength of player's land military that is available for movement.
	const int totalAvailableNavyStrength = player.getMilitaryManager().getTotalArmyStrength(navalMoveCost);
	assert(totalAvailableNavyStrength >= 0);
	if(totalAvailableNavyStrength == 0)
	{
		return;
	}

	// Strategic value that is allocated to each unit of army strength.
	float strategicValuePerUnit = (float)totalStrategicValue / (float)totalAvailableNavyStrength;
	strategicValuePerUnit; // testing.
	assert(strategicValuePerUnit >= 0);

	std::vector<std::unique_ptr<NavalFleet>> &fleets = player.getMilitaryManager().getFleets();

	// Keeps track of what amount of strength of a given army is free to move from
	// its current territory. 
	std::map<NavalFleet*, int> freeToMove;
	for(auto &fleet : fleets)
	{
		freeToMove[fleet.get()] = fleet.get()->getStrength(navalMoveCost);
	}

	// Armies are added through movement so only iterate up to pre movement number of armies. 
	const int numFleets = fleets.size();
	assert(numFleets > 0);

	// Distances of each friendly army to borders of own territory with movement inside own territory.
	const int maxDist = 13;
	std::unordered_map<const Territory*, std::unordered_map<int, std::vector<NavalFleet*>>> fleetBorderDistances = context.getFleetBorderDistances(maxDist);

	// Partially allocates armies prioritising closest first. Closer armies provide greater allocation.
	for(int distance = 0; distance <= maxDist; ++distance)
	{
		for(auto& pair : remainingStrategicValues)
		{
			Territory& territory = *pair.first;
			int &strategicValue = pair.second;

			assert(strategicValue >= 0);

			if(strategicValue == 0)
			{
				continue;
			}

			// Move fleets of specified distance towards the territory.
			std::vector<NavalFleet*> &currDistFleets = fleetBorderDistances[&territory][distance];
			for(int i = 0; i < currDistFleets.size(); ++i)
			{

				NavalFleet &fleet = *currDistFleets[i];

				const Territory &armyTerritory = fleet.getTerritory();
				// assert(distance = bfsfriendly(army, territory))
				assert(remainingStrategicValues.count(&territory) == 1);
				assert(strategicValue > 0);
				assert(freeToMove[&fleet] <= army.getStrength(landMoveCost));

				if(freeToMove[&fleet] == 0)
				{
					continue;
				}

				// Move army to friendly territory closest to territory.
				// In future prioritise border territories!!!
				const float moveRatio = 0.7;
				int moveStrength = freeToMove[&fleet] * moveRatio;
				const int maximumAllocation = strategicValue / strategicValuePerUnit;
				// Move strength is capped by maximum allocation amount derived from territory priority.
				moveStrength = std::min(moveStrength, maximumAllocation);
				if(moveStrength != 0)
				{
					fleet.moveClosest(territory, moveStrength, maxDist);
					freeToMove[&fleet] -= moveStrength;
					const int usedStrategicValue = 1 + moveStrength * strategicValuePerUnit;
					strategicValue -= usedStrategicValue;
					if(strategicValue <= 0)
					{
						strategicValue = 0;
						break;
					}
					assert(freeToMove[&fleet] <= fleet.getStrength(navalMoveCost));
				}
			}
		}
	}
}

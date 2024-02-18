#include "SimplePlayerAI.h"
#include "Player.h"
#include "Territory.h"
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include "GameplaySettings.h"
#include <iostream>
#include <algorithm>

SimplePlayerAI::SimplePlayerAI(Game &game, Player &player)
	: PlayerAIComponent(game, player), context(player, game)
{
}

void SimplePlayerAI::handleTurn()
{
	if(getPlayer().hasLiege())
	{
		handleTurnVassal();
	}
	else
	{
		handleTurnNonVassal();
	}
}

void SimplePlayerAI::handleTurnVassal()
{
	executeRebellionAgainstLiege();
}

void SimplePlayerAI::handleTurnNonVassal()
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
			if(territory->getType() != TerritoryType::naval)
			{
				continue;
			}
			assert(calculateFleetStrategicValue(*territory) >= 0);
			navalFleetStrategicValues[territory] = calculateFleetStrategicValue(*territory);
		}
		executeFleetMoveOrders(navalFleetStrategicValues);
	}

	// Strategic values relevant to land armies.
	std::map<Territory*, int> landArmyStrategicValues;
	for(Territory* territory : borderTerritories)
	{
		assert(calculateArmyStrategicValue(*territory) >= 0);
		landArmyStrategicValues[territory] = calculateArmyStrategicValue(*territory);
	}
	executeArmyMoveOrders(landArmyStrategicValues);
}

int SimplePlayerAI::calculateArmyStrategicValue(const Territory &territory)
{
	Player &player = getPlayer();
	Game &game = getGame();

	// Initialise strategic value as nonzero for case where enemy territory has no armies.
	// This ensures that armies can be allocated to this border territory in case where no 
	// enemy armies.
	int strategicValue = 1;

	// Calculate strategic value from threat of attack of territory.
	std::map<const Player*, int> weightedThreats = context.getArmyWeightedThreats(territory);
	int maxThreat = calculateMaxThreat(weightedThreats);  
	int totalThreat = calculateTotalThreat(weightedThreats);  
	strategicValue += maxThreat + totalThreat;

	// Prioritise having land troops on land territories.
	if(territory.getType() == TerritoryType::naval)
	{
		const float navalStategicValueAdjustmentFactor = 0.1;
		strategicValue *= navalStategicValueAdjustmentFactor;

		// No reason to station land army on naval territory with no adjacent enemy land territories.
		if(!territory.getDistanceMap().hasEnemyAdjacencies(TerritoryType::land))
		{
			strategicValue = 0;
		}
		
		// Fleet strength that allows occupying naval territory by land troops.
		// In future require fleet stronger than adjacent enemy fleets.
		const int minFleetStrength = 4;
		// No enemy naval adjacencies of friendly naval territory then we allow land troop occupancy.
		// In future require a specific distance
		bool hasEnemyNavalAdjacency = territory.getDistanceMap().hasEnemyAdjacencies(TerritoryType::naval);
		// Dont station army troops if naval territory under threat i.e. there is an adjacent enemy naval territory.
		// Override this if min fleet strength is met. In future also must consider enemy adjacent port land territories.
		if(hasEnemyNavalAdjacency && (territory.getOccupancyHandler()->getFleet() == nullptr ||
			territory.getOccupancyHandler()->getFleet()->getTotalStrength() < minFleetStrength))
		{
			strategicValue = 0;
		}
	}
	else
	{
		// Reduce strategic value of territory if all adjacent land territories are friendly controlled.
		// This is so armies are not over allocated to coastal borders.
		const std::set<Territory*> &adjacencies = territory.getDistanceMap().getAdjacencies();
		bool allAdjacentLandIsFriendly = true;
		for(const Territory* adjacency : adjacencies)
		{
			const bool isFriendly = sameUpperRealm(&player, adjacency->getEstateOwner());
			if(adjacency->getType() == TerritoryType::land && !isFriendly)
			{
				allAdjacentLandIsFriendly = false;
				break;
			}
		}
		if(allAdjacentLandIsFriendly)
		{
			strategicValue /= 8;
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
		const Player *estateOwner = (*iter)->getEstateOwner();
		if(sameUpperRealm(&player, estateOwner))
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

	// Calculate strategic value from threat of attack on territory.
	std::map<const Player*, int> weightedThreats = context.getFleetWeightedThreats(territory);
	int maxThreat = calculateMaxThreat(weightedThreats);
	int totalThreat = calculateTotalThreat(weightedThreats);
	strategicValue += maxThreat + totalThreat;

	// Add strategic value if enemy adjacencies including unowned exists. 
	// Ensures unowned territories will not be ignored.
	if(context.getEnemyAdjacencies(territory).size() > 0)
	{
		++strategicValue;
	}

	/////////////////
	//CURRENTLY DOES NOTHING
	// Calculate strategic desire to attack enemy neighbouring territories.
	const std::set<Territory*> adjacencies = territory.getDistanceMap().getAdjacencies();
	std::set<const Territory*> enemyTerritories;
	// Determine enemy territories from adjacencies.
	for(std::set<Territory*>::iterator iter = adjacencies.begin();
		iter != adjacencies.end(); ++iter)
	{
		const Player *estateOwner = (*iter)->getEstateOwner();
		if(sameUpperRealm(&player, estateOwner))
		{
			enemyTerritories.insert(*iter);
		}
	}
	/////////////////////////

	assert(strategicValue >= 0);

	return strategicValue;
}

void SimplePlayerAI::executeArmyAttacks(const std::vector<Territory*> &borderTerritories)
{
	Player &player = getPlayer();

	// Iterate over border territories and execute attacks by armies stationed at these territories.
	for(const Territory *territory : borderTerritories)
	{
		// Only players without a liege can execute army attacks.
		assert(!player.hasLiege());
		assert(sameUpperRealm(&player, territory->getEstateOwner()));

		LandArmy *army = player.getMilitaryManager().getArmy(territory);
		if(army != nullptr)
		{
			executeArmyAttack(*army);
		}
	}
}

void SimplePlayerAI::executeArmyAttack(LandArmy &army)
{
	const GameplaySettings &gameplaySettings = getPlayer().getGameplaySettings();

	const int attackStamina = 3;
	int availableAttackStrength = army.getStrength(attackStamina);
	Territory &territory = army.getTerritory();

	// Don't attack if available strength less than occupancy cost with excess.
	const int minStrengthThreshold = gameplaySettings.landHostileOccupancyCost + 2;
	if(availableAttackStrength <= minStrengthThreshold)
	{
		return;
	}

	// Enemy territories adjacent to army terrritory.
	const std::set<Territory*> enemyTerritories = context.getEnemyAdjacencies(territory);

	// Determine maximum usable strength across all army attacks.
	int maximumAttackStrengthAllocation = 0;
	if(enemyTerritories.size() > 1)
	{
		// Ensure percentage of army is left on this territory.
		const int totalArmyStrength = army.getTotalStrength();
		maximumAttackStrengthAllocation = totalArmyStrength * 0.9;

		// Ensure remaining troops exceeds a minimum value.
		const int remainder = totalArmyStrength - maximumAttackStrengthAllocation;
		const int minimumRemainderStrength = 3;
		if(remainder < minimumRemainderStrength)
		{
			maximumAttackStrengthAllocation = minimumRemainderStrength;
		}

		if(availableAttackStrength > maximumAttackStrengthAllocation)
		{
			availableAttackStrength = maximumAttackStrengthAllocation;
		}
	}

	// Iterate over adjacent enemy territories and attack under certain conditions.
	for(std::set<Territory*>::iterator iter = enemyTerritories.begin();
		iter != enemyTerritories.end(); ++iter)
	{
		const int priorStrength = army.getTotalStrength();

		// Handle case when adjacent territory is a LandTerritory.
		LandTerritory* landTerritory = dynamic_cast<LandTerritory*>(*iter);
		if(landTerritory != nullptr)
		{
			const LandArmy *enemyArmy = landTerritory->getOccupancyHandler()->getArmy();

			// No enemy army.
			if(enemyArmy == nullptr)
			{
				// Only attack with small portion of army since no enemy army.
				int attackStrength = 0.3 * availableAttackStrength;

				// Ensure attack strength at least the min strength threshold.
				if(attackStrength <= minStrengthThreshold)
				{
					attackStrength = minStrengthThreshold;
				}

				army.move(*landTerritory, attackStrength);
			}
			// Relatively weak enemy army.
			else if(enemyArmy->getTotalStrength() < 0.1 * availableAttackStrength)
			{
				// Only attack with small portion of army since no enemy army.
				int attackStrength = 0.4 * availableAttackStrength;

				// Ensure attack strength at least the min strength threshold.
				if(attackStrength <= minStrengthThreshold)
				{
					attackStrength = minStrengthThreshold;
				}

				army.move(*landTerritory, attackStrength);
			}
			// Relatively strong enemy army.
			else if(enemyArmy->getTotalStrength() < 0.6 * availableAttackStrength)
			{
				army.move(*landTerritory, availableAttackStrength);
			}
		}

		////////////
		const int postStrength = army.getTotalStrength();
		
		const int strengthChange = postStrength - priorStrength;
		availableAttackStrength -= strengthChange;
		/////////////////////
		/////////////////////////////////
		// The below option makes for stronger AI. The above option makes for more aggressive weaker AI
		// In future for a harder difficulty than simple AI selection, consider using a combination
		// of both AI options.
		//////////
		// Update strength.
		// availableAttackStrength = army.getStrength(attackStamina);
		///////////


		// Return since army has no strength left for attacking with.
		if(availableAttackStrength == 0)
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
		// Only players without a liege can execute army attacks.
		assert(!player.hasLiege());
		assert(sameUpperRealm(territory->getEstateOwner(), &player));

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
	const std::set<Territory*> enemyTerritories = context.getEnemyAdjacencies(territory); 
	for(std::set<Territory*>::iterator iter = enemyTerritories.begin();
		iter != enemyTerritories.end(); ++iter)
	{
		// Handle case when adjacent territory is a NavalTerritory.
		NavalTerritory* navalTerritory = dynamic_cast<NavalTerritory*>(*iter);
		if(navalTerritory != nullptr)
		{
			const NavalFleet *enemyFleet = navalTerritory->getOccupancyHandler()->getFleet();

			if(enemyFleet == nullptr)
			{
				int attackStrength = 0.3 * availableStrength;

				if(attackStrength <= 1)
				{
					attackStrength = availableStrength;
				}
				fleet.move(*navalTerritory, attackStrength);
			}
			else if(enemyFleet->getTotalStrength() < 0.1 * availableStrength)
			{
				int attackStrength = 0.4 * availableStrength;
				if(attackStrength == 0)
				{
					attackStrength = 1;
				}
				fleet.move(*navalTerritory, attackStrength);
			}
			else if(enemyFleet->getTotalStrength() < 0.6 * availableStrength)
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

	// Fleets are added through movement so only iterate up to pre movement number of armies. 
	const int numFleets = fleets.size();
	assert(numFleets >= 0);

	const int maxDist = 13;
	// Distances of each friendly army to borders of own territory with movement inside own territory.
	std::unordered_map<const Territory*, std::unordered_map<int, std::vector<NavalFleet*>>> fleetBorderDistances = context.getFleetBorderDistances(maxDist);

	// Partially allocates fleets prioritising closest first. Closer armies provide greater allocation.
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
				// assert(freeToMove[&fleet] <= fleet.getStrength(landMoveCost));

				if(freeToMove[&fleet] == 0)
				{
					continue;
				}

				// Move army to friendly territory closest to territory.
				// In future prioritise border territories!!!
				const float moveRatio = 0.7;
				int moveStrength = freeToMove[&fleet] * moveRatio;
				// Ensure move strength not 0 if any amount of army is free to move.
				if(moveStrength == 0 && freeToMove[&fleet] <= 2)
				{
					moveStrength = freeToMove[&fleet];
				}
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

bool SimplePlayerAI::executeRebellionAgainstLiege()
{
	assert(getPlayer().getLiege() != nullptr);

	if(!getPlayer().getVassalPolicy().canRebel())
	{
		return false;
	}

	// Cannot rebel if player's liege themselves has a liege.
	if(getPlayer().getLiege()->getLiege() != nullptr)
	{
		return false;
	}

	const int liegeStrength = getPlayer().getLiege()->getMilitaryManager().getTotalArmyStrength(false);
	const int playerStrength = getPlayer().getMilitaryManager().getTotalArmyStrength(false);
	const int resistanceRebellionThreshold = 150;
	if(playerStrength >= liegeStrength)
	{
		getPlayer().rebel();
		return true;
	}
	else if(getPlayer().getVassalPolicy().resistance > resistanceRebellionThreshold)
	{
		getPlayer().rebel();
		return true;
	}

	return false;
}

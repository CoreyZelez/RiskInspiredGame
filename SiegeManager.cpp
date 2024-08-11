#include "SiegeManager.h"
#include "LandArmy.h"
#include "Player.h"
#include "Barony.h"
#include "GameplaySettings.h"
#include <random>

SiegeManager::SiegeManager(const GameplaySettings& gameplaySettings, Barony& barony)
	: gameplaySettings(gameplaySettings), barony(barony)
{
}

void SiegeManager::update()
{
	if(siegingArmy == nullptr)
	{
		// Update resistance when not under siege.
		updateResistance();
	}
}

void SiegeManager::updateSiege()
{
	assert(siegingArmy != nullptr && !siegingArmy->isDead() && resistanceStrength > 0);

	handleSiegeCombat();

	// Handle sieging army taking over barony.
	if (resistanceStrength == 0 && siegingArmy != nullptr)
	{
		assert(!siegingArmy->isDead());

		siegingArmy->getOwner().getRealm().handleBaronyAggressorSiegeVictory(barony);
		removeSiegingArmy();
	}
}

void SiegeManager::handleSiegeBegin(LandArmy& army)
{
	assert(siegingArmy == nullptr || &siegingArmy->getOwner() == &army.getOwner());

	if(!sameUpperRealm(barony.getRuler(), &army.getOwner()))
	{
		initSiege(army);
	}
}

void SiegeManager::handleArmyVacated()
{
	// Army vacating was either sieging the barony or was friendly to barony ruler.
	assert(siegingArmy == nullptr || siegingArmy->isDead());

	if(siegingArmy != nullptr)
	{
		siegeLifted();
	}
}

void SiegeManager::initSiege(LandArmy& army)
{
	assert(siegingArmy == nullptr);
	assert(!sameUpperRealm(&army.getOwner(), barony.getRuler()));

	siegingArmy = &army;
	siegingArmy->setSiegeBarony(&barony);
	
	army.getOwner().getRealm().handleBaronySiegeBegin(barony, true);
	barony.getRuler()->getRealm().handleBaronySiegeBegin(barony, false);

	if(resistanceStrength == 0)
	{
		assert(!siegingArmy->isDead());

		// Sieging army gains control of barony.
		siegingArmy->getOwner().getRealm().handleBaronyAggressorSiegeVictory(barony);
		removeSiegingArmy();
	}
}

bool SiegeManager::activeSiege() const
{
	return siegingArmy != nullptr;
}

Player& SiegeManager::getBaronyRuler()
{
	return *barony.getRuler();
}

void SiegeManager::updateResistance()
{
	const LandTerritoryFeatures& features = barony.getTerritory().getFeatures();

	////////////////////////////////////////////////////////////////
	// TEMPORARY HARD CODED VALUES FOR TESTING.
	resistanceStrength += features.prosperity * 0.01;
	if(resistanceStrength > features.prosperity * 0.2)
	{
		resistanceStrength = features.prosperity * 0.2;
	}
	////////////////////////////////////////////////////////////////
}

void SiegeManager::siegeLifted()
{
	siegingArmy->getOwner().getRealm().handleBaronySiegeLifted(barony, true);
	barony.getRuler()->getRealm().handleBaronySiegeLifted(barony, false);
	removeSiegingArmy();
}

void SiegeManager::removeSiegingArmy()
{
	siegingArmy->setSiegeBarony(nullptr);
	siegingArmy = nullptr;
}

void SiegeManager::handleSiegeCombat()
{
	const int siegingArmyStrength = siegingArmy->getTotalStrength();
	const double strengthRatio = (double)resistanceStrength / siegingArmyStrength;

	int armyStrengthReduction = 0;
	int resistanceStrengthReduction = 0;

	if(resistanceStrength >= siegingArmyStrength)
	{
		// We reduce the resistance strength less due to being greater than sieging army strength.
		const double resistanceReductionMultiplier = 0.5;

		armyStrengthReduction = std::max(1.0,
			(strengthRatio - 1) * resistanceStrength);
		resistanceStrengthReduction = std::min(armyStrengthReduction - 1.0,
			armyStrengthReduction * resistanceReductionMultiplier);
	}
	else
	{
		resistanceStrengthReduction = std::min(1.0,
			((1 / strengthRatio) - 1) * siegingArmyStrength);
		armyStrengthReduction = std::max(armyStrengthReduction - 1.0,
			(1 - strengthRatio) * resistanceStrength);
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dis(0.8, 1.2);

	double randomArmyReductionMultiplier = dis(gen);
	double randomResistanceReductionMultiplier = dis(gen);
	armyStrengthReduction *= gameplaySettings.siegeStrengthLossMultiplier * randomArmyReductionMultiplier;
	resistanceStrengthReduction *= gameplaySettings.siegeStrengthLossMultiplier * randomResistanceReductionMultiplier;

	siegingArmy->reduceStrength(armyStrengthReduction);
	resistanceStrength = std::max(0.0, resistanceStrength - resistanceStrengthReduction);

	// When sieging army dies the update observer function is called which handles siege removal.
	assert(siegingArmy == nullptr || !siegingArmy->isDead());
}

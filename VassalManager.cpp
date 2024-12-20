#include "VassalManager.h"
#include "Player.h"
#include "Estate.h"
#include "Barony.h"
#include "Game.h"
#include <iostream>

VassalManager::VassalManager(Game &game, Player &ruler)
	: game(game), ruler(ruler)
{
}

VassalManager::~VassalManager()
{
}

void VassalManager::setVassalsGameOver()
{
	assert(ruler.checkGameOver());
	for(Player* vassal : vassals)
	{
		vassal->setGameOver();
		vassal->handleGameOver();
	}
}

void VassalManager::drawVassalRealms(sf::RenderWindow &window) const
{
	for(const Player* vassal : vassals)
	{
		vassal->getRealm().draw(window);
	}
}

Player& VassalManager::createVassal(Barony &barony)
{
	Player& newVassal = game.createPlayer();
	newVassal.setLiege(&ruler);
	vassals.push_back(&newVassal);
	conferEstate(newVassal, barony);
	return newVassal;
}

void VassalManager::removeEstatelessVassal(const Player &vassal)
{
	// Vassal should not have an empty realm.
	assert(vassal.getRealm().getEstates().empty());
	vassals.erase(std::remove(vassals.begin(), vassals.end(), &vassal), vassals.end());
}

void VassalManager::removeRebellingVassal(Player &vassal)
{
	assert(!vassal.hasLiege());
	// Vassal should not have an empty realm.
	assert(!vassal.getRealm().getEstates().empty());
	// Vassal should indeed be in vassals.
	assert(std::find(vassals.begin(), vassals.end(), &vassal) != vassals.end());

	// Remove vassal's estates from estates.
	std::unordered_set<const Estate*> vassalEstates = vassal.getRealm().getEstates();
	for(const Estate* estate : vassalEstates)
	{
		assert(estates.count(const_cast<Estate*>(estate)) == 1);
		assert(estate != nullptr);

		ruler.getRealm().removeEstate(*estate);
	}

	// Remove the vassal.
	auto iter = vassals.erase(std::remove(vassals.begin(), vassals.end(), &vassal), vassals.end());
	assert(iter == vassals.end());
}

Player& VassalManager::conferEstate(Player& vassal, Estate &estate)
{
	// Ensure vassal is actually apart of vassal manager.
	assert(std::find(vassals.begin(), vassals.end(), &vassal) != vassals.end());
	// Ensure estate not already apart of realm.
	assert(estates.count(&estate) == 0);

	estates.insert(&estate);

	// Add estate to vassals realm. Returns the direct owner of estate.
	return vassal.getRealm().addEstate(estate);
}

void VassalManager::removeEstate(const Estate &estate)
{
	assert(estates.count(const_cast<Estate*>(&estate)) == 1);
	estates.erase(const_cast<Estate*>(&estate));
}

void VassalManager::ammendUnlandedEstateOwnership()
{
	for(Player *vassal : vassals)
	{
		vassal->getRealm().ammendUnlandedEstateOwnership();
	}
}

int VassalManager::calculateArmySoftCapContribution(double contributionRatio) const
{
	int contribution = 0;
	for(Player *vassal : vassals)
	{
		contribution += contributionRatio * vassal->getRealm().calculateArmySoftCap();
	}
	return contribution;
}

int VassalManager::calculateFleetSoftCapContribution(double contributionRatio) const
{
	int contribution = 0;
	for(Player *vassal : vassals)
	{
		contribution += contributionRatio * vassal->getRealm().calculateFleetSoftCap();
	}
	return contribution;
}

int VassalManager::getTotalArmyReserves() const
{
	int totalArmyReserves = 0;
	for(const Player* vassal : vassals)
	{
		totalArmyReserves += vassal->getMilitaryManager().getTotalArmyStrength(false);
	}
	return totalArmyReserves;
}

const std::vector<Player*>& VassalManager::getVassals() const
{
	return vassals;
}

std::unordered_set<const Estate*> VassalManager::getEstates() const
{
	std::unordered_set<const Estate*> estates;
	for(auto iter = this->estates.begin(); iter != this->estates.end(); ++iter)
	{
		estates.insert(*iter);
	}
	return estates;
}

bool VassalManager::containsEstate(const Estate &estate) const
{
	return (estates.count(const_cast<Estate*>(&estate)) == 1);
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

void VassalManager::addEstate(Estate *estate)
{
	estates.insert(estate);
}


#include "Player.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include "SimplePlayerAI.h"
#include "Estate.h"
#include "Game.h"
#include <assert.h>
#include <iostream>

Player::Player(Game &game, AIPersonality personality, const std::string &realmName)
	: game(game), realm(game, *this, liegePolicy, realmName), diplomacy(diplomacy)
{
}

Player::Player(Game& game, const std::string &realmName)
	: game(game), realm(game, *this, liegePolicy, realmName), 
	AIComponent(std::make_unique<SimplePlayerAI>(game, *this)), diplomacy(diplomacy)
{
}

bool Player::gameOver() const
{
	if(getRealm().getEstates().size() == 0)
	{
		assert(liege == nullptr);
		return true;
	}
	return false;
}

void Player::handleTurn()
{
	if(liege != nullptr)
	{
		vassalPolicy.handleLiegeInfluenceChange(liege->liegePolicy);
	}
	militaryManager.update();
	realm.handleMilitaryYields();
	diplomacy.update();

	if(!isHuman)
	{
		AIComponent.get()->handleTurn();
	}
}

void Player::handleReserveArmyYield(double amount)
{
	if(liege != nullptr)
	{
		// Army amount yielded to player liege.
		const double liegeYield = amount * vassalPolicy.liegeLevyContribution;
		assert(liegeYield >= 0);
		// Army amount yielded to player reserves.
		const double playerYield = amount - liegeYield;
		assert(playerYield >= 0);

		// Recurse on liege.
		liege->handleReserveArmyYield(liegeYield);
		// Add army to reserves since player has a liege.
		militaryManager.addArmyReserves(playerYield);
	}
	else
	{
		// No liege so add army to reinforcements rather than reserves.
		militaryManager.addArmyReinforcements(amount);
	}
}

void Player::handleReserveFleetYield(double amount)
{
	if(liege != nullptr)
	{
		// Army amount yielded to player liege.
		const double liegeYield = amount * vassalPolicy.liegeLevyContribution;
		assert(liegeYield >= 0);
		// Army amount yielded to player reserves.
		const double playerYield = amount - liegeYield;
		assert(playerYield >= 0);

		// Recurse on liege.
		liege->handleReserveFleetYield(liegeYield);
		// Add army to reserves since player has a liege.
		militaryManager.addFleetReserves(playerYield);
	}
	else
	{
		// No liege so add army to reinforcements rather than reserves.
		militaryManager.addFleetReinforcements(amount);
	}
}

void Player::addAttackHistory(Player &enemy)
{
	diplomacy.addAttackHistory(enemy);
}

bool Player::hasLiege() const
{
	return liege != nullptr;
}

const Player* Player::getLiege() const
{
	return liege;
}

Player * Player::getLiege()
{
	return liege;
}

void Player::setLiege(Player * player)
{
	liege = player;
}

void Player::setHuman()
{
	isHuman = true;
}

bool Player::getIsHuman() const
{
	return isHuman;
}

MilitaryManager& Player::getMilitaryManager()
{
	return militaryManager;
}

const MilitaryManager & Player::getMilitaryManager() const
{
	return militaryManager;
}

Realm& Player::getRealm()
{
	return realm;
}

const Realm& Player::getRealm() const
{
	return realm;
}

const LiegePolicy& Player::getLiegePolicy() const
{
	return liegePolicy;
}

const VassalPolicy& Player::getVassalPolicy() const
{
	return vassalPolicy;
}

bool Player::sameUpperLiege(const Player &player) const
{
	const Player &upperLiege1 = getUpperLiege();
	const Player &upperLiege2 = player.getUpperLiege();
	return &upperLiege1 == &upperLiege2;
}

Player& Player::getUpperLiege()
{
	Player *upperLiege = this;
	while(upperLiege->liege != nullptr)
	{
		upperLiege = upperLiege->liege;
	}
	return *upperLiege;
}

const Player& Player::getUpperLiege() const
{
	const Player *upperLiege = this;
	while(upperLiege->liege != nullptr)
	{
		upperLiege = upperLiege->liege;
	}
	return *upperLiege;
}

bool Player::isVassal(const Player &player, bool direct) const
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

	// Check if realm's ruler is an indirect vassal of player.
	// Iterates through liege hierarchy of ruler to check for occurrence of player.
	const Player *currLiege = liege;
	while(currLiege != nullptr)
	{
		if(currLiege == &player)
		{
			return true;
		}
		currLiege = currLiege->liege;
	}

	return false;
}

bool sameUpperRealm(const Player *player1, const Player *player2)
{
	if(player1 == nullptr || player2 == nullptr)
	{
		return false;
	}
	else
	{
		return player1->sameUpperLiege(*player2);
	}
}



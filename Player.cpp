#include "Player.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include "SimplePlayerAI.h"
#include "Estate.h"
#include "Game.h"
#include "GameplaySettings.h"
#include "IntegerSlider.h"
#include "IntegerSlider.cpp"
#include "NumericTextDisplay.h"
#include "NumericTextDisplay.cpp"
#include "TextDisplay.h"
#include "InteractionPanel.h"
#include <assert.h>
#include <iostream>

Player::Player(Game &game, AIPersonality personality, const std::string &realmName)
	: game(game), militaryManager(*this), realm(game, *this, liegePolicy, realmName), diplomacy(*this), vassalPolicy(*this)
{
}

Player::Player(Game& game, const std::string &realmName)
	: game(game), militaryManager(*this), realm(game, *this, liegePolicy, realmName),
	AIComponent(std::make_unique<SimplePlayerAI>(game, *this)), diplomacy(*this), vassalPolicy(*this)
{
}

std::unique_ptr<UIEntity> Player::createUI(UIType type)
{
	if(type == UIType::interaction)
	{
		const int textDisplayWidth = 180;
		const int headingWidth = 600;
		const int headingCharacterSize = 40;
		const int headingHeight = 50;
		const int sliderHeight = 95;
		sf::Vector2f sliderDimensions(headingWidth, sliderHeight);

		std::vector<std::vector<std::unique_ptr<UIEntity>>> entities;

		// Barony limit slider heading.
		std::vector<std::unique_ptr<UIEntity>> row1;
		std::unique_ptr<TextDisplay> rulerBaronyLimitHeading =
			std::make_unique<TextDisplay>("Ruler Barony Limit");
		rulerBaronyLimitHeading.get()->setBackgroundSize(sf::Vector2f(headingWidth, headingHeight));
		rulerBaronyLimitHeading.get()->setBackgroundColor(sf::Color(47, 47, 47));
		rulerBaronyLimitHeading.get()->setPadding(10);
		rulerBaronyLimitHeading.get()->setCharacterSize(headingCharacterSize);
		row1.push_back(std::move(rulerBaronyLimitHeading));

		// Unlanded limit slider heading.
		std::vector<std::unique_ptr<UIEntity>> row3;
		std::unique_ptr<TextDisplay> rulerUnlandedLimitHeading =
			std::make_unique<TextDisplay>("Ruler Unlanded Limit");
		rulerUnlandedLimitHeading.get()->setBackgroundSize(sf::Vector2f(headingWidth, headingHeight));
		rulerUnlandedLimitHeading.get()->setBackgroundColor(sf::Color(47, 47, 47));
		rulerUnlandedLimitHeading.get()->setPadding(10);
		rulerUnlandedLimitHeading.get()->setCharacterSize(headingCharacterSize);
		row3.push_back(std::move(rulerUnlandedLimitHeading));

		// Vassal limit slider heading.
		std::vector<std::unique_ptr<UIEntity>> row5;
		std::unique_ptr<TextDisplay> rulerVassalLimitHeading =
			std::make_unique<TextDisplay>("Ruler Vassal Limit");
		rulerVassalLimitHeading.get()->setBackgroundSize(sf::Vector2f(headingWidth, headingHeight));
		rulerVassalLimitHeading.get()->setBackgroundColor(sf::Color(47, 47, 47));
		rulerVassalLimitHeading.get()->setPadding(10);
		rulerVassalLimitHeading.get()->setCharacterSize(headingCharacterSize);
		row5.push_back(std::move(rulerVassalLimitHeading));

		// Barony conferral realm size weight heading.
		std::vector<std::unique_ptr<UIEntity>> row7;
		std::unique_ptr<TextDisplay> rulerRealmSizeWeightHeading =
			std::make_unique<TextDisplay>("Realm Size Weight");
		rulerRealmSizeWeightHeading.get()->setBackgroundSize(sf::Vector2f(headingWidth, headingHeight));
		rulerRealmSizeWeightHeading.get()->setBackgroundColor(sf::Color(47, 47, 47));
		rulerRealmSizeWeightHeading.get()->setPadding(10);
		rulerRealmSizeWeightHeading.get()->setCharacterSize(headingCharacterSize);
		row7.push_back(std::move(rulerRealmSizeWeightHeading));

		// Barony conferral liege influence weight heading.
		std::vector<std::unique_ptr<UIEntity>> row9;
		std::unique_ptr<TextDisplay> rulerLiegeInfluenceWeightHeading =
			std::make_unique<TextDisplay>("Liege Influence Weight");
		rulerLiegeInfluenceWeightHeading.get()->setBackgroundSize(sf::Vector2f(headingWidth, headingHeight));
		rulerLiegeInfluenceWeightHeading.get()->setBackgroundColor(sf::Color(47, 47, 47));
		rulerLiegeInfluenceWeightHeading.get()->setPadding(10);
		rulerLiegeInfluenceWeightHeading.get()->setCharacterSize(headingCharacterSize);
		row9.push_back(std::move(rulerLiegeInfluenceWeightHeading));

		// Barony conferral related estates weight heading.
		std::vector<std::unique_ptr<UIEntity>> row11;
		std::unique_ptr<TextDisplay> rulerLiegeRelatedEstatesHeading =
			std::make_unique<TextDisplay>("Related Estates Weight");
		rulerLiegeRelatedEstatesHeading.get()->setBackgroundSize(sf::Vector2f(headingWidth, headingHeight));
		rulerLiegeRelatedEstatesHeading.get()->setBackgroundColor(sf::Color(47, 47, 47));
		rulerLiegeRelatedEstatesHeading.get()->setPadding(10);
		rulerLiegeRelatedEstatesHeading.get()->setCharacterSize(headingCharacterSize);
		row11.push_back(std::move(rulerLiegeRelatedEstatesHeading));

		int characterSize = 80;

		// Barony limit slider.
		std::vector<std::unique_ptr<UIEntity>> row2;
		std::unique_ptr<UIEntity> rulerBaronyLimitSlider = 
			std::make_unique<IntegerSlider<int>>(0, 8, liegePolicy.rulerBaronyLimit, sliderDimensions);
		std::unique_ptr<NumericTextDisplay<int>> rulerBaronyLimitTextDisplay =
			std::make_unique<NumericTextDisplay<int>>(liegePolicy.rulerBaronyLimit, "", "");
		rulerBaronyLimitTextDisplay.get()->setBackgroundSize(sf::Vector2f(textDisplayWidth, sliderHeight));
		rulerBaronyLimitTextDisplay.get()->setBackgroundColor(sf::Color(50, 50, 50));
		rulerBaronyLimitTextDisplay.get()->setCharacterSize(characterSize);
		rulerBaronyLimitTextDisplay.get()->setTextColor(sf::Color(200, 200, 200));
		row2.push_back(std::move(rulerBaronyLimitSlider));
		row2.push_back(std::move(rulerBaronyLimitTextDisplay));

		// Unlanded limit slider.
		std::vector<std::unique_ptr<UIEntity>> row4;
		std::unique_ptr<UIEntity> rulerUnlandedLimitSlider =
			std::make_unique<IntegerSlider<int>>(0, 12, liegePolicy.rulerUnlandedLimit, sliderDimensions);
		std::unique_ptr<NumericTextDisplay<int>> rulerUnlandedLimitTextDisplay =
			std::make_unique<NumericTextDisplay<int>>(liegePolicy.rulerUnlandedLimit, "", "");
		rulerUnlandedLimitTextDisplay.get()->setBackgroundSize(sf::Vector2f(textDisplayWidth, sliderHeight));
		rulerUnlandedLimitTextDisplay.get()->setBackgroundColor(sf::Color(50, 50, 50));
		rulerUnlandedLimitTextDisplay.get()->setCharacterSize(characterSize);
		rulerUnlandedLimitTextDisplay.get()->setTextColor(sf::Color(200, 200, 200));
		row4.push_back(std::move(rulerUnlandedLimitSlider));
		row4.push_back(std::move(rulerUnlandedLimitTextDisplay));

		// Vassal limit slider.
		std::vector<std::unique_ptr<UIEntity>> row6;
		std::unique_ptr<UIEntity> rulerVassalLimitSlider =
			std::make_unique<IntegerSlider<int>>(0, 30, liegePolicy.rulerVassalLimit, sliderDimensions);
		std::unique_ptr<NumericTextDisplay<int>> rulerVassalLimitTextDisplay =
			std::make_unique<NumericTextDisplay<int>>(liegePolicy.rulerVassalLimit, "", "");
		rulerVassalLimitTextDisplay.get()->setBackgroundSize(sf::Vector2f(textDisplayWidth, sliderHeight));
		rulerVassalLimitTextDisplay.get()->setBackgroundColor(sf::Color(50, 50, 50));
		rulerVassalLimitTextDisplay.get()->setCharacterSize(characterSize);
		rulerVassalLimitTextDisplay.get()->setTextColor(sf::Color(200, 200, 200));
		row6.push_back(std::move(rulerVassalLimitSlider));
		row6.push_back(std::move(rulerVassalLimitTextDisplay));

		const double divisor = 100;

		// Barony conferral realm size weight slider.
		std::vector<std::unique_ptr<UIEntity>> row8;
		std::unique_ptr<UIEntity> rulerRealmSizeWeightSlider =
			std::make_unique<IntegerSlider<double>>(0, 100, liegePolicy.baronyConferralRealmSizeWeight, divisor, sliderDimensions);
		std::unique_ptr<NumericTextDisplay<double>> rulerRealmSizeWeightTextDisplay =
			std::make_unique<NumericTextDisplay<double>>(liegePolicy.baronyConferralRealmSizeWeight, "", "%", divisor);
		rulerRealmSizeWeightTextDisplay.get()->setBackgroundSize(sf::Vector2f(textDisplayWidth, sliderHeight));
		rulerRealmSizeWeightTextDisplay.get()->setBackgroundColor(sf::Color(50, 50, 50));
		rulerRealmSizeWeightTextDisplay.get()->setCharacterSize(characterSize);
		rulerRealmSizeWeightTextDisplay.get()->setTextColor(sf::Color(200, 200, 200));
		row8.push_back(std::move(rulerRealmSizeWeightSlider));
		row8.push_back(std::move(rulerRealmSizeWeightTextDisplay));

		// Barony conferral liege influence weight slider.
		std::vector<std::unique_ptr<UIEntity>> row10;
		std::unique_ptr<UIEntity> rulerLiegeInfluenceWeightSlider =
			std::make_unique<IntegerSlider<double>>(0, 100, liegePolicy.baronyConferralLiegeInfluenceWeight, divisor, sliderDimensions);
		std::unique_ptr<NumericTextDisplay<double>> rulerLiegeInfluenceWeightTextDisplay =
			std::make_unique<NumericTextDisplay<double>>(liegePolicy.baronyConferralLiegeInfluenceWeight, "", "%", divisor);
		rulerLiegeInfluenceWeightTextDisplay.get()->setBackgroundSize(sf::Vector2f(textDisplayWidth, sliderHeight));
		rulerLiegeInfluenceWeightTextDisplay.get()->setBackgroundColor(sf::Color(50, 50, 50));
		rulerLiegeInfluenceWeightTextDisplay.get()->setCharacterSize(characterSize);
		rulerLiegeInfluenceWeightTextDisplay.get()->setTextColor(sf::Color(200, 200, 200));
		row10.push_back(std::move(rulerLiegeInfluenceWeightSlider));
		row10.push_back(std::move(rulerLiegeInfluenceWeightTextDisplay));

		// Barony conferral liege influence weight slider.
		std::vector<std::unique_ptr<UIEntity>> row12;
		std::unique_ptr<UIEntity> rulerRelatedEstatesWeightSlider =
			std::make_unique<IntegerSlider<double>>(0, 100, liegePolicy.baronyConferralRelatedEstatesWeight, divisor, sliderDimensions);
		std::unique_ptr<NumericTextDisplay<double>> rulerRelatedEstatesTextDisplay =
			std::make_unique<NumericTextDisplay<double>>(liegePolicy.baronyConferralRelatedEstatesWeight, "", "%", divisor);
		rulerRelatedEstatesTextDisplay.get()->setBackgroundSize(sf::Vector2f(textDisplayWidth, sliderHeight));
		rulerRelatedEstatesTextDisplay.get()->setBackgroundColor(sf::Color(50, 50, 50));
		rulerRelatedEstatesTextDisplay.get()->setCharacterSize(characterSize);
		rulerRelatedEstatesTextDisplay.get()->setTextColor(sf::Color(200, 200, 200));
		row12.push_back(std::move(rulerRelatedEstatesWeightSlider));
		row12.push_back(std::move(rulerRelatedEstatesTextDisplay));

		entities.push_back(std::move(row1));
		entities.push_back(std::move(row2));
		entities.push_back(std::move(row3));
		entities.push_back(std::move(row4));
		entities.push_back(std::move(row5));
		entities.push_back(std::move(row6));
		entities.push_back(std::move(row7));
		entities.push_back(std::move(row8));
		entities.push_back(std::move(row9));
		entities.push_back(std::move(row10));
		entities.push_back(std::move(row11));
		entities.push_back(std::move(row12));

		std::unique_ptr<UIEntity> panel = std::make_unique<InteractionPanel>(std::move(entities));
		return panel;
	}
}

bool Player::checkGameOver()
{
	if(gameOver)
	{
		return true;
	}

	if(liege != nullptr)
	{
		return false;
	}

	// Game is over when liegeless player has no remaining estates on LandTerritory and has no land armies.
	if(getMilitaryManager().getTotalArmyStrength(0) == 0 && getRealm().hasNoBaronies())
	{
		gameOver = true;
	}

	return gameOver;
}

void Player::handleGameOver()
{
	realm.handleGameOver();
	militaryManager.handleGameOver();
}

void Player::setGameOver()
{
	gameOver = true;
}

void Player::handleTurn()
{
	if(liege != nullptr)
	{
		vassalPolicy.handleLiegeInfluenceChange(liege->liegePolicy);
		vassalPolicy.handleResistanceChange(*liege, liege->liegePolicy);
	}
	militaryManager.update();
	realm.handleMilitaryYields();
	diplomacy.update();

	if(!isHuman)
	{
		AIComponent.get()->handleTurn();
	}
}

void Player::rebel()
{
	// Player must have a liege.
	assert(liege != nullptr);
	// Players liege must not have a liege.
	assert(liege->liege == nullptr);

	Player* oldLiege = liege;
	// Null liege first since otherwise assert will fail in diplomacy class.
	liege = nullptr;
	oldLiege->handleVassalRebellion(*this);

	// Player loses reserves for rebelling.
	const double reserveRemovalRatio = 0.5;
	militaryManager.removeArmyReserves(reserveRemovalRatio);

	// Yield military reserves.
	// Some armies may be yielded to pre-rebelion liege occupied territories. 
	yieldArmyReserves();

	// Check ownership changes of territories due to pre-rebellion liege armies.
	std::unordered_set<Territory*> territories = realm.getTerritories();
	for(Territory *territory : territories)
	{
		territory->getOccupancyHandler()->determineOccupation();
	}
}

void Player::handleReinforcementArmyYield(double amount)
{
	assert(liege == nullptr);
	assert(amount >= 0);
	const double multiplier = realm.getEffectiveArmyYieldRatio();
	const double adjustedAmount = amount * multiplier;
	militaryManager.addArmyReinforcements(adjustedAmount);
}

void Player::handleReinforcementFleetYield(double amount)
{
	assert(liege == nullptr);
	assert(amount >= 0);
	const double multiplier = realm.getEffectiveFleetYieldRatio();
	const double adjustedAmount = amount * multiplier;
	militaryManager.addFleetReinforcements(adjustedAmount);
}

/*
 * Determine adjusted amount based on the effective army yield ratio. This is the actual amount
 * of armies yielded to both the player and their liege (if any) combined. The amount yielded to
 * liege is dependent on the pre-adjusted amount and does not exceed the adjusted amount.
 */
void Player::handleReserveArmyYield(double amount)
{
	// Amount of armies adjusted for yield ratio.
	double adjustedAmount = amount * getRealm().getEffectiveArmyYieldRatio();
	if(liege != nullptr)
	{
		// Army amount yielded to player liege.
		const double liegeYield = std::min(amount * vassalPolicy.liegeLevyContribution, adjustedAmount);
		assert(liegeYield >= 0);
		// Army amount yielded to player reserves.
		const double playerYield = adjustedAmount - liegeYield;
		assert(playerYield >= 0);

		// Recurse on liege.
		liege->handleReserveArmyYield(liegeYield);
		// Add army to reserves since player has a liege.
		militaryManager.addArmyReserves(playerYield);
	}
	else
	{
		// No liege so add army to reinforcements rather than reserves.
		militaryManager.addArmyReinforcements(adjustedAmount);
	}
}

void Player::handleReserveFleetYield(double amount)
{
	// Amount of fleet adjusted for yield ratio.
	double adjustedAmount = amount * getRealm().getEffectiveFleetYieldRatio();
	if(liege != nullptr)
	{
		// Fleet amount yielded to player liege.
		const double liegeYield = std::min(amount * vassalPolicy.liegeLevyContribution, adjustedAmount);
		assert(liegeYield >= 0);
		// Fleet amount yielded to player reserves.
		const double playerYield = adjustedAmount - liegeYield;
		assert(playerYield >= 0);

		// Recurse on liege.
		liege->handleReserveFleetYield(liegeYield);
		// Add fleet to reserves since player has a liege.
		militaryManager.addFleetReserves(playerYield);
	}
	else
	{
		// No liege so add fleet to reinforcements rather than reserves.
		militaryManager.addFleetReinforcements(adjustedAmount);
	}
}

void Player::addAttackHistory(Player &enemy)
{
	diplomacy.addAttackHistory(enemy);
}

void Player::removeDiplomacyWithPlayer(Player &player)
{
	diplomacy.removeDiplomacyWithPlayer(player);
}

void Player::setDiplomacyColors(const std::vector<std::unique_ptr<Player>> &players)
{
	diplomacy.setColors(players);
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

const GameplaySettings & Player::getGameplaySettings() const
{
	return game.getGameplaySettings();
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

void Player::handleVassalRebellion(Player &vassal)
{
	realm.removeRebellingVassal(vassal);
	diplomacy.addRebellingVassal(vassal);

	// We ammend the ownership of unlanded estates since the rebelling vassal take with them baronies which may have formed
	// estates held by their old liege and the old liege's realm may hold baronies which form estates held by the rebelling vassal.
	vassal.getRealm().ammendUnlandedEstateOwnership();
	realm.ammendUnlandedEstateOwnership();
}

void Player::yieldArmyReserves()
{
	realm.yieldArmyReserves();
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



#include "Realm.h"
#include "Player.h"
#include "Estate.h"
#include "Utility.h"
#include "MilitaryManager.h"
#include "FontManager.h"
#include "RichText.h"
#include "InformationPanel.h"
#include "LiegePolicy.h"
#include "Game.h"
#include "Barony.h"
#include <assert.h>
#include <iostream>
#include <unordered_map>

Realm::Realm(Game &game, Player &ruler, const LiegePolicy &liegePolicy)
	: ruler(ruler), vassalManager(game, ruler), liegePolicy(liegePolicy)
{
}

void Realm::draw(sf::RenderWindow &window) const
{
	realmGrid.draw(window);
	// Draw vassal realms on top of entire realm grid if specified.
	if(vassalView)
	{
		vassalManager.drawVassalRealms(window);
	}
}

std::unique_ptr<UIEntity> Realm::getUI(UIType type) const
{
	FontManager &fontManager = FontManager::getInstance();
	if(type == UIType::information)
	{
		const sf::Font &font = *fontManager.getFont("UIFont1");

		// Name text.
		sfe::RichText nameText(font);
		nameText << sf::Text::Regular << sf::Color::White << "Name: "
			<< sf::Color::Yellow << "tempName";

		std::map<Title, int> titleCounts = getTitleCounts();

		// Barony count text.
		sfe::RichText baronyCntText(font);
		baronyCntText << sf::Text::Regular << sf::Color::White << "Number of Baronies: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::barony]);
		// County count text.
		sfe::RichText countyCntText(font);
		countyCntText << sf::Text::Regular << sf::Color::White << "Number of Counties: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::county]);
		// Duchy count text.
		sfe::RichText duchyCntText(font);
		duchyCntText << sf::Text::Regular << sf::Color::White << "Number of Duchies: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::duchy]);
		// Kingdom count text.
		sfe::RichText kingdomCntText(font);
		kingdomCntText << sf::Text::Regular << sf::Color::White << "Number of Kingdoms: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::kingdom]);
		// Empire count text.
		sfe::RichText empireCntText(font);
		empireCntText << sf::Text::Regular << sf::Color::White << "Number of Empires: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::empire]);

		std::vector<sfe::RichText> texts = { nameText };

		if(titleCounts[Title::barony] > 0)
		{
			texts.push_back(baronyCntText);
		}
		if(titleCounts[Title::county] > 0)
		{
			texts.push_back(countyCntText);
		}
		if(titleCounts[Title::duchy] > 0)
		{
			texts.push_back(duchyCntText);
		}
		if(titleCounts[Title::kingdom] > 0)
		{
			texts.push_back(kingdomCntText);
		}
		if(titleCounts[Title::empire] > 0)
		{
			texts.push_back(empireCntText);
		}

		return std::make_unique<InformationPanel>(texts);
	}

	return nullptr;
}

void Realm::handleMilitaryYields()
{
	rulerEstateManager.handleMilitaryYields();
}

Player& Realm::addEstate(Estate &estate)
{
	const LandedEstate* landedEstate = dynamic_cast<const LandedEstate*>(&estate);
	// Update realm grid if estate is landed.
	if(landedEstate != nullptr)
	{
		realmGrid.addGrid(landedEstate->getGrid());
	}

	// Confer the estate to vassal or ruler and return the recipient.
	return allocate(estate);
}

void Realm::removeEstate(Estate &estate)
{
	if(rulerEstateManager.containsEstate(estate))
	{
		assert(estate.compareRuler(&ruler));
		rulerEstateManager.removeEstate(estate);
	}
	else
	{
		assert(!estate.compareRuler(&ruler));
		assert(vassalManager.containsEstate(estate));
		vassalManager.removeEstate(estate);
	}

	// Recurse upon lieges to ensure estate and associated territory if landed is removed
	// from all upper liege realms.
	if(ruler.getLiege() != nullptr)
	{
		ruler.getLiege()->getRealm().removeEstate(estate);
	}

	LandedEstate *landedEstate = dynamic_cast<LandedEstate*>(&estate);
	// Update realm grid if estate is landed.
	if(landedEstate != nullptr)
	{
		realmGrid.removeGrid(landedEstate->getGrid());
	}
}

std::unordered_set<Territory*> Realm::getTerritories()
{
	std::unordered_set<Territory*> realmTerritories;
	const std::unordered_set<Territory*> &rulerTerritories = rulerEstateManager.getTerritories(); 
	const std::unordered_set<Territory*> &vassalTerritories = vassalManager.getTerritories();
	realmTerritories = rulerTerritories;
	realmTerritories.insert(vassalTerritories.begin(), vassalTerritories.end());
	return realmTerritories;
}

std::unordered_set<const Estate*> Realm::getEstates() const
{
	std::unordered_set<const Estate*> realmEstates;
	const std::unordered_set<const Estate*> rulerEstates = rulerEstateManager.getEstates();
	const std::unordered_set<const Estate*> vassalEstates = vassalManager.getEstates();
	realmEstates = rulerEstates;
	realmEstates.insert(vassalEstates.begin(), vassalEstates.end());
	return realmEstates;
}

void Realm::updateGrid()
{
	realmGrid.updateGrid();
}

bool Realm::containsPosition(const sf::Vector2f &position, bool considerVassalView) const
{
	if(considerVassalView && vassalView)
	{
		return rulerEstateManager.landedEstatesContainsPosition(position);
	}
	else
	{
		return realmGrid.containsPosition(position);
	}
}

void Realm::setVassalView(bool vassalView)
{
	this->vassalView = vassalView;
}

void Realm::setGridColor(const sf::Color & color)
{
	realmGrid.setGridColor(color);
}

void Realm::setGridColorDefault()
{
	realmGrid.setGridColorDefault();
}

Title Realm::getHighestRulerTitle() const
{
	return rulerEstateManager.getHighestTitle();
}

Player & Realm::allocate(Estate & estate)
{
	std::map<Title, int> rulerTitleCounts = rulerEstateManager.getTitleCounts();
	const Title estateTitle = estate.getTitle();
	assert(estateTitle != Title::noTitle);

	// CURRENTLY ALWAYS ALLOCATED MARIDOMS TO RULER. IN FUTURE MAYBE CHANGE.
	if(estateTitle == Title::maridom)
	{
		rulerEstateManager.addEstate(estate);
		return ruler;
	}
	else if(estateTitle == Title::barony)
	{
		// Confer barony to ruler.
		if(rulerTitleCounts[Title::barony] < liegePolicy.rulerBaronyLimit)
		{
			rulerEstateManager.addEstate(estate);
			return ruler;
		}
		// Confer barony to new vassal.
		else if(vassalManager.getVassals().size() < liegePolicy.rulerVassalLimit)
		{
			Barony *barony = dynamic_cast<Barony*>(&estate);
			assert(barony != nullptr);  // Must hold as the estate title is barony.
			// Create a vassal conferring them the barony and return the created vassal.
			return vassalManager.createVassal(*barony);
		}
		// Confer barony to existing vassal.
		else
		{
			const std::vector<Player*> vassals = vassalManager.getVassals();
			Barony *barony = dynamic_cast<Barony*>(&estate);
			assert(vassals.size() > 0);
			assert(barony != nullptr);  // Must hold as the estate title is barony.
			// Vassal which barony is conferred to.
			Player &vassal = getGreatestBaronyInfluence(*barony, vassals);
			return vassalManager.conferEstate(vassal, estate);
		}
	}
	else
	{
		// Estate should not be landed.
		assert(dynamic_cast<LandedEstate*>(&estate) == nullptr);

		const Title highestRulerTitle = ruler.getRealm().getHighestRulerTitle();

		// If no vassals the estate must be conferred to ruler.
		if(vassalManager.getVassals().size() == 0)
		{
			rulerEstateManager.addEstate(estate);
			return ruler;
		}
		// Confer to ruler if estate title is higher than rulers highest titled estate.
		// This promotes having the ruler hold the highest titled estate in the realm.
		else if(estate.getTitle() > highestRulerTitle)
		{
			rulerEstateManager.addEstate(estate);
			return ruler;
		}
		// Confer to ruler.
		else if(countUnlandedEstates(rulerTitleCounts) < liegePolicy.rulerUnlandedLimit)
		{
			rulerEstateManager.addEstate(estate);
			return ruler;
		}
		// Confer to vassal.
		else
		{
			const std::vector<Player*> vassals = vassalManager.getVassals();
			assert(vassals.size() > 0);
			// Vassal which barony is conferred to.
			Player &vassal = getGreatestUnlandedEstateInfluence(estate, vassals);
			return vassalManager.conferEstate(vassal, estate);
		}
	}
}

std::map<Title, int> Realm::getTitleCounts() const
{
	std::map<Title, int> realmTitleCounts;
	std::map<Title, int> rulerTitleCounts = rulerEstateManager.getTitleCounts();
	std::map<Title, int> vassalsTitleCounts = vassalManager.getTitleCounts();
	realmTitleCounts.insert(rulerTitleCounts.begin(), rulerTitleCounts.end());
	realmTitleCounts.insert(vassalsTitleCounts.begin(), vassalsTitleCounts.end());
	return realmTitleCounts;

}

/*
 * Players gain influence score for controlling baronies in estates shared with the parameter barony. Players will
 * gain relatively greater amounts of influence score for baronies in relatively lower titled unlanded estates.
 * A reference to the player with the highest influence score is returned.
 */
Player& getGreatestBaronyInfluence(const Barony &barony, const std::vector<Player*>& players)
{
	assert(players.size() > 0);

	std::unordered_map<const Player*, int> influenceScores;
	// Influence score contributions of baronies which share estate of given title.
	std::unordered_map<Title, int> influenceContributions;
	influenceContributions[Title::county] = 14;
	influenceContributions[Title::duchy] = 5;
	influenceContributions[Title::kingdom] = 2;
	influenceContributions[Title::empire] = 1;

	// Calculate influence scores of each player.
	for(const Estate *upperEstate = barony.getParent(); upperEstate != nullptr; upperEstate = upperEstate->getParent())
	{
		const Title title = upperEstate->getTitle();
		assert(influenceContributions[title] > 0);

		for(const Player* player : players)
		{
			// Number of owned landed subfief estates of upper estate.
			const int landedEstateCount = upperEstate->calculateLandedSubfiefOwnershipCount(*player);
			const int influenceContribution = landedEstateCount * influenceContributions[title];
			assert(influenceContribution >= 0);
			influenceScores[player] += influenceContribution;
		}
	}

	// Player with the most influence.
	Player* maxInfluencePlayer = players[0];
	// Greatest influence score.
	int maxInfluenceScore = influenceScores[maxInfluencePlayer] = 0;
	// Determine player with maximum influence score.
	// Tie breaks through choosing first player in vector with max score.
	for(Player* player : players)
	{
		if(influenceScores[player] > maxInfluenceScore)
		{
			maxInfluencePlayer = player;
			maxInfluenceScore = influenceScores[player];
		}
	}

	return *maxInfluencePlayer;
}

/*
 * Players gain influence score for controlling subfiefs of the estate and baronies of the parent estate.
 * The greater the title of the controlled subfief, the more influence score awarded. A reference to the
 * player with the greatest influence score is returned.
 */
Player& getGreatestUnlandedEstateInfluence(const Estate &estate, const std::vector<Player*>& players)
{
	// Ensure that estate is unlanded.
	assert(dynamic_cast<const LandedEstate*>(&estate) == nullptr);
	assert(players.size() > 0);

	std::unordered_map<const Player*, int> influenceScores;
	// Influence score contribution of baronies in parent estate.
	const int parentEstateBaronyInfluence = 1;
	// Influence score contributions of baronies which share estate of given title.
	std::unordered_map<Title, int> subfiefInfluenceContributions;
	subfiefInfluenceContributions[Title::barony] = 2;
	subfiefInfluenceContributions[Title::county] = 6;
	subfiefInfluenceContributions[Title::duchy] = 12;
	subfiefInfluenceContributions[Title::kingdom] = 24;

	// Current upper estate we calculate each players influence contribution from.
	const Estate* upperEstate = estate.getParent();
	// Calculate influence score contributions from baronies in upper estate.
	if(upperEstate != nullptr)
	{
		for(const Player* player : players)
		{
			// Number of owned landed subfief estates of upper estate.
			const int landedEstateCount = upperEstate->calculateLandedSubfiefOwnershipCount(*player);
			const int influenceContribution = landedEstateCount * parentEstateBaronyInfluence;
			assert(influenceContribution >= 0);
			influenceScores[player] += influenceContribution;
		}
	}

	// Calculate influence score contributions from lower estates of estate.
	std::unordered_set<const Estate*> lowerEstates = estate.getLowerEstates();
	for(const Estate* lowerEstate : lowerEstates)
	{
		for(const Player* player : players)
		{
			const Player* lowerEstateRuler = lowerEstate->getRuler();
			assert(lowerEstateRuler != nullptr);
			// Add influence score to player if player is a ruler of lower estate or has a direct or indirect vassal ruling lower estate.
			if(player == lowerEstateRuler || lowerEstateRuler->isVassal(*player, false))
			{
				const Title title = lowerEstate->getTitle();
				influenceScores[player] += subfiefInfluenceContributions[title];
			}
		}
	}

	// Player with the most influence.
	Player* maxInfluencePlayer = players[0];
	// Greatest influence score.
	int maxInfluenceScore = influenceScores[maxInfluencePlayer] = 0;
	// Determine player with maximum influence score.
	// Tie breaks through choosing first player in vector with max score.
	for(Player* player : players)
	{
		if(influenceScores[player] > maxInfluenceScore)
		{
			maxInfluencePlayer = player;
			maxInfluenceScore = influenceScores[player];
		}
	}

	return *maxInfluencePlayer;

}

int countUnlandedEstates(std::map<Title, int>& estateCounts)
{
	const std::vector<Title> orderedUnlandedTitles = getOrderedUnlandedTitles();
	int cnt = 0;
	for(Title title : orderedUnlandedTitles)
	{
		cnt += estateCounts[title];
	}
	return cnt;
}





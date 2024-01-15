#include "Realm.h"
#include "Player.h"
#include "Estate.h"
#include "Utility.h"
#include "MilitaryManager.h"
#include "FontManager.h"
#include "RichText.h"
#include "InformationPanel.h"
#include "LiegePolicy.h"
#include "NameGenerator.h"
#include "Game.h"
#include "Barony.h"
#include <assert.h>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

Realm::Realm(Game &game, Player &ruler, const LiegePolicy &liegePolicy, const std::string &name)
	: ruler(ruler), vassalManager(game, ruler), liegePolicy(liegePolicy), name(name)
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
		nameText << sf::Text::Regular << sf::Color::White << "Realm name: "
			<< sf::Color::Yellow << name;

		std::map<Title, int> titleCounts = getTitleCounts();

		// Barony count text.
		sfe::RichText baronyCntText(font);
		baronyCntText << sf::Text::Regular << sf::Color::White << "Total number of Baronies: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::barony]);
		// County count text.
		sfe::RichText countyCntText(font);
		countyCntText << sf::Text::Regular << sf::Color::White << "Total number of Counties: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::county]);
		// Duchy count text.
		sfe::RichText duchyCntText(font);
		duchyCntText << sf::Text::Regular << sf::Color::White << "Total number of Duchies: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::duchy]);
		// Kingdom count text.
		sfe::RichText kingdomCntText(font);
		kingdomCntText << sf::Text::Regular << sf::Color::White << "Total number of Kingdoms: "
			<< sf::Color::Yellow << std::to_string(titleCounts[Title::kingdom]);
		// Empire count text.
		sfe::RichText empireCntText(font);
		empireCntText << sf::Text::Regular << sf::Color::White << "Total number of Empires: "
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

		// Ruler direct owned baronies text.
		const int directBaronyCnt = rulerEstateManager.getTitleCounts()[Title::barony];
		sfe::RichText directBaronyCntText(font);
		directBaronyCntText << sf::Text::Regular << sf::Color::White << "Ruler directly controlled baronies: "
			<< sf::Color::Yellow << std::to_string(directBaronyCnt);
		texts.push_back(directBaronyCntText);

		// Ruler direct vassal count text.
		const int directVassalCnt = vassalManager.getVassals().size();
		sfe::RichText directVassalCntText(font);
		directVassalCntText << sf::Text::Regular << sf::Color::White << "Ruler directly controlled vassals: "
			<< sf::Color::Yellow << std::to_string(directVassalCnt);
		texts.push_back(directVassalCntText);

		return std::make_unique<InformationPanel>(texts);
	}

	return nullptr;
}

void Realm::handleMilitaryYields()
{
	rulerEstateManager.handleMilitaryYields();
}

void Realm::removeRebellingVassal(Player &vassal)
{
	// Remove the vassals realm grid from this grid.
	realmGrid.removeGrid(vassal.getRealm().realmGrid);
	vassalManager.removeRebellingVassal(vassal);
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

	// Remove ruler as vassal of liege if no estates remaining in realm.
	if(ruler.getLiege() != nullptr && getEstates().empty())
	{
		ruler.getLiege()->getRealm().vassalManager.removeEstatelessVassal(ruler);
		ruler.setLiege(nullptr);
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

int Realm::getTotalVassalArmyReserves() const
{
	return vassalManager.getTotalArmyReserves();
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

Player& Realm::allocate(Estate &estate)
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
		Barony *barony = dynamic_cast<Barony*>(&estate);
		assert(barony != nullptr);  // Must hold as the estate title is barony.
		// Confer barony to ruler.
		if(shouldConferBaronyToRuler(*barony))
		{
			rulerEstateManager.addEstate(estate);
			return ruler;
		}
		// Confer barony to new vassal.
		else if(shouldConferBaronyToNewVassal(*barony))
		{
			// Create a vassal conferring them the barony and return the created vassal.
			return vassalManager.createVassal(*barony);
		}
		// Confer barony to existing vassal.
		else
		{
			const std::vector<Player*> vassals = vassalManager.getVassals();
			assert(vassals.size() > 0);
			// Vassal which barony is conferred to.
			Player *vassal = getHighestBaronyConferralScoreVassal(*barony);
			assert(vassal != nullptr);
			return vassalManager.conferEstate(*vassal, estate);
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

	for(auto &pair : rulerTitleCounts)
	{
		const Title title = pair.first;
		const int count = pair.second;
		realmTitleCounts[title] += count;
	}
	for(auto &pair : vassalsTitleCounts)
	{
		const Title title = pair.first;
		const int count = pair.second;
		realmTitleCounts[title] += count;
	}
	return realmTitleCounts;

}

bool Realm::shouldConferBaronyToRuler(Barony &barony) const
{
	if(liegePolicy.rulerVassalLimit == 0 && vassalManager.getVassals().size() == 0)
	{
		return true;
	}
	else if(liegePolicy.rulerBaronyLimit <= rulerEstateManager.getTitleCounts()[Title::barony])
	{
		return false;
	}

	// Amount of baronies that are guaranteed to be conferred to ruler regardless of other factors except limit.
	const int guaranteedConferralAmount = 2;
	if(liegePolicy.rulerBaronyLimit >= guaranteedConferralAmount && rulerEstateManager.getTitleCounts()[Title::barony] < guaranteedConferralAmount)
	{
		return true;
	}

	// The maximum number of landed estates ruler can control per specified estate. Limits only enforced
	// when ruler has no vassals and will not create new vassals.
	std::unordered_map<Title, int> rulerBaronyLimits;
	rulerBaronyLimits[Title::county] = 1;
	rulerBaronyLimits[Title::duchy] = 3;
	rulerBaronyLimits[Title::kingdom] = 6;
	rulerBaronyLimits[Title::empire] = INT_MAX;
	const Estate *upperEstate = barony.getParent();
	while(upperEstate != nullptr)
	{
		const Title title = upperEstate->getTitle();
		const int landedEstateCount = upperEstate->calculateBaronySubfiefOwnershipCount(ruler, true);
		if(landedEstateCount >= rulerBaronyLimits[title])
		{
			return false;
		}
		upperEstate = upperEstate->getParent();
	}
	return true;
}

/* 
 * A new vassal is created when the barony is belonging to a county which player's realm contains no other
 * baronies within. An exception to this rule is when the number of vassals has not reached some minimum. In
 * that case, create new vassals until minimum number of vassals reached. A new vassal is never created when
 * the vassal limit is reached or exceeded.
 */
bool Realm::shouldConferBaronyToNewVassal(Barony &barony) const
{
	const int numVassals = vassalManager.getVassals().size();
	const int minVassalGuaranteeForNewVassalCreation = 3;

	if(numVassals >= liegePolicy.rulerVassalLimit)
	{
		return false;
	}

	// Confer the barony to a new vassal if a minimum number of vassals are not currently in realm.
	if(numVassals < minVassalGuaranteeForNewVassalCreation)
	{
		return true;
	}

	const Estate *upperEstate = barony.getParent();
	if(upperEstate != nullptr && upperEstate->getTitle() == Title::county)
	{
		std::unordered_set<const Estate*> lowerEstates = upperEstate->getLowerEstates();
		for(const Estate *estate : lowerEstates)
		{
			if(sameUpperRealm(estate->getRuler(), &ruler))
			{
				return false;
			}
		}
	}

	return true;
}

Player* Realm::getHighestBaronyConferralScoreVassal(const Barony &barony) const
{
	const std::vector<Player*> &vassals = vassalManager.getVassals();
	if(vassals.size() == 0)
	{
		return nullptr;
	}

	std::unordered_map<Player*, double> conferralScores;
	for(Player *vassal : vassals)
	{
		// No vassal should be estateless.
		assert(!vassal->getRealm().getEstates().empty());

		conferralScores[vassal] += ruler.getLiegePolicy().baronyConferralLiegeInfluenceWeight * vassal->getVassalPolicy().liegeInfluence;
		conferralScores[vassal] += ruler.getLiegePolicy().baronyConferralRealmSizeWeight * realmSizeBaronyConferralContribution(*vassal);
		conferralScores[vassal] += ruler.getLiegePolicy().baronyConferralRelatedEstatesWeight * realmEstatesInfluenceBaronyConferralContribution(*vassal, barony);
	}

	Player *maxScoreVassal = nullptr;
	double maxScore = -100000;
	for(Player *vassal : vassals)
	{
		assert(conferralScores[vassal] > -1000);
		if(conferralScores[vassal] > maxScore)
		{
			maxScoreVassal = vassal;
			maxScore = conferralScores[vassal];
		}
	}

	assert(maxScoreVassal != nullptr);
	return maxScoreVassal;
}

double Realm::realmSizeBaronyConferralContribution(const Player &vassal) const
{
	assert(vassalManager.getVassals().size() != 0);

	// Total baronies in ruler realm.
	const int realmBaronies = getTitleCounts()[Title::barony];
	// Percent of total realm baronies vassal controls that results in 0 contribution to conferral score.
	const double equilibriumRealmProportion = (double)1 / vassalManager.getVassals().size();
	assert(equilibriumRealmProportion > 0);
	// Total baronies in vassal realm.
	const int vassalRealmBaronies = vassal.getRealm().getTitleCounts()[Title::barony];
	// Proportion vassal realm encompasses lieges realm.
	const double vassalRealmProportion = (double)vassalRealmBaronies / realmBaronies;
	// Residual value.
	const double residualRealmProportion = equilibriumRealmProportion - vassalRealmProportion;
	double conferralScoreContribution;
	if(residualRealmProportion > 0)
	{
		const double contributionUpperBound = 100;
		double conferralScoreContributionRatio = residualRealmProportion / equilibriumRealmProportion;
		assert(conferralScoreContributionRatio > 0);
		conferralScoreContribution = conferralScoreContributionRatio * contributionUpperBound;
	}
	else
	{
		const double contributionLowerBound = -100;
		// Ratio of residual proportion to equilibrium proportion resulting in maximised negative conferral contribution score.
		const double negativeContriutionMaximalRatio = -4;
		double baseConferralScoreContributionRatio = residualRealmProportion / equilibriumRealmProportion;
		assert(baseConferralScoreContributionRatio <= 0);
		double adjustedConferralScoreContributionRatio = -baseConferralScoreContributionRatio / negativeContriutionMaximalRatio;
		if(adjustedConferralScoreContributionRatio < -1)
		{
			adjustedConferralScoreContributionRatio = -1;
		}
		conferralScoreContribution = -adjustedConferralScoreContributionRatio * contributionLowerBound;
	}

	return conferralScoreContribution;
}

double Realm::realmEstatesInfluenceBaronyConferralContribution(const Player &vassal, const Barony &barony) const
{
	// Influence score contributions of baronies which share estate of given title.
	std::unordered_map<Title, int> influenceContributions;
	influenceContributions[Title::county] = 14;
	influenceContributions[Title::duchy] = 5;
	influenceContributions[Title::kingdom] = 2;
	influenceContributions[Title::empire] = 1;
	const double liegeBaronyInfluence = calculateBaronyInfluence(barony, vassal, influenceContributions);
	const double vassalBaronyInfluence = calculateBaronyInfluence(barony, vassal, influenceContributions);
	assert(liegeBaronyInfluence >= vassalBaronyInfluence);
	const double maxConferralContribution = 100;
	// Reduction constant ensures vassal not awarded too high contribution in case where liege influence is very
	// small due to small amount of related baronies controlled.
	const double reductionConstant = 40;
	const double baronyConferralContributionRatio = vassalBaronyInfluence / (liegeBaronyInfluence + reductionConstant);
	const double baronyConferralContribution = baronyConferralContributionRatio * maxConferralContribution;
	return baronyConferralContribution;
}

/*
 * Players gain influence score for controlling baronies in estates shared with the parameter barony. Players will
 * gain relatively greater amounts of influence score for baronies in relatively lower titled unlanded estates.
 * A reference to the player with the highest influence score is returned.
 */
Player& getGreatestBaronyInfluence(const Barony &barony, const std::vector<Player*>& players)
{
	assert(players.size() > 0);

	// Influence score contributions of baronies which share estate of given title.
	std::unordered_map<Title, int> influenceContributions;
	influenceContributions[Title::county] = 14;
	influenceContributions[Title::duchy] = 5;
	influenceContributions[Title::kingdom] = 2;
	influenceContributions[Title::empire] = 1;

	// Calculate influence score for each player.
	std::unordered_map<const Player*, int> influenceScores;
	for(const Player* player : players)
	{
		influenceScores[player] = calculateBaronyInfluence(barony, *player, influenceContributions);
	}

	// Player with the most influence.
	Player* maxInfluencePlayer = players[0];
	// Greatest influence score.
	int maxInfluenceScore = influenceScores[maxInfluencePlayer];
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

int calculateBaronyInfluence(const Barony & barony, const Player & player, std::unordered_map<Title, int>& influenceContributions)
{
	// Calculate influence scores of player.
	int influenceScore = 0;
	for(const Estate *upperEstate = barony.getParent(); upperEstate != nullptr; upperEstate = upperEstate->getParent())
	{
		const Title title = upperEstate->getTitle();
		assert(influenceContributions[title] > 0);

		// Number of owned landed subfief estates of upper estate.
		const int landedEstateCount = upperEstate->calculateBaronySubfiefOwnershipCount(player, false);
		const int influenceContribution = landedEstateCount * influenceContributions[title];

		assert(influenceContribution >= 0);
		influenceScore += influenceContribution;
	}

	return influenceScore;
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
			const int landedEstateCount = upperEstate->calculateBaronySubfiefOwnershipCount(*player, false);
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





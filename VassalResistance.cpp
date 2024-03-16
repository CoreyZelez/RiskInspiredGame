#include "VassalResistance.h"
#include "LiegePolicy.h"
#include "Player.h"

VassalResistance::VassalResistance(Player &player)
	: player(player)
{
	player.getRealm().addObserver(this);
}


void VassalResistance::update(Message message)
{
	// Handle resistance changes due to gain or loss of estates.
	// Gain lose relatively more resistance compared to gain.
	// Barony.
	if(message == lostBarony)
	{
		const double resistanceChange = 3;
		resistance += resistanceChange;
	}
	if(message == receivedBarony)
	{
		const double resistanceChange = -8;
		resistance += resistanceChange;
	}
	// County.
	if(message == lostCounty)
	{
		const double resistanceChange = 3;
		resistance += resistanceChange;
	}
	if(message == receivedCounty)
	{
		const double resistanceChange = -6;
		resistance += resistanceChange;
	}
	// Duchy.
	if(message == lostDuchy)
	{
		const double resistanceChange = 4;
		resistance += resistanceChange;
	}
	if(message == receivedDuchy)
	{
		const double resistanceChange = -8;
		resistance += resistanceChange;
	}
	// Kingdom.
	if(message == lostKingdom)
	{
		const double resistanceChange = 5;
		resistance += resistanceChange;
	}
	if(message == receivedKingdom)
	{
		const double resistanceChange = -10;
		resistance += resistanceChange;
	}
	// Empire
	if(message == lostEmpire)
	{
		const double resistanceChange = 5;
		resistance += resistanceChange;
	}
	if(message == receivedEmpire)
	{
		const double resistanceChange = -20;
		resistance += resistanceChange;
	}
}

void VassalResistance::update(const Player &liege, const LiegePolicy &liegePolicy)
{
	assert(player.hasLiege());

	double resistanceChange = 0;

	// Resistance reduces by multiplication factor.
	// This stops resistance from growing far past the rebellion threshold. 
	const double resistanceReductionFactor = 0.005;
	resistanceChange = resistance * resistanceReductionFactor;

	// Increase resistance by some constant.
	const double resistanceConstant = 0.1;
	resistanceChange += resistanceConstant;

	// Adjust resistance due to liege levy.
	const double equilibriumLevy = 0.3;
	const double residualLevy = liegePolicy.vassalLevy - equilibriumLevy;
	const double levyResistance = 3 * residualLevy;
	resistanceChange += levyResistance;

	// Adjust resistance due to difference between liege and players highest title.
	const Title highestLiegeTitle = liege.getRealm().getHighestRulerTitle();
	const Title highestPlayerTitle = player.getRealm().getHighestRulerTitle();
	// + 1 since having same highest title should increase resistance.
	const int titleDifference = (int)highestPlayerTitle - (int)highestLiegeTitle + 1;
	double titleResistance = titleDifference * 0.1;
	// Relatively more change to resistance when player's title exceeds liege's. 
	if(titleResistance > 0)
	{
		titleResistance *= 3;
	}
	resistanceChange += titleResistance;

	//////////////////
	//////////////////
	//////////////////
	///////// IMPLEMENT
	// Adjust resistance due to liege not having dejure upper title of player's highest title(s).
	//////////////////
	//
	//////////////////

	// Adjust resistance due to total military strength relative to liege's of all vassals combined.
	const double equilibriumRatio = 0.9;
	const int totalLiegeArmyStrength = liege.getMilitaryManager().getTotalArmyStrength(false);
	const int totalVassalArmyReserves = liege.getRealm().getTotalVassalArmyReserves();
	// Minimum vassal reserves for any resistance to possibly be generated.
	const int minVassalReserves = 3;
	if(totalVassalArmyReserves <= minVassalReserves)
	{
		// Do nothing.
	}
	else if(totalLiegeArmyStrength == 0)
	{
		resistanceChange += 100;
	}
	else
	{
		const double armyRatio = totalVassalArmyReserves / totalLiegeArmyStrength;
		const double residualRatio = armyRatio - equilibriumRatio;
		double armyResistance = residualRatio;
		resistanceChange += armyResistance;
	}

	// Less resistance change when liege has liege.
	if(liege.hasLiege())
	{
		const double reductionFactor = 0.5;
		resistanceChange *= reductionFactor;
	}

	// Add change to resistance.
	resistance += resistanceChange;

	// Resistance is non negative.
	if(resistance < 0)
	{
		resistance = 0;
	}
}

bool VassalResistance::canRebel() const
{
	const int rebellionThreshold = player.getLiege()->getLiegePolicy().resistanceThreshold;
	return resistance >= rebellionThreshold;
}

int VassalResistance::getResistance() const
{
	return resistance;
}

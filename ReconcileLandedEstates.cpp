#include "ReconcileLandedEstates.h"
#include "EstateManager.h"

ReconcileLandedEstates::ReconcileLandedEstates(EstateManager & estateManager,
	const std::vector<std::unique_ptr<LandTerritory>>& landTerritories,
	const std::vector<std::unique_ptr<NavalTerritory>>& navalTerritories)
	: estateManager(estateManager), landTerritories(landTerritories), navalTerritories(navalTerritories)
{
}

void ReconcileLandedEstates::execute()
{
	estateManager.reconcileBaronies(landTerritories);
	estateManager.reconcileMaridoms(navalTerritories);
}
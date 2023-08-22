#include "ReconcileBaronies.h"
#include "EstateManager.h"

ReconcileBaronies::ReconcileBaronies(EstateManager &estateManager, const std::vector<std::unique_ptr<LandTerritory>>& landTerritories)
	: estateManager(estateManager), landTerritories(landTerritories)
{
}

void ReconcileBaronies::execute()
{
	estateManager.reconcileBaronies(landTerritories);
}

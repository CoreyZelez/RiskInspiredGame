#pragma once
#include "Command.h"
#include <vector>
#include <memory>

class LandTerritory;
class NavalTerritory;
class EstateManager;

class ReconcileLandedEstates : public Command
{
public:
	ReconcileLandedEstates(EstateManager &estateManager,
		const std::vector<std::unique_ptr<LandTerritory>> &landTerritories,
		const std::vector<std::unique_ptr<NavalTerritory>> &navalTerritories);
	virtual void execute() override;

private:
	EstateManager &estateManager;
	const std::vector<std::unique_ptr<LandTerritory>> &landTerritories;
	const std::vector<std::unique_ptr<NavalTerritory>> &navalTerritories;
};


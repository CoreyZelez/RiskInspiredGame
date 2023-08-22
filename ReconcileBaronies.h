#pragma once
#include "Command.h"
#include <vector>
#include <memory>

class LandTerritory;
class EstateManager;

class ReconcileBaronies : public Command
{
public:
	ReconcileBaronies(EstateManager &estateManager, const std::vector<std::unique_ptr<LandTerritory>> &landTerritories);
	virtual void execute() override;

private:
	EstateManager &estateManager;
	const std::vector<std::unique_ptr<LandTerritory>> &landTerritories;
};


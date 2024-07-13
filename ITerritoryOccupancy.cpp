#include "ITerritoryOccupancy.h"
#include "Player.h"
#include <assert.h>

const Player* ITerritoryOccupancy::getController() const
{
    return controller;
}

Player* ITerritoryOccupancy::getController()
{
    // Only liegless players can be considered the controller of a territory.
    assert(controller == nullptr || !controller->hasLiege());

    return controller;
}

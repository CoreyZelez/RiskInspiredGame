#include "ITerritoryOccupancy.h"
#include "Player.h"
#include <assert.h>

const Player* ITerritoryOccupancy::getController() const
{
    return controller;
}

Player* ITerritoryOccupancy::getController()
{
    return controller;
}

#include "AIPlayerComponent.h"

AIPlayerComponent::AIPlayerComponent(Game & game, Player & player, AIPersonality personality)
	: game(game), player(player), personality(personality)
{
}

void AIPlayerComponent::handleTurn()
{
}

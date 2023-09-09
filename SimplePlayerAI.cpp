#include "SimplePlayerAI.h"
#include "Player.h"

SimplePlayerAI::SimplePlayerAI(Game &game, Player &player)
	: PlayerAIComponent(game, player)
{
}

void SimplePlayerAI::handleTurn()
{
	Player &player = getPlayer();
	Game &game = getGame();
	MilitaryManager &militaryManager = player.getMilitaryManager();
	Realm &realm = player.getRealm();
}

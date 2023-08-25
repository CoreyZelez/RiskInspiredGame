#include "GameController.h"
#include "InputUtility.h"

GameController::GameController(Game &game, GameView &gameView)
	: game(game), gameView(gameView)
{
}

void GameController::handleInput()
{
	InputUtility &inputUtility = InputUtility::getInstance();

}

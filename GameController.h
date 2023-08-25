#pragma once

class Game;
class GameUI;

class GameController
{
public:
	GameController(Game &game, GameUI &gameUI);

	/* On mouseclick notify gameview to see if any buttons pressed. game view
	then returns button id. the controller handles necessary actions for game and gameview.
	Another action to take on mouse click is to check for what estates were clicked on. This is
	partially dependant on the map mode and which estates the map mode is showing e.g. just baronies
	or only the highest titles etc. Then pass the view the estate to display information about it. View
	may also create buttons to influence the estate if it is a humans turn.*/
	void handleInput();

private:
	Game &game;
	GameUI &gameView;
};

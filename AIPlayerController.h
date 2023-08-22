#pragma once
#include "IController.h"

class Player;

struct PersonalityAI
{
	int aggression;  // Preference to play defensively vs offensively.
	int invadeSmallLargePreference;  // Preference towards invading small vs large enemy realms.
	int weakStrongVassalPreference;  // Preference towards granting fiefs to weak vs strong vassals.
	int vassalTaxPreference;  // Preference to tax vassals less vs more.
	int dejurePreference;  // Preference to target dejure baronies of owned estates and parent estates.
};

class AIPlayerController : public IController
{
public:
	AIPlayerController(Player &player, PersonalityAI personality);

	// Empty function.
	virtual void handleInput(const sf::RenderWindow &window, sf::View &view) override;

	// Completes an entire player turn. 
	virtual void action();
	/* one approach for decision making is to first anlyse neightbouring enemy territories and provide each a score
	for whether they should be attacked. Consider things such as enemy strength and threat of counterattack. Also analyse
	own personal border territories and determine which are likely to be attacked and provide a score for whether to defend
	them. perhaps final decision might involve both attacking enemy territory and moving armies to defend own territory. 
	
	player should provide a getter to an information object which has all the relevant information for ai decision making.*/

private:
	Player &player;
	PersonalityAI personality;
};


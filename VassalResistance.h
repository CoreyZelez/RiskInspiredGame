#pragma once
#include "Observer.h"

class Player;
struct LiegePolicy;

class VassalResistance : public Observer
{
public:
	VassalResistance(Player &player);

	virtual void update(Message message) override;

	// Adjusts resistance against liege for end of turn.
	void update(const Player &liege, const LiegePolicy &liegePolicy);
	// Returns true if resistance exceeds threshold.
	bool canRebel() const;
	int getResistance() const;

private:
	const Player &player;
	double resistance = 0;
};


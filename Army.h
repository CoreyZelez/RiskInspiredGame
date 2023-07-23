#pragma once

class Player;
class Territory;

class Army
{
public:
	Army(const Player &player);

private:
	const Player &owner;
	Territory *location;
};


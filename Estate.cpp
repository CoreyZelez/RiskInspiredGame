#include "Estate.h"
#include "Player.h"
#include <assert.h>

Estate::Estate(Title title, const Player *ruler)
	: title(title), ruler(ruler)
{
}

Estate::Estate(Title title)
	: title(title)
{
}

Title Estate::getTitle() const
{
	return title;
}

const Player* Estate::getRuler() const
{
	return ruler;
}

void Estate::setRuler(const Player *ruler)
{
	this->ruler = ruler;
}





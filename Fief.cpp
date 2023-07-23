#include "Fief.h"
#include <assert.h>

Fief::Fief(Title title, const Player *ruler)
	: title(title), ruler(ruler)
{
}

Title Fief::getTitle() const
{
	return title;
}

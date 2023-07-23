#include "Fief.h"
#include <assert.h>

Fief::Fief(Title title, Player *ruler)
	: title(title), ruler(ruler)
{
}

Title Fief::getTitle() const
{
	return title;
}

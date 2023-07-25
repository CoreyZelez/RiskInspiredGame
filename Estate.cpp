#include "Estate.h"
#include <assert.h>

Estate::Estate(Title title, const Player *ruler)
	: title(title), ruler(ruler)
{
}

Title Estate::getTitle() const
{
	return title;
}

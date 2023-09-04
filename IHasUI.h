#pragma once
#include "UIComponent.h"

class IHasUI
{
public:
	virtual UIComponent getUI() const = 0;
};


#pragma once
#include "UIPanel.h"
#include <memory>

class HasUI
{
public:
	virtual std::unique_ptr<UIPanel> getUI() const = 0;

};


#pragma once
#include "UIPanel.h"
#include <memory>

enum class UIType
{
	information,
	interaction
};

class HasUI
{
public:
	virtual std::unique_ptr<UIPanel> getUI(UIType type) const = 0;

};


#pragma once
#include <memory>
#include "UIEntity.h"

class UIEntity;

enum class UIType
{
	information,
	interaction
};

class HasUI
{
public:
	virtual std::unique_ptr<UIEntity> getUI(UIType type) const = 0;
};


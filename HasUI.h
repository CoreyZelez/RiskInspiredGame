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
	// Calls const version of this function. Override when needing UI to change object state.
	virtual std::unique_ptr<UIEntity> createUI(UIType type);
	// Implement this function for any UI which does not change object state.
	virtual std::unique_ptr<UIEntity> createUI(UIType type) const;
};


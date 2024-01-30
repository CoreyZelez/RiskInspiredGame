#include "HasUI.h"

std::unique_ptr<UIEntity> HasUI::createUI(UIType type)
{
	const HasUI* constThis = this;
	return constThis->createUI(type);
}

std::unique_ptr<UIEntity> HasUI::createUI(UIType type) const
{
	return nullptr;
}

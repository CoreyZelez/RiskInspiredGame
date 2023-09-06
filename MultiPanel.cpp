#include "MultiPanel.h"

MultiPanel::MultiPanel(std::vector<std::vector<std::unique_ptr<UIPanel>>> panels)
	: panels(std::move(panels))
{
}

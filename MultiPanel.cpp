#include "MultiPanel.h"

MultiPanel::MultiPanel(std::vector<std::vector<std::unique_ptr<UIPanel>>> panels)
	: UIPanel(sf::Color(200, 200, 200)), panels(std::move(panels))
{
}

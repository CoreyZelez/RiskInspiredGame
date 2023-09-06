#pragma once
#include "UIPanel.h"
#include <memory>

class MultiPanel : UIPanel
{
public:
	MultiPanel(std::vector<std::vector<std::unique_ptr<UIPanel>>> panels);

private:
	std::vector<std::vector<std::unique_ptr<UIPanel>>> panels;  // Each vector of panels represents a row of panels. Ordered.
};


#pragma once
#include "Territory.h"
#include "Observer.h"
#include "Subject.h"
#include "Port.h"
#include <memory>

class NavalFleet;
class LandArmy;

class LandTerritory : public Territory 
{
public:
	LandTerritory(int id, Grid graphics);
	explicit LandTerritory(int id);

	virtual ~LandTerritory() = default;

	// Save label is identifier in txt file for territory type.
	virtual std::string getSaveLabel() const override;

	bool hasPort() const;

private:
	std::unique_ptr<Port> port = nullptr;
};
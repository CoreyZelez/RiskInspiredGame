#include "VassalPolicy.h"
#include "LiegePolicy.h"
#include <iostream>

void VassalPolicy::handleLiegeInfluenceChange(const LiegePolicy &liegePolicy)
{
	const double reductionConstant = 0.3;
	liegeInfluence += (liegeLevyContribution - liegePolicy.vassalLevy) / (liegePolicy.vassalLevy + reductionConstant);
	const double reductionFactor = 0.99;
	liegeInfluence *= reductionFactor;
}

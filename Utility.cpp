#include "Utility.h"
#include <sstream>
#include <iomanip>
#include <random>

std::string doubleToStringWithDecimals(double value, int decimals)
{
	std::ostringstream stream;

	if(decimals == 0) {
		// If 0 decimals, format without decimal point
		stream << std::fixed << std::setprecision(0) << value;
	}
	else {
		// Set the desired number of decimal places
		stream << std::fixed << std::setprecision(decimals) << value;
	}

	std::string result = stream.str();

	// If no decimals specified, remove decimal point
	if(decimals == 0) {
		size_t pos = result.find('.');
		if(pos != std::string::npos) {
			result.erase(pos, 1);
		}
	}

	return result;
}

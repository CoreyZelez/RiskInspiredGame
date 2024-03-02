#pragma once
#include "Factory.h"
#include "Factory.cpp"
#include <SFML/Graphics.hpp>

class Culture;

class CultureFactory : public Factory<int, Culture>

{
public:
	CultureFactory();
};
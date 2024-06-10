#pragma once
#include "Factory.h"
#include "Factory.cpp"
#include <SFML/Graphics.hpp>

struct Culture;

class CultureFactory : public Factory<int, Culture>

{
public:
	CultureFactory();
};
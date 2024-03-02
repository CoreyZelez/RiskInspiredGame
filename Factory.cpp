#pragma once
#include "Factory.h"
#include <assert.h>

template<typename I, typename T>
Factory<I, T>::Factory(std::map<I, T> map)
	: map(map)
{
}

template<typename I, typename T>
T Factory<I, T>::create(I id) const
{
	assert(has(id));

	return map.at(id);
}

template<typename I, typename T>
bool Factory<I, T>::has(I id) const
{
	return map.count(id) == 1;
}

template<typename I, typename T>
void Factory<I, T>::add(I id, T t)
{
	assert(!has(id));

	map[id] = t;
}

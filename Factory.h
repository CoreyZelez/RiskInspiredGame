#pragma once
#include <map>

// Factory with object T uniquely identified by I.
template<typename I, typename T> class Factory
{
public:
	Factory() = default;
	Factory(std::map<I, T> map);

	T create(I id) const;
	bool has(I id) const;

protected:
	void add(I id, T t);

private:
	std::map<I, T> map;
};



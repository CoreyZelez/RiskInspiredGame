#pragma once
#include <assert.h>
#include <vector>

enum class Title
{
	noTitle = 0,
	admiral = -1,
	baron = 1,  // Be very careful adding new titles as there are loops dependant on the current title set. In general baron should = 1 always.
	count = 2,
	duke = 3,
	king = 4,
	emperor = 5
};

const std::vector<Title> getOrderedTitles();

inline
bool operator>=(Title lhs, Title rhs)
{
	return static_cast<int>(lhs) >= static_cast<int>(rhs);
}

inline
Title& operator--(Title& title) {
	int intValue = static_cast<int>(title);
	intValue--;
	assert(intValue >= static_cast<int>(Title::noTitle));
	title = static_cast<Title>(intValue);
	return title;
}

inline
Title& operator--(Title& title, int) {
	Title originalTitle = title;
	--title;
	return originalTitle;
}

inline
Title operator-(Title lhs, int rhs)
{
	return static_cast<Title>(static_cast<int>(lhs) - rhs);
}

struct TitleComparer
{
	bool operator()(const Title& lhs, const Title& rhs) const
	{
		return static_cast<int>(lhs) < static_cast<int>(rhs);
	}
};
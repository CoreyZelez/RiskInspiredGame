#pragma once
#include <assert.h>

enum class Title
{
	noTitle,
	baron,
	count,
	duke,
	king,
	emperor
};

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
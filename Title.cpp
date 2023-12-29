#include "Title.h"

const std::vector<Title> getOrderedTitles()
{
	return {
		    Title::maridom,
			Title::barony,
			Title::county,
			Title::duchy,
			Title::kingdom,
			Title::empire
	};
}

const std::vector<Title> getOrderedUnlandedTitles()
{
	return {
			Title::county,
			Title::duchy,
			Title::kingdom,
			Title::empire
	};
}

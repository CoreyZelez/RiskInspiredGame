#include "Subject.h"
#include "Observer.h"
#include <algorithm>
#include <iostream>

Subject::~Subject()
{
	// Remove all observers.
	for(Observer *observer : observers)
	{
		removeObserver(observer);
	}
}

void Subject::addObserver(Observer *observer)
{
	observers.push_back(observer);
	observer->setSubject(this);
}

void Subject::removeObserver(Observer *observer)
{
	auto iter = observers.begin();
	while(iter != observers.end())
	{
		if(*iter == observer) 
		{
			observers.erase(iter);
			observer->setSubject(nullptr);
			return;
		}
		++iter;
	}
}

void Subject::notifyObservers(Message message)
{
	for(Observer* observer : observers) 
	{
		observer->update(message);
	}
}

#include "Subject.h"
#include "Observer.h"
#include <algorithm>

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

			iter = observers.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void Subject::notifyObservers(Message message)
{
	for(Observer* observer : observers) {
		observer->update(message);
	}
}

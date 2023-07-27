#include "Subject.h"
#include "Observer.h"
#include <algorithm>

void Subject::addObserver(Observer * observer)
{
	observers.push_back(observer);
}

void Subject::removeObserver(Observer *observer)
{
	observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Subject::notifyObservers(Message message)
{
	for(Observer* observer : observers) {
		observer->update(message);
	}
}

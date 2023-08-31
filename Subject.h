#pragma once
#include "Message.h"
#include <vector>

class Observer;

class Subject 
{
public:
	virtual ~Subject();

	void addObserver(Observer* observer);

	void removeObserver(Observer* observer); 

	void notifyObservers(Message message); 

private:
	std::vector<Observer*> observers;
};
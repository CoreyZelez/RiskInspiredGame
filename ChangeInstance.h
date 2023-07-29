#pragma once
#include "Command.h"

template<typename T> class ChangeInstance : public Command
{
public:
	ChangeInstance(T & instance, T target) 
		: instance(instance), target(target) {}

	virtual void execute() override
	{
		instance = target;
	}

private:
	T &instance;
	T target;

};


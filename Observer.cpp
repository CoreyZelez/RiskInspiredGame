#include "Observer.h"
#include "Subject.h"
#include <assert.h>
#include <iostream>

Observer::~Observer()
{
	if(subject != nullptr)
	{
		subject->removeObserver(this);
	}
}

void Observer::setSubject(Subject *subject)
{
	this->subject = subject;
}

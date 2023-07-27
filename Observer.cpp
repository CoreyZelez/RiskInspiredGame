#include "Observer.h"
#include "Subject.h"
#include <assert.h>

Observer::~Observer()
{
	if(subject != nullptr)
	{
		subject->removeObserver(this);
	}
}

void Observer::setSubject(Subject *subject)
{
	assert(this->subject == nullptr);  // Observers may only observe one object throughout lifetime. (Perhaps change in future).
	this->subject = subject;
}

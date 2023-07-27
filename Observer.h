#pragma once
#include "Message.h"

class Subject;

class Observer
{
public:
	virtual ~Observer();

	virtual void update(Message message) = 0;

	void setSubject(Subject *subject);

private:
	Subject *subject = nullptr;
};
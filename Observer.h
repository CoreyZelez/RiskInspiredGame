#pragma once
#include "Message.h"

class Observer
{
public:
	virtual ~Observer() {}

	virtual void update(Message message) = 0;
};
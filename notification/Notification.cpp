#include "StdAfx.h"
#include "Notification.h"
#include <typeinfo>

Notification::Notification(void)
{
}

Notification::~Notification(void)
{
}

std::string Notification::name() const
{
	return typeid(*this).name();
}
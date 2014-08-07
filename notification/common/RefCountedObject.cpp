#include "StdAfx.h"
#include "RefCountedObject.h"

RefCountedObject::RefCountedObject(void)
:_counter(1)
{
}

RefCountedObject::~RefCountedObject(void)
{
}


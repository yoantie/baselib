#ifndef _NOTIFICATION_H__
#define _NOTIFICATION_H__

#include "Mutex.h"
#include "RefCountedObject.h"
#include "AutoPtr.h"

class Notification : public RefCountedObject
{
public:
	typedef AutoPtr<Notification> Ptr;

	Notification(void);
	
	virtual std::string name() const;

protected:
	virtual	~Notification(void);
};


#endif
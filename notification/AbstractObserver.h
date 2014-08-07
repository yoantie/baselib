#ifndef _ABSTRACTOBSERVER_H__
#define _ABSTRACTOBSERVER_H__

#include "Notification.h"

class AbstractObserver
{
public:
	AbstractObserver(void);
	AbstractObserver(const AbstractObserver& observer);
	virtual ~AbstractObserver();
	
	AbstractObserver& operator = (const AbstractObserver& observer);

	virtual void notify(Notification* pNf) const = 0;
	virtual bool equals(const AbstractObserver& observer) const = 0;
	virtual bool accepts(Notification* pNf) const = 0;
	virtual AbstractObserver* clone() const = 0;
	virtual void disable() = 0;
};


#endif
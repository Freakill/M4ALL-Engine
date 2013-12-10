#include "notifierClass.h"

#include "listenerClass.h"

Notifier::Notifier()
{

}

Notifier::~Notifier()
{

}

bool Notifier::AddListener(Listener* listener)
{
	std::vector<Listener*>::iterator temp = find(listeners_.begin(), listeners_.end(), listener);
	if ( temp != listeners_.end() )
		return false;

	listeners_.push_back(listener);
	return true;
}

bool Notifier::RemoveListener(Listener* listener)
{
	std::vector<Listener*>::iterator temp = find(listeners_.begin(), listeners_.end(), listener);
	if ( temp == listeners_.end() )
		return false;
	else
		listeners_.erase(remove(listeners_.begin(), listeners_.end(), listener));
	return true;
}

bool Notifier::NotifyListeners()
{
	std::vector<Listener*>::iterator temp;
	for(temp = listeners_.begin(); temp != listeners_.end(); temp++)
	{
		(*temp)->Notify(this);
	}

	return (listeners_.size() > 0);
}

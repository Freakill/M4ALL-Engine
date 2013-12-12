#ifndef _NOTIFIER_CLASS_H_
#define _NOTIFIER_CLASS_H_

#include <vector>
#include <algorithm>

#include "listenerClass.h"

template <class T, class A>
class Notifier
{
	public:
		Notifier(){};
		//Notifier(const Notifier<T, A> &);
		virtual ~Notifier(){};

		bool AddListener(Listener<T, A> &listener)
		{
			std::vector<Listener<T, A> *>::const_iterator iterator = std::find(listeners_.begin(), listeners_.end(), &listener);
			if (iterator == listeners_.end()) {
				listeners_.push_back(&listener);
				return true;
			}

			return false;
		};

		bool RemoveListener(Listener<T, A> &listener)
		{
			std::vector<Listener<T,A> *>::const_iterator iterator = find(listeners_.begin(), listeners_.end(), &listener);
			if (iterator == listeners_.end())
				return false;
			else
				listeners_.erase(remove(listeners_.begin(), listeners_.end(), &listener));
			return true;
		};

		bool NotifyListeners(A arg)
		{
			std::vector<Listener<T,A> *>::iterator temp;
			for(temp = listeners_.begin(); temp != listeners_.end(); temp++)
			{
				(*temp)->Notify(static_cast<T *>(this), arg);
			}

			return (listeners_.size() > 0);
		};

	private: 
		std::vector<Listener<T, A> *> listeners_;

};

#endif //_NOTIFIER_CLASS_H_

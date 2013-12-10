#ifndef _NOTIFIER_CLASS_H
#define _NOTIFIER_CLASS_H

#include <vector>
#include <algorithm>

class Listener;

class Notifier
{
	public:
		Notifier();
		Notifier(const Notifier&);
		virtual ~Notifier();		//destructor

		bool AddListener(Listener* listener);
		bool RemoveListener(Listener* listener);
		bool NotifyListeners();

	private: 
		std::vector<Listener*> listeners_;

};

#endif //_NOTIFIER_CLASS_H
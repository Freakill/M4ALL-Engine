#ifndef _LISTENER_CLASS_H_
#define _LISTENER_CLASS_H_

class Notifier;

class Listener
{
	public:
		Listener();
		Listener(const Listener&);
		virtual ~Listener();

		virtual void Notify(Notifier* notifier) = 0;
};

#endif //_LISTENER_CLASS_H_

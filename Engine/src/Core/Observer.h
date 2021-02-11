#pragma once

#include "Events/Event.h"

namespace meow {
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void onNotify(Event event) = 0;
	};

	class Subject
	{
	public:
		void addObserver(Observer* observer);
		void removeObserver(Observer* observer);
	protected:
		void notify(Event event);
	};
}

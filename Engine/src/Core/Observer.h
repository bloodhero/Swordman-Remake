#pragma once


namespace meow {
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void onNotify() = 0;
	};

	class Subject
	{
	public:
		void addObserver(Observer* observer);
		void removeObserver(Observer* observer);
	protected:
		void notify();
	};
}

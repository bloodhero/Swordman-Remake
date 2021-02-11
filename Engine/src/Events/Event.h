#pragma once

#include <functional>
#include <queue>

#include "Debug/Instrumentor.h"
#include "Core/Utils.h"


namespace meow {

	enum class EventType
	{
		None = 0,
		WindowClose,
		WindowResize,
		WindowFocus,
		WindowLostFocus,
		WindowMoved,
		AppTick,
		AppUpdate,
		AppRender,
		KeyPressed,
		KeyReleased,
		KeyTyped,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type; }\
								virtual EventType getEventType() const override { return getStaticType(); }\
								virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

	class Event
	{
	public:
		// CREARTORS
		virtual ~Event() = default;


		// ACCESSORS
		virtual EventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }
		virtual bool operator==(Event& other) const = 0;
		bool isInCategory(EventCategory category) const { return getCategoryFlags() & category; }
		bool isConsumed() const { return  m_Handled; }
		float getTimestap() const { return m_Timestap; }

		// MANIPULATORS
		void setCompareFN(std::function<bool(Event, Event)> cmp) { m_Cmp = cmp; }
		void consume() { m_Handled = true; }

	private:
		bool m_Handled;
		float m_Timestap;
		std::function<bool(Event, Event)> m_Cmp;
	};


	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}

	class EventPackage
	{
	public:
		// CREARTORS
		static EventPackage pollEvent();
		static void pushEvent(Event* e);

		// ACCESSORS
		bool isEmpty() const; // internal has test all event has consumed?
		// int has_pattern(arg, ...);
		EventType getFirstType() const;
		EventPackage filterEvent(EventCategory category);

		// MANIPULATORS
		Event* popEvent();

	private:
		struct Impl;
		Ref<Impl> m_Impl;
		static std::queue<Event*> s_MsgBox;
	};
}


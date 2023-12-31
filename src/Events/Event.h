#pragma once

#include <string>
#include <sstream>
#include <functional>
#include "Events/KeyCodes.h"

enum class EventType
{
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    AppTick, AppUpdate, AppRender,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory {
    None = 0,
    EventCategoryApplication = 1,
    EventCategoryInput = 2,
    EventCategoryKeyboard = 3,
    EventCategoryMouse = 4,
    EventCategoryMouseButton = 5
};

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::##type; }\
                                virtual EventType getEventType() const override { return getStaticType(); }\
                                virtual const char* getName() const override { return #type; }

class Event {
    friend class EventDispatcher;
public:
    bool Handled = false;

    virtual EventType getEventType() const = 0;
    virtual const char* getName() const = 0;
    virtual int getCategoryFlags() const = 0;
    virtual std::string toString() const { return getName(); };

    inline bool isInCategory(EventCategory category) {
        return getCategoryFlags() & category;
    }

protected:
};

class EventDispatcher {
    template <class T>
    using EventFn = std::function<bool(T&)>;

public:
    EventDispatcher(Event& event)
        : m_Event(event)
    {
    }

    template <class T>
    bool dispatch(EventFn<T> func) {
        if (m_Event.getEventType() == T::getStaticType()) {
            m_Event.Handled = func(*(T*)&m_Event);
            return true;
        }
        return false;
    }
private:
    Event& m_Event;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
    return os << e.toString();
}




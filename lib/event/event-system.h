#pragma once
#include <renderer-system.h>
#include <memory>

namespace event {
	// asbract class, subclasses must implement the handleEvent method, that is called when a relevant event is processed
	// the EventHandler can be registered for a certain event type using event::registerHandler
	class EventHandler;

	// defines a pointer to a function that takes a sf::Event parameter and returns void
	typedef void(*eventFunction)(const sf::Event&);

	// register the given EventHandler whose handleEvent method will be called each time an Event of the given type is processed
	// if this the handler is only registered for a single event type, it will only be called for that type
	// however if the same handler is registered for several event types, the handler should check the type of the event that was passed
	void registerHandler(sf::Event::EventType eventType, EventHandler* eventHandler);
	
	// unregister the given EventHandler from all the events of the given type
	// does not unregister the EventHandler from the other types it may have been registered for
	void unregisterHandler(sf::Event::EventType eventType, EventHandler* eventHandler);
	
	// register a function that will be called each time an Event of the given type is processed
	// if this the handler is only registered for a single event type, it will only be called for that type
	// however if the same handler is registered for several event types, the handler should check the type of the event that was passed
	void registerHandler(sf::Event::EventType eventType, eventFunction eventHandler);
	
	// unregister the given function from all the events of the given type
	// does not unregister the function from the other types it may have been registered for
	void unregisterHandler(sf::Event::EventType eventType, eventFunction eventHandler);
	
	// poll all the events from the renderer's window and call all the handlers registered for the event's type
	void update();

	// unregister every registered handler function and EventHandler
	void unregisterAll();
}

class event::EventHandler {
public:
	EventHandler() = default;
	virtual ~EventHandler() = default;

	virtual void handleEvent(const sf::Event& event) = 0;
};


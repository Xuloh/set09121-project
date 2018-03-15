#include <unordered_map>
#include <set>
#include "event-system.h"

using namespace sf;
using namespace std;
using namespace event;

static unordered_map<Event::EventType, set<eventFunction>> eventHandlersFunctions;
static unordered_map<Event::EventType, set<EventHandler*>> eventHandlers;

void event::registerHandler(const Event::EventType eventType, EventHandler* const eventHandler) {
	if (!eventHandlers.count(eventType))
		eventHandlers[eventType] = set<EventHandler*>();
	eventHandlers[eventType].insert(eventHandler);
}

void event::unregisterHandler(const Event::EventType eventType, EventHandler* const eventHandler) {
	if(!eventHandlers.empty() && eventHandlers.count(eventType))
		eventHandlers[eventType].erase(eventHandler);
}

void event::registerHandler(const Event::EventType eventType, const eventFunction eventHandler) {
	if (!eventHandlersFunctions.count(eventType))
		eventHandlersFunctions[eventType] = set<eventFunction>();
	eventHandlersFunctions.at(eventType).insert(eventHandler);
}

void event::unregisterHandler(const Event::EventType eventType, const eventFunction eventHandler) {
	if(!eventHandlersFunctions.empty() && eventHandlersFunctions.count(eventType))
		eventHandlersFunctions[eventType].erase(eventHandler);
}

void event::unregisterAll() {
	eventHandlersFunctions.clear();
	eventHandlers.clear();
}

void event::update() {
	auto& window = renderer::getWindow();

	Event event;
	while (window.pollEvent(event)) {
		if(eventHandlersFunctions.count(event.type))
			for (const auto& handler : eventHandlersFunctions[event.type])
				handler(event);
		if(eventHandlers.count(event.type))
			for (const auto& handler : eventHandlers[event.type])
				handler->handleEvent(event);
	}
}

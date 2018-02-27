#include <unordered_map>
#include <set>
#include "event-system.h"

using namespace sf;
using namespace std;
using namespace event;

static unordered_map<Event::EventType, set<eventFunction>> eventHandlersFunctions;
static unordered_map<Event::EventType, set<EventHandler* const>> eventHandlers;

void event::registerHandler(const Event::EventType eventType, EventHandler* const eventHandler) {
	if (!eventHandlers.count(eventType))
		eventHandlers.insert({ eventType, set<EventHandler* const>() });
	eventHandlers[eventType].insert(eventHandler);
}

void event::unregisterHandler(const Event::EventType eventType, EventHandler* const eventHandler) {
	eventHandlers[eventType].erase(eventHandler);
}

void event::registerHandler(const Event::EventType eventType, const eventFunction eventHandler) {
	if (!eventHandlersFunctions.count(eventType))
		eventHandlersFunctions.insert({ eventType, set<eventFunction>() });
	eventHandlersFunctions[eventType].insert(eventHandler);
}

void event::unregisterHandler(const Event::EventType eventType, const eventFunction eventHandler) {
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
		for (auto& handler : eventHandlersFunctions[event.type])
			handler(event);
		for (auto& handler : eventHandlers[event.type])
			handler->handleEvent(event);
	}
}

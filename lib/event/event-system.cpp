#include <unordered_map>
#include <set>
#include "event-system.h"

using namespace sf;
using namespace std;
using namespace event;

static unordered_map<Event::EventType, set<void(*)(Event)>> eventHandlersFunctions;
static unordered_map<Event::EventType, set<shared_ptr<EventHandler>>> eventHandlers;

void event::registerHandler(const Event::EventType eventType, const shared_ptr<EventHandler>& eventHandler) {
	if (!eventHandlers.count(eventType))
		eventHandlers.insert({ eventType, set<shared_ptr<EventHandler>>() });
	eventHandlers[eventType].insert(eventHandler);
}

void event::unregisterHandler(const Event::EventType eventType, const shared_ptr<EventHandler>& eventHandler) {
	eventHandlers[eventType].erase(eventHandler);
}

void event::registerHandler(const Event::EventType eventType, void(*eventHandler)(Event)) {
	if (!eventHandlersFunctions.count(eventType))
		eventHandlersFunctions.insert({ eventType, set<void(*)(Event)>() });
	eventHandlersFunctions[eventType].insert(eventHandler);
}

void event::unregisterHandler(const Event::EventType eventType, void(*eventHandler)(Event)) {
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

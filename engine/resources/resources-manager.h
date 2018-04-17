#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>

namespace resources {
    // generic resource loader (throws an exception)
    template<typename T> std::shared_ptr<T> load(const std::string& name) {
        throw std::string("No resource loader defined for resource " + name + " of type " + typeid(T).name());
    }

    // Font resource loader
    template<> std::shared_ptr<sf::Font> load(const std::string& name);
    
    // Texture resource loader
    template<> std::shared_ptr<sf::Texture> load(const std::string& name);

    // SoundBuffer resource loader
    template<> std::shared_ptr<sf::SoundBuffer> load(const std::string& name);

    // gets the resource of the given name the given type
    // all the resources loaded with this function are handled by the resources manager
    template<typename T> std::shared_ptr<T> get(const std::string& name) {
        static std::unordered_map<std::string, std::shared_ptr<T>> resources;
        auto it = resources.find(name);
        if(it != resources.end()) {
            return it->second;
        }
        const std::shared_ptr<T> resource = load<T>(name);
        resources[name] = resource;
        return resource;
    }
}

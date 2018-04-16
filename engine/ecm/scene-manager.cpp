#include "scene-manager.h"
#include <unordered_map>
#include <iostream>

using namespace std;
using namespace ecm;

static unordered_map<string, shared_ptr<Scene>> scenes;
static shared_ptr<Scene> activeScene;
static string nextScene;
static string currentScene;

void scene::add(const std::string& name, const std::shared_ptr<ecm::Scene>& scene) {
    if (!scenes.count(name))
        scenes[name] = scene;
    else
        throw string("A scene already exists for the name : " + name);
}

void scene::load(const std::string& name) {
    // the actual scene loading is not done here
    // this is to prevent a bug that was caused when loading a scene from an event handler
    // that would cause the current scene to be unloaded and all its event handlers to be unregistered
    // which would mess up the event handler iterators in the event handling system
    if (scenes.count(name))
        nextScene = name;
    else
        throw string("There is no scene with the name : " + name);
}

const std::string& scene::getCurrentScene() {
    return currentScene;
}

void scene::update(const double dt) {
    if(!nextScene.empty()) {
        if (activeScene) {
            cout << "unloading previous scene" << endl;
            activeScene->unload();
        }
        activeScene = scenes[nextScene];
        activeScene->load();
        currentScene = nextScene;
        nextScene = "";
    }
    activeScene->update(dt);
}

void scene::render() {
    activeScene->render();
}

void scene::shutdown() {
    activeScene.reset();
    scenes.clear();
}
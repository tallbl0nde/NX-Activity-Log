#include "screenmanager.hpp"

ScreenManager * ScreenManager::instance = nullptr;

ScreenManager::ScreenManager() {
    this->changed = false;
    this->looping = true;
    this->screen_ptr = nullptr;
}

ScreenManager * ScreenManager::getInstance() {
    // Create instance if it doesn't exist
    if (!ScreenManager::instance) {
        ScreenManager::instance = new ScreenManager();
    }
    return ScreenManager::instance;
}

void ScreenManager::setScreen(UI::Screen * s) {
    if (this->screen_ptr != nullptr) {
        delete this->screen_ptr;
    }
    this->screen_ptr = s;
    this->changed = true;
}

void ScreenManager::pushScreen() {
    this->stack.push(this->screen_ptr);
    this->screen_ptr = nullptr;
}

void ScreenManager::popScreen() {
    if (this->screen_ptr != nullptr) {
        delete this->screen_ptr;
        this->screen_ptr = nullptr;
    }
    if (this->stack.size() > 0) {
        this->screen_ptr = this->stack.top();
        this->stack.pop();
    }
}

void ScreenManager::event() {
    if (this->screen_ptr != nullptr && !this->changed) {
        this->screen_ptr->event();
    }
}

void ScreenManager::update(uint32_t dt) {
    if (this->screen_ptr != nullptr && !this->changed) {
        this->screen_ptr->update(dt);
    }
}

void ScreenManager::draw() {
    if (this->screen_ptr != nullptr && !this->changed) {
        this->screen_ptr->draw();
    }
    this->changed = false;
}

bool ScreenManager::loop() {
    return this->looping;
}

void ScreenManager::stopLoop() {
    this->looping = false;
}

void ScreenManager::free() {
    do {
        this->popScreen();
    } while (this->screen_ptr != nullptr);
}
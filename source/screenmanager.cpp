#include "screenmanager.hpp"
#include "utils.hpp"

ScreenManager * ScreenManager::instance = nullptr;

ScreenManager::ScreenManager() {
    this->changed = false;
    this->looping = true;
    this->screen_ptr = nullptr;
    this->selection = nullptr;
    this->selection_active = false;
    this->selection_callback = nullptr;
    this->touch_active = true;
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

void ScreenManager::createSelection(std::string t, std::vector<std::string> v, int ti, std::function<void(int)> f) {
    if (this->selection != nullptr) {
        delete this->selection;
    }
    this->selection = new UI::Selection(&this->touch_active, t, v, ti);
    this->selection_active = true;
    this->selection_callback = f;
}

void ScreenManager::event() {
    // Parse events and pass to screen (and selection panel if active)
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            // Button pressed/released
            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
                // Joysticks push appropriate button event
                if (e.jbutton.button >= Utils::key_map[KEY_LSTICK_LEFT] && e.jbutton.button <= Utils::key_map[KEY_RSTICK_DOWN]) {
                    SDL_Event event;
                    event.type = e.type;
                    event.jbutton.which = 0;
                    event.jbutton.state = e.jbutton.state;
                    if (e.jbutton.button == Utils::key_map[KEY_LSTICK_LEFT] || e.jbutton.button == Utils::key_map[KEY_RSTICK_LEFT]) {
                        event.jbutton.button = Utils::key_map[KEY_DLEFT];
                    } else if (e.jbutton.button == Utils::key_map[KEY_LSTICK_RIGHT] || e.jbutton.button == Utils::key_map[KEY_RSTICK_RIGHT]) {
                        event.jbutton.button = Utils::key_map[KEY_DRIGHT];
                    } else if (e.jbutton.button == Utils::key_map[KEY_LSTICK_UP] || e.jbutton.button == Utils::key_map[KEY_RSTICK_UP]) {
                        event.jbutton.button = Utils::key_map[KEY_DUP];
                    } else if (e.jbutton.button == Utils::key_map[KEY_LSTICK_DOWN] || e.jbutton.button == Utils::key_map[KEY_RSTICK_DOWN]) {
                        event.jbutton.button = Utils::key_map[KEY_DDOWN];
                    }
                    SDL_PushEvent(&event);
                    break;
                }

                // Break on first press (ie. only activate highlighting)
                if (this->touch_active && e.jbutton.which != CONTROLS_ID) {
                    this->touch_active = false;
                }

                // Pass events to active screen (and selection)
                if (this->selection_active) {
                    this->selection->event(e);
                    break;
                }

                if (this->screen_ptr != nullptr && !this->changed) {
                    this->screen_ptr->event(e);
                }
                break;

            case SDL_FINGERDOWN:
            case SDL_FINGERMOTION:
            case SDL_FINGERUP:
                this->touch_active = true;

                // Pass events to active screen (and selection)
                if (this->selection_active) {
                    this->selection->event(e);
                    break;
                }

                if (this->screen_ptr != nullptr && !this->changed) {
                    this->screen_ptr->event(e);
                }
                break;
        }
    }
}

void ScreenManager::update(uint32_t dt) {
    if (this->selection_active) {
        this->selection->update(dt);
        if (this->selection->closed()) {
            // Call callback function on close
            this->selection_callback(this->selection->getChosen());
            // Delete selection object
            delete this->selection;
            this->selection = nullptr;
            this->selection_active = false;
        }
    }

    if (this->screen_ptr != nullptr && !this->changed) {
        this->screen_ptr->update(dt);
    }
}

void ScreenManager::draw() {
    if (this->screen_ptr != nullptr && !this->changed) {
        this->screen_ptr->draw();
    }

    if (this->selection_active) {
        SDLHelper::setColour(SDL_Color{0, 0, 0, 150});
        SDLHelper::drawRect(0, 0, WIDTH, HEIGHT);
        this->selection->draw();
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

    // Delete selection
    if (this->selection != nullptr) {
        delete this->selection;
    }
}
#include "listitem_user.hpp"
#include "screenmanager.hpp"
#include "SDLHelper.hpp"
#include "utils.hpp"

namespace Screen {
    UserSelect::UserSelect(std::vector<User *> v, bool b) : Screen::Screen() {
        this->users = v;
        this->is_mypage = b;

        // Set fade value if not user page
        if (!is_mypage) {
            this->fade = 255;
        } else {
            this->fade = 1;
        }

        // Create list
        this->list = new UI::List(&this->touch_active, WIDTH/2 - 300, 130, 600, 480);
        for (size_t i = 0; i < this->users.size(); i++) {
            this->list->addItem(new UI::ListItem::User(this->users[i]->getImage(), this->users[i]->getUsername()));
        }
        this->list->setActive(true);

        // Add controls
        this->controls->add(KEY_A, "OK", 0);
        this->controls->add(KEY_PLUS, "Exit", 1);
    }

    void UserSelect::event() {
        // Do nothing until fade is complete
        if (this->fade > 0) {
            return;
        }

        // Poll events
        SDL_Event events;
        while (SDL_PollEvent(&events)) {
            switch (events.type) {
                // Button pressed
                case SDL_JOYBUTTONDOWN:
                    // Break on first press (ie. only active highlighting)
                    if (this->touch_active && events.jbutton.which != 99) {
                        if (!(events.jbutton.button >= Utils::key_map[KEY_LSTICK_LEFT] && events.jbutton.button <= Utils::key_map[KEY_RSTICK_DOWN])) {
                            this->touch_active = false;
                        }
                        if (events.jbutton.button >= Utils::key_map[KEY_DLEFT] && events.jbutton.button <= Utils::key_map[KEY_DDOWN] && this->active_element != (int)ActiveElement::List) {
                            break;
                        }
                    }

                    if (events.jbutton.which == 0 || events.jbutton.which == 99) {
                        // Joysticks push appropriate button event
                        if (events.jbutton.button >= Utils::key_map[KEY_LSTICK_LEFT] && events.jbutton.button <= Utils::key_map[KEY_RSTICK_DOWN]) {
                            SDL_Event event;
                            event.type = SDL_JOYBUTTONDOWN;
                            event.jbutton.which = 0;
                            event.jbutton.state = SDL_PRESSED;
                            if (events.jbutton.button == Utils::key_map[KEY_LSTICK_LEFT] || events.jbutton.button == Utils::key_map[KEY_RSTICK_LEFT]) {
                                event.jbutton.button = Utils::key_map[KEY_DLEFT];
                            } else if (events.jbutton.button == Utils::key_map[KEY_LSTICK_RIGHT] || events.jbutton.button == Utils::key_map[KEY_RSTICK_RIGHT]) {
                                event.jbutton.button = Utils::key_map[KEY_DRIGHT];
                            } else if (events.jbutton.button == Utils::key_map[KEY_LSTICK_UP] || events.jbutton.button == Utils::key_map[KEY_RSTICK_UP]) {
                                event.jbutton.button = Utils::key_map[KEY_DUP];
                            } else if (events.jbutton.button == Utils::key_map[KEY_LSTICK_DOWN] || events.jbutton.button == Utils::key_map[KEY_RSTICK_DOWN]) {
                                event.jbutton.button = Utils::key_map[KEY_DDOWN];
                            }
                            SDL_PushEvent(&event);

                        // Plus exits app
                        } else if (events.jbutton.button == Utils::key_map[KEY_PLUS] || events.jbutton.button == Utils::key_map[KEY_B]) {
                            ScreenManager::getInstance()->stopLoop();

                        // All other buttons get handled by list
                        } else {
                            this->list->handleButton(events.jbutton.button, events.jbutton.state);
                        }
                    }
                    break;

                case SDL_JOYBUTTONUP:
                    if (events.jbutton.which == 0 || events.jbutton.which == 99) {
                        // Joysticks push appropriate button event
                        if (events.jbutton.button >= Utils::key_map[KEY_LSTICK_LEFT] && events.jbutton.button <= Utils::key_map[KEY_RSTICK_DOWN]) {
                            SDL_Event event;
                            event.type = SDL_JOYBUTTONUP;
                            event.jbutton.which = 0;
                            event.jbutton.state = SDL_RELEASED;
                            if (events.jbutton.button == Utils::key_map[KEY_LSTICK_LEFT] || events.jbutton.button == Utils::key_map[KEY_RSTICK_LEFT]) {
                                event.jbutton.button = Utils::key_map[KEY_DLEFT];
                            } else if (events.jbutton.button == Utils::key_map[KEY_LSTICK_RIGHT] || events.jbutton.button == Utils::key_map[KEY_RSTICK_RIGHT]) {
                                event.jbutton.button = Utils::key_map[KEY_DRIGHT];
                            } else if (events.jbutton.button == Utils::key_map[KEY_LSTICK_UP] || events.jbutton.button == Utils::key_map[KEY_RSTICK_UP]) {
                                event.jbutton.button = Utils::key_map[KEY_DUP];
                            } else if (events.jbutton.button == Utils::key_map[KEY_LSTICK_DOWN] || events.jbutton.button == Utils::key_map[KEY_RSTICK_DOWN]) {
                                event.jbutton.button = Utils::key_map[KEY_DDOWN];
                            }
                            SDL_PushEvent(&event);

                        // All other buttons get handled by list
                        } else {
                            this->list->handleButton(events.jbutton.button, events.jbutton.state);
                        }
                    }
                    break;

                // Touch (pressed)
                case SDL_FINGERDOWN: {
                    this->touch_active = true;
                    float x = WIDTH * events.tfinger.x;
                    float y = HEIGHT * events.tfinger.y;

                    // Pressed within list
                    if (x >= this->list->getX() && x <= this->list->getX() + this->list->getW() && y >= this->list->getY() && y <= this->list->getY() + this->list->getH()) {
                        this->list->touched(events.type, x, y);

                    // Pass event to controls object if below bottom line
                    } else if (y > 647) {
                        this->controls->touched(events.type, x, y);
                    }
                    break;
                }

                // Touch (moved)
                case SDL_FINGERMOTION: {
                    float x = WIDTH * events.tfinger.x;
                    float y = HEIGHT * events.tfinger.y;
                    float dx = WIDTH * events.tfinger.dx;
                    float dy = HEIGHT * events.tfinger.dy;

                    // List scrolling overrides any other actions
                    if (this->list->isTouched()) {
                        this->list->touched(events.type, x, y, dx, dy);

                    // Pass event to controls object if was below or originally below line
                    } else if (y > 647 || (HEIGHT * (events.tfinger.y - events.tfinger.dy)) > 647) {
                        this->controls->touched(events.type, x, y);
                    }
                    break;
                }

                // Touch (released)
                case SDL_FINGERUP: {
                    float x = WIDTH * events.tfinger.x;
                    float y = HEIGHT * events.tfinger.y;

                    if (this->list->isTouched()) {
                        this->list->touched(events.type, x, y, (WIDTH * events.tfinger.dx), (HEIGHT * events.tfinger.dy));

                    // Pass event to controls object if below bottom line
                    } else if (y > 647) {
                        this->controls->touched(events.type, x, y);
                    }
                    break;
                }
            }
        }
    }

    void UserSelect::update(uint32_t dt) {
        Screen::update(dt);

        // Fade the fade ;)
        if (this->fade > 0) {
            this->fade -= 600*(dt/1000.0);
            if (this->fade <= 0) {
                this->fade = 0;
            }
            return;
        }

        // If launched as mypage proceed straight to stats
        if (this->is_mypage) {
             ScreenManager::getInstance()->setScreen(new AllActivity(this->users[0]));
             return;
        }

        // Only executed after fade is finished
        this->list->update(dt);

        // Change to loading screen with chosen user and titles
        if (this->list->getChosen() != -1) {
            ScreenManager::getInstance()->setScreen(new AllActivity(this->users[this->list->getChosen()]));
        }
    }

    void UserSelect::draw() {
        // Clear screen (draw background)
        SDLHelper::setColour(this->theme->getBG());
        SDLHelper::clearScreen();

        // Draw top and bottom lines
        SDLHelper::setColour(this->theme->getFG());
        SDLHelper::drawRect(30, 87, 1220, 1);
        SDLHelper::drawRect(30, 647, 1220, 1);

        if (!this->is_mypage) {
            // Print UserSelect title
            SDLHelper::drawText("Select a User", this->theme->getText(), 65, 44 - (HEADING_FONT_SIZE/2), HEADING_FONT_SIZE);

            // Draw list of items
            this->list->draw();

            // Draw over list to hide scrolling
            SDLHelper::setColour(this->theme->getBG());
            SDLHelper::drawRect(WIDTH/2 - 305, 0, 610, 87);
            SDLHelper::drawRect(WIDTH/2 - 305, 648, 610, 72);

            // Draw controls
            this->controls->draw();
        } else {
            SDLHelper::drawText("Loading...", this->theme->getText(), 65, 44 - (HEADING_FONT_SIZE/2), HEADING_FONT_SIZE);
        }

        // Draw fading overlay
        if (this->fade > 0) {
            SDLHelper::setColour(SDL_Color{0, 0, 0, (uint8_t)this->fade});
            SDLHelper::drawRect(0, 0, WIDTH, HEIGHT);
        }
    }

    void UserSelect::setTheme(bool b) {
        if (b) {
            this->theme->setDark();
        }
    }

    UserSelect::~UserSelect() {
        delete this->list;
    }
}
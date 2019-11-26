#include "screenmanager.hpp"
#include "SDLHelper.hpp"
#include "utils.hpp"

// Gap between username and icon
#define ICON_GAP 20
// Size of game icon
#define ICON_SIZE 60
// Font sizes
#define NAME_FONT_SIZE 18
#define STAT_FONT_SIZE 22

namespace Screen {
    Details::Details(User * u, Title * t) : Screen() {
        // Store objects
        this->user = u;
        this->title = t;

        // Generate textures
        std::string str = "";
        this->first_played = SDLHelper::renderText(Utils::formatLastPlayed(t->getFirstPlayed()).c_str(), STAT_FONT_SIZE);
        this->last_played = SDLHelper::renderText(Utils::formatLastPlayed(t->getLastPlayed()).c_str(), STAT_FONT_SIZE);
        str = "Launched: " + std::to_string(t->getLaunches()) + " times";
        this->launches = SDLHelper::renderText(str.c_str(), STAT_FONT_SIZE);
        str = "Played for: " + Utils::formatPlaytime(t->getPlaytime());
        this->playtime = SDLHelper::renderText(str.c_str(), STAT_FONT_SIZE);

        this->name = SDLHelper::renderText(this->title->getName().c_str(), HEADING_FONT_SIZE);
        this->username = SDLHelper::renderText(this->user->getUsername().c_str(), NAME_FONT_SIZE);

        // Set controls
        this->controls->add(KEY_B, "Back", 0);
        this->controls->add(KEY_PLUS, "Exit", 1);
        this->controls->add(KEY_R, "Next", 2);
        this->controls->add(KEY_L, "Previous", 3);
    }

    void Details::event() {
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
                        } else if (events.jbutton.button == Utils::key_map[KEY_PLUS]) {
                            ScreenManager::getInstance()->stopLoop();

                        // B returns to previous screen
                        } else if (events.jbutton.button == Utils::key_map[KEY_B]) {
                            ScreenManager::getInstance()->popScreen();
                        }
                    }
                    break;

                case SDL_JOYBUTTONUP:
                    if (events.jbutton.which == 0) {
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
                        }
                    }
                    break;

                // Touch (pressed)
                case SDL_FINGERDOWN: {
                    this->touch_active = true;
                    float x = WIDTH * events.tfinger.x;
                    float y = HEIGHT * events.tfinger.y;

                    // Pass event to controls object if below bottom line
                    if (y > 647) {
                        this->controls->touched(events.type, x, y);
                    }
                    break;
                }

                // Touch (moved)
                case SDL_FINGERMOTION: {
                    float x = WIDTH * events.tfinger.x;
                    float y = HEIGHT * events.tfinger.y;

                    // Pass event to controls object if was below or originally below line
                    if (y > 647 || (HEIGHT * (events.tfinger.y - events.tfinger.dy)) > 647) {
                        this->controls->touched(events.type, x, y);
                    }
                    break;
                }

                // Touch (released)
                case SDL_FINGERUP: {
                    float x = WIDTH * events.tfinger.x;
                    float y = HEIGHT * events.tfinger.y;

                    // Pass event to controls object if below bottom line
                    if (y > 647) {
                        this->controls->touched(events.type, x, y);
                    }
                    break;
                }
            }
        }
    }

    void Details::update(uint32_t dt) {
        Screen::update(dt);
    }

    void Details::draw() {
        // Clear screen (draw background)
        SDLHelper::setColour(this->theme->getBG());
        SDLHelper::clearScreen();

        // Different shade on right pane
        SDLHelper::setColour(this->theme->getAltBG());
        SDLHelper::drawRect(500, 88, 750, 560);

        // Draw top and bottom lines
        SDLHelper::setColour(this->theme->getFG());
        SDLHelper::drawRect(30, 87, 1220, 1);
        SDLHelper::drawRect(30, 647, 1220, 1);

        // Print heading and icon
        int tw, tw2, th;
        SDLHelper::drawTexture(this->title->getIcon(), 65, 44 - (ICON_SIZE/2), ICON_SIZE, ICON_SIZE);
        SDLHelper::getDimensions(this->name, &tw, &th);
        SDLHelper::drawTexture(this->name, this->theme->getText(), 65 + ICON_SIZE + ICON_GAP, 44 - (th/2));

        // Draw player icon and name
        SDLHelper::getDimensions(this->user->getImage(), &tw, &th);
        SDLHelper::drawTexture(this->user->getImage(), WIDTH - 65 - tw, 44 - (th/2), ICON_SIZE, ICON_SIZE);
        SDLHelper::getDimensions(this->username, &tw2, &th);
        SDLHelper::drawTexture(this->username, this->theme->getMutedText(), WIDTH - 150 - tw - ICON_GAP - tw2, 44 - (th/2));

        // Draw statistics
        SDLHelper::drawTexture(this->first_played, this->theme->getText(), 50, 200);
        SDLHelper::drawTexture(this->last_played, this->theme->getText(), 50, 300);
        SDLHelper::drawTexture(this->launches, this->theme->getText(), 50, 400);
        SDLHelper::drawTexture(this->playtime, this->theme->getText(), 50, 500);

        // Draw controls
        this->controls->draw();
    }

    Details::~Details() {
        SDLHelper::destroyTexture(this->username);
    }
};
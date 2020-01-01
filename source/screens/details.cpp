#include "list.hpp"
#include "listitem_heading.hpp"
#include "listitem_separator.hpp"
#include "listitem_session.hpp"
#include "listitem_statistic.hpp"
#include "PlayData.hpp"
#include "screenmanager.hpp"
#include "SDLHelper.hpp"
#include "TimeHelper.hpp"
#include "utils.hpp"

// Gap between username and icon
#define ICON_GAP 20
// Size of game icon
#define ICON_SIZE 60
// Font sizes
#define NAME_FONT_SIZE 18

namespace Screen {
    Details::Details(User * u, Title * t) : Screen() {
        // Set controls
        this->controls->add(KEY_B, "Back", 0);

        // Create list using values stored in title
        this->list = new UI::List(&ScreenManager::getInstance()->touch_active, 40, 120, 570, 500);
        this->list->setActive(true);
        this->list->addItem(new UI::ListItem::Heading("Summary"));
        this->list->addItem(new UI::ListItem::Separator(20));
        this->list->addItem(new UI::ListItem::Statistic("Play Time", TimeH::playtimeToString(t->getPlaytime() * 60, ", ")));
        this->list->addItem(new UI::ListItem::Statistic("Times Played", Utils::formatNumberComma(t->getLaunches())));
        this->list->addItem(new UI::ListItem::Statistic("Avg. Play Time", TimeH::playtimeToString((t->getPlaytime() * 60) / t->getLaunches(), ", ")));
        this->list->addItem(new UI::ListItem::Statistic("First Played", TimeH::timestampToString(pdmPlayTimestampToPosix(t->getFirstPlayed()))));
        this->list->addItem(new UI::ListItem::Statistic("Last Played", TimeH::timestampToString(pdmPlayTimestampToPosix(t->getLastPlayed()))));
        this->list->addItem(new UI::ListItem::Separator(50));
        this->list->addItem(new UI::ListItem::Heading("Play Sessions", [](){
            std::vector<std::string> vv;
            vv.push_back("Test 1");
            ScreenManager::getInstance()->createSelection("Test", vv, -1, [](int c){

            });
        }));
        this->list->addItem(new UI::ListItem::Separator(20));

        // Add play sessions to list
        PlayData * pd = new PlayData();
        std::vector<PlaySession> sessions = pd->getPlaySessionsForUser(t->getTitleID(), u->getID());
        for (int i = sessions.size() - 1; i >= 0; i--) {
            this->list->addItem(new UI::ListItem::Session(sessions[i].playtime, t->getPlaytime() * 60, sessions[i].startTimestamp, sessions[i].endTimestamp));
        }
        delete pd;

        // Generate textures
        this->icon = t->getIcon();
        this->name = SDLHelper::renderText(t->getName().c_str(), HEADING_FONT_SIZE);
        this->userimage = u->getImage();
        this->username = SDLHelper::renderText(u->getUsername().c_str(), NAME_FONT_SIZE);
    }

    void Details::event(SDL_Event e) {
        switch (e.type) {
            // Button pressed
            case SDL_JOYBUTTONDOWN:
                if (e.jbutton.which == 0 || e.jbutton.which == CONTROLS_ID) {
                    // B returns to previous screen
                    if (e.jbutton.button == Utils::key_map[KEY_B]) {
                        ScreenManager::getInstance()->popScreen();

                    } else {
                        this->list->handleButton(e.jbutton.button, e.jbutton.state);
                    }
                }
                break;

            case SDL_JOYBUTTONUP:
                this->list->handleButton(e.jbutton.button, e.jbutton.state);
                break;

            // Touch (pressed)
            case SDL_FINGERDOWN: {
                float x = WIDTH * e.tfinger.x;
                float y = HEIGHT * e.tfinger.y;

                // Pass event to controls object if below bottom line
                if (y > 647) {
                    this->controls->touched(e.type, x, y);
                } else if (x >= this->list->getX() && x <= this->list->getX() + this->list->getW() && y >= this->list->getY() && y <= this->list->getY() + this->list->getH()) {
                    this->list->touched(e.type, x, y);
                }
                break;
            }

            // Touch (moved)
            case SDL_FINGERMOTION: {
                float x = WIDTH * e.tfinger.x;
                float y = HEIGHT * e.tfinger.y;

                // List takes priority
                if (this->list->isTouched()) {
                    this->list->touched(e.type, x, y, (WIDTH * e.tfinger.dx), (HEIGHT * e.tfinger.dy));

                // Pass event to controls object if was below or originally below line
                } else if (y > 647 || (HEIGHT * (e.tfinger.y - e.tfinger.dy)) > 647) {
                    this->controls->touched(e.type, x, y);
                }
                break;
            }

            // Touch (released)
            case SDL_FINGERUP: {
                float x = WIDTH * e.tfinger.x;
                float y = HEIGHT * e.tfinger.y;

                // List takes priority
                if (this->list->isTouched()) {
                    this->list->touched(e.type, x, y, (WIDTH * e.tfinger.dx), (HEIGHT * e.tfinger.dy));

                // Pass event to controls object if below bottom line
                } else if (y > 647) {
                    this->controls->touched(e.type, x, y);
                }
                break;
            }
        }
    }

    void Details::update(uint32_t dt) {
        Screen::update(dt);
        this->list->update(dt);
    }

    void Details::draw() {
        // Clear screen (draw background)
        SDLHelper::setColour(this->theme->getBG());
        SDLHelper::clearScreen();

        // Different shade on left pane
        SDLHelper::setColour(this->theme->getAltBG());
        SDLHelper::drawRect(30, 88, 600, 560);

        // Draw list
        this->list->draw();

        // Draw top and bottom lines
        SDLHelper::setColour(this->theme->getFG());
        SDLHelper::drawRect(30, 87, 1220, 1);
        SDLHelper::drawRect(30, 647, 1220, 1);

        // Draw over list to hide scrolling
        SDLHelper::setColour(this->theme->getBG());
        SDLHelper::drawRect(40, 0, 580, 87);
        SDLHelper::drawRect(40, 648, 580, 72);

        // Print heading and icon
        int tw, th;
        SDLHelper::drawTexture(this->icon, 65, 44 - (ICON_SIZE/2), ICON_SIZE, ICON_SIZE);
        SDLHelper::getDimensions(this->name, &tw, &th);
        SDLHelper::drawTexture(this->name, this->theme->getText(), 65 + ICON_SIZE + ICON_GAP, 44 - (th/2));

        // Draw player icon and name
        SDLHelper::drawTexture(this->userimage, WIDTH - 65 - ICON_SIZE, 44 - (ICON_SIZE/2), ICON_SIZE, ICON_SIZE);
        SDLHelper::getDimensions(this->username, &tw, &th);
        SDLHelper::drawTexture(this->username, this->theme->getMutedText(), WIDTH - 65 - ICON_SIZE - ICON_GAP - tw, 44 - (th/2));

        // Draw controls
        this->controls->draw();
    }

    Details::~Details() {
        SDLHelper::destroyTexture(this->username);
        SDLHelper::destroyTexture(this->name);
        delete this->list;
    }
};
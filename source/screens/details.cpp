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
// Pixels to indent text
#define INDENT 16
// Position of stats
#define XPOS 50
#define YPOS 160
#define WID 500
#define HEI 70

namespace Screen {
    Details::Details(User * u, std::vector<Title *> v, unsigned int t) : Screen() {
        // Store objects
        this->user = u;
        this->titles = v;
        this->title = t;

        // Set controls
        this->controls->add(KEY_B, "Back", 0);
        this->controls->add(KEY_R, "Next", 1);
        this->controls->add(KEY_L, "Previous", 2);

        // Generate textures
        this->av_heading = SDLHelper::renderText("Average Play Time", STAT_FONT_SIZE);
        this->fp_heading = SDLHelper::renderText("First Played", STAT_FONT_SIZE);
        this->lp_heading = SDLHelper::renderText("Last Played", STAT_FONT_SIZE);
        this->l_heading = SDLHelper::renderText("Times Played", STAT_FONT_SIZE);
        this->p_heading = SDLHelper::renderText("Play Time", STAT_FONT_SIZE);
        this->average_time = nullptr;
        this->first_played = nullptr;
        this->last_played = nullptr;
        this->launches = nullptr;
        this->playtime = nullptr;
        this->index = nullptr;
        this->name = nullptr;
        this->renderTextures();

        this->username = SDLHelper::renderText(this->user->getUsername().c_str(), NAME_FONT_SIZE);
    }

    void Details::renderTextures() {
        if (this->average_time != nullptr) {
            SDLHelper::destroyTexture(this->average_time);
        }
        this->average_time = SDLHelper::renderText(Utils::formatPlaytime(this->titles[this->title]->getPlaytime() / this->titles[this->title]->getLaunches(), ", ").c_str(), STAT_FONT_SIZE);

        if (this->first_played != nullptr) {
            SDLHelper::destroyTexture(this->first_played);
        }
        this->first_played = SDLHelper::renderText(Utils::formatTimestamp(this->titles[this->title]->getFirstPlayed()).c_str(), STAT_FONT_SIZE);

        if (this->last_played != nullptr) {
            SDLHelper::destroyTexture(this->last_played);
        }
        this->last_played = SDLHelper::renderText(Utils::formatTimestamp(this->titles[this->title]->getLastPlayed()).c_str(), STAT_FONT_SIZE);

        if (this->launches != nullptr) {
            SDLHelper::destroyTexture(this->launches);
        }
        this->launches = SDLHelper::renderText(Utils::formatNumberComma(this->titles[this->title]->getLaunches()).c_str(), STAT_FONT_SIZE);

        if (this->playtime != nullptr) {
            SDLHelper::destroyTexture(this->playtime);
        }
        this->playtime = SDLHelper::renderText(Utils::formatPlaytime(this->titles[this->title]->getPlaytime(), ", ").c_str(), STAT_FONT_SIZE);

        if (this->index != nullptr) {
            SDLHelper::destroyTexture(this->index);
        }
        std::string str = std::to_string(this->title + 1) + " of " + std::to_string(this->titles.size());
        this->index = SDLHelper::renderText(str.c_str(), NAME_FONT_SIZE);

        if (this->name != nullptr) {
            SDLHelper::destroyTexture(this->name);
        }
        this->name = SDLHelper::renderText(this->titles[this->title]->getName().c_str(), HEADING_FONT_SIZE);

        // Enable/disable controls (not rendering but best spot)
        if (this->title == 0) {
            this->controls->disable(KEY_L);
        } else if (this->title == this->titles.size() - 1) {
            this->controls->disable(KEY_R);
        } else {
            this->controls->enable(KEY_L);
            this->controls->enable(KEY_R);
        }
    }

    void Details::event(SDL_Event e) {
        switch (e.type) {
            // Button pressed
            case SDL_JOYBUTTONDOWN:
                if (e.jbutton.which == 0 || e.jbutton.which == CONTROLS_ID) {
                    // B returns to previous screen
                    if (e.jbutton.button == Utils::key_map[KEY_B]) {
                        ScreenManager::getInstance()->popScreen();

                    // L moves "up" a title
                    } else if (e.jbutton.button == Utils::key_map[KEY_L]) {
                        if (this->title > 0) {
                            this->title--;
                        }
                        this->renderTextures();

                    // R moves "down" a title
                    } else if (e.jbutton.button == Utils::key_map[KEY_R]) {
                        if (this->title < this->titles.size() - 1) {
                            this->title++;
                        }
                        this->renderTextures();
                    }
                }
                break;

            // Touch (pressed)
            case SDL_FINGERDOWN: {
                float x = WIDTH * e.tfinger.x;
                float y = HEIGHT * e.tfinger.y;

                // Pass event to controls object if below bottom line
                if (y > 647) {
                    this->controls->touched(e.type, x, y);
                }
                break;
            }

            // Touch (moved)
            case SDL_FINGERMOTION: {
                float x = WIDTH * e.tfinger.x;
                float y = HEIGHT * e.tfinger.y;

                // Pass event to controls object if was below or originally below line
                if (y > 647 || (HEIGHT * (e.tfinger.y - e.tfinger.dy)) > 647) {
                    this->controls->touched(e.type, x, y);
                }
                break;
            }

            // Touch (released)
            case SDL_FINGERUP: {
                float x = WIDTH * e.tfinger.x;
                float y = HEIGHT * e.tfinger.y;

                // Pass event to controls object if below bottom line
                if (y > 647) {
                    this->controls->touched(e.type, x, y);
                }
                break;
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
        SDLHelper::drawRect(600, 88, 650, 560);

        // Draw top and bottom lines
        SDLHelper::setColour(this->theme->getFG());
        SDLHelper::drawRect(30, 87, 1220, 1);
        SDLHelper::drawRect(30, 647, 1220, 1);

        // Print heading and icon
        int tw, th;
        SDLHelper::drawTexture(this->titles[this->title]->getIcon(), 65, 44 - (ICON_SIZE/2), ICON_SIZE, ICON_SIZE);
        SDLHelper::getDimensions(this->name, &tw, &th);
        SDLHelper::drawTexture(this->name, this->theme->getText(), 65 + ICON_SIZE + ICON_GAP, 44 - (th/2));

        // Draw player icon and name
        SDLHelper::drawTexture(this->user->getImage(), WIDTH - 65 - ICON_SIZE, 44 - (ICON_SIZE/2), ICON_SIZE, ICON_SIZE);
        SDLHelper::getDimensions(this->username, &tw, &th);
        SDLHelper::drawTexture(this->username, this->theme->getMutedText(), WIDTH - 65 - ICON_SIZE - ICON_GAP - tw, 44 - (th/2));

        // Draw statistics
        SDLHelper::setColour(this->theme->getMutedLine());
        SDLHelper::drawRect(XPOS, YPOS, WID, 1);
        SDLHelper::drawTexture(this->p_heading, this->theme->getText(), XPOS + INDENT, YPOS + (HEI - STAT_FONT_SIZE)/2);
        SDLHelper::getDimensions(this->playtime, &tw, &th);
        SDLHelper::drawTexture(this->playtime, this->theme->getAccent(), XPOS + WID - INDENT - tw, YPOS + (HEI - STAT_FONT_SIZE)/2);

        SDLHelper::drawRect(XPOS, YPOS + HEI, WID, 1);
        SDLHelper::drawTexture(this->l_heading, this->theme->getText(), XPOS + INDENT, (YPOS + HEI) + (HEI - STAT_FONT_SIZE)/2);
        SDLHelper::getDimensions(this->launches, &tw, &th);
        SDLHelper::drawTexture(this->launches, this->theme->getAccent(), XPOS + WID - INDENT - tw, (YPOS + HEI) + (HEI - STAT_FONT_SIZE)/2);

        SDLHelper::drawRect(XPOS, YPOS + (2*HEI), WID, 1);
        SDLHelper::drawTexture(this->av_heading, this->theme->getText(), XPOS + INDENT, (YPOS + 2*HEI) + (HEI - STAT_FONT_SIZE)/2);
        SDLHelper::getDimensions(this->average_time, &tw, &th);
        SDLHelper::drawTexture(this->average_time, this->theme->getAccent(), XPOS + WID - INDENT - tw, (YPOS + 2*HEI) + (HEI - STAT_FONT_SIZE)/2);

        SDLHelper::drawRect(XPOS, YPOS + (3*HEI), WID, 1);
        SDLHelper::drawTexture(this->fp_heading, this->theme->getText(), XPOS + INDENT, (YPOS + 3*HEI) + (HEI - STAT_FONT_SIZE)/2);
        SDLHelper::getDimensions(this->first_played, &tw, &th);
        SDLHelper::drawTexture(this->first_played, this->theme->getAccent(), XPOS + WID - INDENT - tw, (YPOS + 3*HEI) + (HEI - STAT_FONT_SIZE)/2);

        SDLHelper::drawRect(XPOS, YPOS + (4*HEI), WID, 1);
        SDLHelper::drawTexture(this->lp_heading, this->theme->getText(), XPOS + INDENT, (YPOS + 4*HEI) + (HEI - STAT_FONT_SIZE)/2);
        SDLHelper::getDimensions(this->last_played, &tw, &th);
        SDLHelper::drawTexture(this->last_played, this->theme->getAccent(), XPOS + WID - INDENT - tw, (YPOS + 4*HEI) + (HEI - STAT_FONT_SIZE)/2);
        SDLHelper::drawRect(XPOS, YPOS + (5*HEI), WID, 1);

        // Title index
        SDLHelper::getDimensions(this->index, &tw, &th);
        SDLHelper::drawTexture(this->index, this->theme->getMutedText(), XPOS + (WID-tw)/2, YPOS + 5.5*HEI);

        // Draw controls
        this->controls->draw();
    }

    Details::~Details() {
        SDLHelper::destroyTexture(this->username);
        SDLHelper::destroyTexture(this->av_heading);
        SDLHelper::destroyTexture(this->fp_heading);
        SDLHelper::destroyTexture(this->lp_heading);
        SDLHelper::destroyTexture(this->l_heading);
        SDLHelper::destroyTexture(this->p_heading);
        SDLHelper::destroyTexture(this->average_time);
        SDLHelper::destroyTexture(this->first_played);
        SDLHelper::destroyTexture(this->last_played);
        SDLHelper::destroyTexture(this->launches);
        SDLHelper::destroyTexture(this->playtime);
        SDLHelper::destroyTexture(this->index);
        SDLHelper::destroyTexture(this->name);
    }
};
#include "Stats.h"
#include <algorithm>
#include <cmath>
#include <time.h>
#include <string>

void Menu_Stats::sortTitles(){
    switch (sort){
        case HomeMenu:
            std::sort(this->titles.begin(), this->titles.end(), [](Title * lhs, Title * rhs){
                return lhs->getID() < rhs->getID();
            });
            break;
        case HoursAsc:
            std::sort(this->titles.begin(), this->titles.end(), [](Title * lhs, Title * rhs){
                return lhs->getPlaytime() > rhs->getPlaytime();
            });
            break;
        case LaunchAsc:
            std::sort(this->titles.begin(), this->titles.end(), [](Title * lhs, Title * rhs){
                return lhs->getLaunches() > rhs->getLaunches();
            });
            break;
        case FirstPlayed:
            std::sort(this->titles.begin(), this->titles.end(), [](Title * lhs, Title * rhs){
                return lhs->getFirstPlayed() < rhs->getFirstPlayed();
            });
            break;
        case LastPlayed:
            std::sort(this->titles.begin(), this->titles.end(), [](Title * lhs, Title * rhs){
                return lhs->getLastPlayed() > rhs->getLastPlayed();
            });
            break;
        case AlphaAsc:
            std::sort(this->titles.begin(), this->titles.end(), [](Title * lhs, Title * rhs){
                return lhs->getName() < rhs->getName();
            });
            break;
    }
}

Menu_Stats::Menu_Stats(std::vector<Title *> titles){
    redraw = true;
    this->titles = titles;
    this->sort = HoursAsc;
    this->sortTitles();
    this->page = 1;
    this->pages = (int)ceil(titles.size()/6.0);
}

MenuType Menu_Stats::draw(u64 kDown){
    // Previous page
    if (kDown & KEY_LEFT){
        if (page > 1){
            page--;
            redraw = true;
        }
    }

    // Next page
    if (kDown & KEY_RIGHT){
        if (page < pages){
            page++;
            redraw = true;
        }
    }

    // Back
    if (kDown & KEY_B){
        return M_UserSelect;
    }

    // Sort
    if (kDown & KEY_MINUS){
        switch (sort){
            case HomeMenu:
                sort = HoursAsc;
                break;
            case HoursAsc:
                sort = LaunchAsc;
                break;
            case LaunchAsc:
                sort = FirstPlayed;
                break;
            case FirstPlayed:
                sort = LastPlayed;
                break;
            case LastPlayed:
                sort = AlphaAsc;
                break;
            case AlphaAsc:
                sort = HomeMenu;
                break;
        }
        this->sortTitles();
        this->page = 1;
        redraw = true;
    }

    // Draw
    if (redraw){
        clearConsole();
        // Print sorting type
        std::string str = "Order by: ";
        switch (sort){
            case HomeMenu:
                str += "Home Menu Order";
                break;
            case HoursAsc:
                str += "Longest Played";
                break;
            case LaunchAsc:
                str += "Most Launched";
                break;
            case FirstPlayed:
                str += "First Played";
                break;
            case LastPlayed:
                str += "Recently Played";
                break;
            case AlphaAsc:
                str += "Alphabetically";
                break;
        }
        moveCursor(CONSOLE_WIDTH/2, 0);
        for (int i = CONSOLE_WIDTH/2; i < CONSOLE_WIDTH; i++){
            std::cout << " ";
        }
        moveCursor(CONSOLE_WIDTH - str.length()+1, 0);
        std::cout << str;

        for (unsigned int i = (page-1)*6; i < (page)*6; i++){
            // Don't try and read past the end of the vector
            if (i >= titles.size()){
                break;
            }
            moveCursor(2, 4+((i%6)*6));
            std::cout << TEXT_CYAN << i+1 << ": " << titles[i]->getName() << " (" << std::hex << titles[i]->getTitleID() << std::dec << ")" << TEXT_WHITE;
            moveCursor(2, 5+((i%6)*6));

            time_t timestamp = pdmPlayTimestampToPosix(titles[i]->getFirstPlayed());
            struct tm * t = localtime(&timestamp);
            char buf[100];
            strftime(buf, 100, "%d %B %Y %I:%M %p", t);
            std::cout << "First played: " << buf;

            moveCursor(2, 6+((i%6)*6));

            timestamp = pdmPlayTimestampToPosix(titles[i]->getLastPlayed());
            t = localtime(&timestamp);
            strftime(buf, 100, "%d %B %Y %I:%M %p", t);
            std::cout << "Last played: " << buf;

            moveCursor(2, 7+((i%6)*6));
            std::cout << "Playtime: " << titles[i]->getPlaytime()/60 << " hours, "<< titles[i]->getPlaytime()%60 << " minutes";

            moveCursor(2, 8+((i%6)*6));
            std::cout << "Launched: " << titles[i]->getLaunches() << " times";
        }

        // Page indicator
        std::string s = "Page ";
        s += std::to_string(page);
        s += " of ";
        s += std::to_string(pages);
        moveCursor((CONSOLE_WIDTH/2) - (s.length()/2), CONSOLE_HEIGHT-5);
        std::cout << s;

        // Total playtime
        int tHours = 0;
        int tMins = 0;
        for (long unsigned int i = 0; i < titles.size(); i++){
            tHours += titles[i]->getPlaytime()/60;
            tMins += titles[i]->getPlaytime()%60;
        }
        while (tMins >= 60){
            tHours++;
            tMins -= 60;
        }
        moveCursor(1, CONSOLE_HEIGHT-1);
        std::cout << TEXT_CYAN << "Total: " << tHours << " hours, " << tMins << " minutes" << TEXT_RESET;

        redraw = false;
    }
    return M_Stats;
}
#include "Stats.h"
#include <algorithm>
#include <cmath>
#include <time.h>
#include <string>

#define GAMES_PER_PAGE 5

void Menu_Stats::sortTitles(){
    switch (sort){
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
            while (this->titles[0]->getFirstPlayed() == 0){
                std::rotate(this->titles.begin(), this->titles.begin()+1, this->titles.end());
            }
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
    this->pages = (int)ceil(this->titles.size()/(float)GAMES_PER_PAGE);
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
                sort = HoursAsc;
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
            case HoursAsc:
                str += "Most Played";
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

        for (unsigned int i = (page-1)*GAMES_PER_PAGE; i < (page)*GAMES_PER_PAGE; i++){
            // Don't try and read past the end of the vector
            if (i >= titles.size()){
                break;
            }
            moveCursor(2, 4+((i%GAMES_PER_PAGE)*7));
            std::cout << TEXT_CYAN << "#" << i+1 << " " << titles[i]->getName() << TEXT_WHITE;
            moveCursor(2, 5+((i%GAMES_PER_PAGE)*7));

            time_t timestamp = pdmPlayTimestampToPosix(titles[i]->getFirstPlayed());
            struct tm * t = localtime(&timestamp);
            char buf[100];
            strftime(buf, 100, "%d %B %Y %I:%M %p", t);
            std::cout << "First played:\t\t" << ((titles[i]->getFirstPlayed() > 0) ? buf : "Never");

            moveCursor(2, 6+((i%GAMES_PER_PAGE)*7));

            timestamp = pdmPlayTimestampToPosix(titles[i]->getLastPlayed());
            t = localtime(&timestamp);
            strftime(buf, 100, "%d %B %Y %I:%M %p", t);
            std::cout << "Last played:\t\t" << ((titles[i]->getFirstPlayed() > 0) ? buf : "Never");

            moveCursor(2, 7+((i%GAMES_PER_PAGE)*7));
            std::cout << "Played for:\t\t";
            switch (titles[i]->getPlaytime()/60){
                case 0:
                    break;
                case 1:
                    std::cout << "1 hour";
                    break;
                default:
                    std::cout << titles[i]->getPlaytime()/60 << " hours";
                    break;
            }
            if (titles[i]->getPlaytime()/60 != 0 && titles[i]->getPlaytime()%60 != 0){
                std::cout << ", ";
            }
            switch (titles[i]->getPlaytime()%60){
                case 0:
                    if (titles[i]->getPlaytime()/60 == 0){
                        std::cout << "0 minutes";
                    }
                    break;
                case 1:
                    std::cout << "1 minute";
                    break;
                default:
                    std::cout << titles[i]->getPlaytime()%60 << " minutes";
                    break;
            }

            moveCursor(2, 8+((i%GAMES_PER_PAGE)*7));
            u32 avgPlay = titles[i]->getPlaytime() / titles[i]->getLaunches();
            std::cout << "Avg. playtime:\t";
            switch (avgPlay/60){
                case 0:
                    break;
                case 1:
                    std::cout << "1 hour";
                    break;
                default:
                    std::cout << avgPlay/60 << " hours";
                    break;
            }
            if (avgPlay/60 != 0 && avgPlay%60 != 0){
                std::cout << ", ";
            }
            switch (avgPlay%60){
                case 0:
                    if (avgPlay/60 == 0){
                        std::cout << "0 minutes";
                    }
                    break;
                case 1:
                    std::cout << "1 minute";
                    break;
                default:
                    std::cout << avgPlay%60 << " minutes";
                    break;
            }

            moveCursor(2, 9+((i%GAMES_PER_PAGE)*7));
            std::cout << "Launched:\t\t\t";
            if (titles[i]->getLaunches() == 1){
                std::cout << "1 time";
            } else {
                std::cout << titles[i]->getLaunches() << " times";
            }
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
        std::cout << TEXT_CYAN << "Total: ";
        switch (tHours){
            case 0:
                break;
            case 1:
                std::cout << "1 hour";
                break;
            default:
                std::cout << tHours << " hours";
                break;
        }
        if (tHours != 0 && tMins != 0){
            std::cout << ", ";
        }
        switch (tMins){
            case 0:
                break;
            case 1:
                std::cout << "1 minute";
                break;
            default:
                std::cout << tMins << " minutes";
                break;
        }
        std::cout << TEXT_RESET;
        redraw = false;
    }
    return M_Stats;
}
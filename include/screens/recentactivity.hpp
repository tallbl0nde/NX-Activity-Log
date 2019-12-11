#ifndef SCREEN_RECENTACTIVITY_HPP
#define SCREEN_RECENTACTIVITY_HPP

#include "datedlist.hpp"
#include "PlayData.hpp"
#include "screen.hpp"
#include "TimeHelper.hpp"
#include "User.hpp"
#include <vector>

// Enum for period of time shown
enum TimePeriod {
    TP_Day,     // Show by day
    TP_Month,   // Show by month
    TP_Year     // Show by year
};

namespace Screen {
    class RecentActivity : public UI::Screen {
        private:
            // List
            UI::DatedList * list;
            // Texture to draw when no activity
            bool empty_list;
            SDL_Texture * no_activity;
            // PlayData object
            PlayData * playdata;
            // Top right time texture
            SDL_Texture * total_hours;
            // Used to print/render user name and image
            User * user;

            // Period of time to view
            TimePeriod view_period;
            // tm struct for start point
            struct tm start_tm;

            // Populates this->list with given time range
            void generateList();

            // Decrease start_tm by one view_period (returns false if not changed)
            bool decreaseTm();
            // Increase start_tm by one view_period (returns false if not changed)
            bool increaseTm();

        // See ui/screen.hpp for what these functions do
        public:
            // Additionally takes a user object
            RecentActivity(User *);

            void event(SDL_Event);
            void update(uint32_t dt);
            void draw();

            ~RecentActivity();
    };
}

#endif
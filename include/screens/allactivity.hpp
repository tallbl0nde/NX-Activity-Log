#ifndef SCREEN_ALLACTIVITY_HPP
#define SCREEN_ALLACTIVITY_HPP

#include "screen.hpp"
#include "sortedlist.hpp"
#include "Title.hpp"
#include "User.hpp"
#include <vector>

namespace Screen {
    class AllActivity : public UI::Screen {
        private:
            // List object
            UI::SortedList * list;
            // Set value on change to compare on resume
            bool hide_deleted;
            // Vector storing created titles
            std::vector<Title *> titles;
            // Top right time texture
            SDL_Texture * total_hours;
            // Used to print/render user name and image
            User * user;

            // Populates this->list with this->titles objects and calculates total hours
            void generateList();

            // Sort list with given SortType
            void sort_callback(int);

        // See ui/screen.hpp for what these functions do
        public:
            // Additionally takes a user object
            AllActivity(User *);

            // See ui/screen.hpp for what these functions do
            void event(SDL_Event);
            void update(uint32_t dt);
            void draw();

            ~AllActivity();
    };
}

#endif
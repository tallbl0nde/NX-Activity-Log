#ifndef UI_LISTITEM_HPP
#define UI_LISTITEM_HPP

#include <SDL2/SDL.h>
#include <string>
#include "Title.hpp"

namespace UI {
    // ListItem represents an entry/item which will be stored in
    // a list. In this particular case it is specifically for presenting
    // a game's icon, name and play stats
    class ListItem {
        private:
            // Pointer to title object from which all info is read
            Title * title;
            // Self-explanatory
            bool selected;
            // String holding time of last play
            std::string last_played;

        public:
            // See ui/screen.hpp for similar explanations of these functions
            // Assign Title pointer
            ListItem(Title *);

            void update(uint32_t);

            void draw(SDL_Renderer *, int, int, int, int);

            ~ListItem();
    };
};

#endif
#ifndef UI_LIST_ITEM_ACTIVITY_HPP
#define UI_LIST_ITEM_ACTIVITY_HPP

#include "listitem.hpp"
#include "SDLHelper.hpp"
#include "User.hpp"
#include "Title.hpp"

namespace UI::ListItem {
    // The Activity ListItem represents an entry on the play activity
    // screen. It holds a game icon, title object and textures.
    class Activity : public List_Item {
        private:
            // Rendered textures
            SDL_Texture * icon;
            SDL_Texture * lastplayed;
            SDL_Texture * launches;
            SDL_Texture * playtime;
            SDL_Texture * rank;
            SDL_Texture * title;
            // Title object used for sorting
            Title * title_object;
            // Scrolling text variables
            int text_scroll_x;
            int text_scroll_pause;

            // TEMPORARY
            User * user;

        public:
            // Constructor accepts the title object
            Activity(User *, Title *);

            // Update and draw functions are defined
            void update(uint32_t);
            void draw();

            // Pressed does nothing yet
            void pressed();

            // Set the "rank" of the current item
            void setRank(unsigned int);

            void setSelected(bool);

            // Returns title object pointer (for sorting)
            Title * getTitleObj();

            // Destructor deletes textures
            ~Activity();
    };
};

#endif
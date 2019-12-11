#ifndef UI_LIST_ITEM_RecentActivity_HPP
#define UI_LIST_ITEM_RecentActivity_HPP

#include "listitem.hpp"
#include "PlayData.hpp"
#include "SDLHelper.hpp"

namespace UI::ListItem {
    // The RecentActivity ListItem represents an entry on the RecentActivity
    // screen. It holds a game icon and textures.
    class RecentActivity : public List_Item {
        private:
            // Rendered textures
            SDL_Texture * icon;
            SDL_Texture * launches;
            SDL_Texture * playtime;
            SDL_Texture * rank;
            SDL_Texture * title;

            // Scrolling text variables
            int text_scroll_x;
            int text_scroll_pause;

        public:
            // Constructor accepts RecentPlayStatistics
            RecentActivity(RecentPlayStatistics *);

            // Update and draw functions are defined
            void update(uint32_t);
            void draw();

            // Pressed does nothing yet
            void pressed();

            // Set the "rank" of the current item
            void setRank(unsigned int);

            void setSelected(bool);

            // Destructor deletes textures
            ~RecentActivity();
    };
};

#endif
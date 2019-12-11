#ifndef UI_DATEDLIST_HPP
#define UI_DATEDLIST_HPP

#include "list.hpp"
#include "listitem_recentactivity.hpp"
#include <string>

namespace UI {
    // A sorted list essentially is just a list but can sort the items
    class DatedList : public List {
        private:
            // Texture for date/period
            SDL_Texture * date_text;

        public:
            // The constructor does not accept ListItems
            DatedList(bool *, int, int, int, int);

            // Only accepts recent activity items
            void addItem(ListItem::RecentActivity *);

            // Draw at stored coords
            void draw();

            // Render date string with provided string
            void setDate(std::string);

            // Destructor frees stored ListItems
            ~DatedList();
    };
};

#endif
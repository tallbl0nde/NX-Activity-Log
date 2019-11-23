#ifndef SETTINGLIST_HPP
#define SETTINGLIST_HPP

#include "navigable.hpp"
#include "settinglistitem.hpp"
#include <vector>

namespace UI {
    // A setting list is a list that contains items that toggle
    // settings (merge into normal list at some point?)
    class SettingList : public Navigable {
        private:
            // Vector of SettingListItems
            std::vector<SettingListItem *> items;
            // Used for scrolling
            unsigned int pos;
            unsigned int max_pos;
            // Scrolling vars
            bool is_touched;
            unsigned int hi_pos;
            unsigned int t_pos;

        public:
            // The constructor does not accept ListItems
                SettingList(bool *, int, int, int, int);

                // Add an item to the list
                void addItem(SettingListItem *);

                void update(uint32_t);

                // Draw at stored coords
                void draw();

                // Handles touches
                void touched(uint32_t, float, float, float = 0, float = 0);

                // Returns if scrolling
                bool isTouched();

                // Handles button presses
                void handleButton(uint8_t, uint8_t);

                unsigned int getPos();

                void setPos(unsigned int);

                // Destructor frees stored ListItems
                ~SettingList();
    };
};

#endif
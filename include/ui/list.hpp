#ifndef UI_LIST_HPP
#define UI_LIST_HPP

#include "listitem.hpp"
#include <SDL2/SDL.h>
#include <vector>

namespace UI {
    // A List contains ListItems and handles the positioning and
    // selection stuff. A small navigation bar is also shown
    class List {
        private:
            // Vector containing ListItems
            std::vector<ListItem *> items;
            // Index of top element
            size_t pos;

        public:
            // The constructor does not accept ListItems
            List();

            // Add an item to the list
            void addItem(ListItem *);

            void update(uint32_t);

            // Draw at x, y with w, h
            void draw(int, int, int, int);

            // Return pos
            size_t getPos();

            // Change pos to parameter
            void movePos(size_t);

            // Destructor frees stored ListItems
            ~List();
    };
};

#endif
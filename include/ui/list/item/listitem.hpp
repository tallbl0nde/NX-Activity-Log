#ifndef UI_LIST_ITEM_LISTITEM_HPP
#define UI_LIST_ITEM_LISTITEM_HPP

#include "drawable.hpp"

namespace UI {
    // A ListItem represents an entry in a list. It can be selectable
    // or just text. This is an abstract class and must be inherited.
    class List_Item : public Drawable {
        protected:
            // Can this element be selected?
            bool is_selectable;
            bool auto_highlight;
            // Is this item currently selected?
            bool is_selected;
            // Offset/position in list (in pixels)
            int offset;

        public:
            // Constructor initializes variables
            List_Item();

            // Update and draw functions should be overridden
            virtual void update(uint32_t);
            virtual void draw() = 0;

            // Pressed is called when the item has been pressed/tapped
            virtual void pressed();

            // Get/set offset
            int getOffset();
            void setOffset(int);

            // Adjust is_selected variable
            virtual void setSelected(bool);

            // Returns true if this item is selectable
            bool isSelectable();

            // Returns true if this item has it's own highlight code
            bool hasOwnHighlighting();

            // Destructor does nothing
            ~List_Item();
    };
};

#endif
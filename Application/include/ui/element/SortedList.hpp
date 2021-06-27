#ifndef ELEMENT_SORTEDLIST_HPP
#define ELEMENT_SORTEDLIST_HPP

#include "Aether/Aether.hpp"
#include "ui/element/ListActivity.hpp"
#include "Types.hpp"

// Struct storing information about entry used for sorting
struct SortInfo {
    std::string name;                // Name of title
    unsigned long long int titleID;  // Title's ID
    unsigned int firstPlayed;        // Timestamp of first launch
    unsigned int lastPlayed;         // Timestamp of last play
    unsigned int playtime;           // Total playtime in seconds
    unsigned int launches;           // Total launches
};

namespace CustomElm {
    // A SortedList extends Aether::List by storing a SortType,
    // and reorders items without the need for recreation.
    class SortedList : public Aether::List {
        private:
            // Pointer to heading to change text
            Aether::Text * heading;

            // Vector of pointers to sortinfo (aligns with element position)
            std::vector<SortInfo *> sortinfo;

            // Method items are currently sorted with
            SortType sorting;

        public:
            // X, Y, W, H
            SortedList(int, int, int, int);

            // An added element needs to have relevant information stored in order to sort
            // Only accepts ListActivity due to information requirements
            void addElement(ListActivity *, SortInfo *);
            // Also frees memory used by SortInfos
            void removeAllElements();
            // Returns all list items and SortInfos
            void returnAllElements();

            // Sets sort method and sorts items
            void setSort(SortType);
            // Returns SortType
            SortType sort();

            // Set colour of heading text
            void setHeadingColour(Aether::Colour);
    };
};

#endif

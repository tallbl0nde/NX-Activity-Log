#ifndef TITLE_HPP
#define TITLE_HPP

#include "Types.hpp"

namespace NX {
    class Title {
        private:
            // Game's titleID
            TitleID titleID_;
            // Is the game installed?
            bool is_installed;
            // Game's name
            std::string name_;
            // Pointer to and size of image
            u8 * ptr;
            u32 size;

        public:
            // The constructor derives all relevant info from given titleID
            Title(TitleID, bool);

            // These functions return Title's data
            TitleID titleID();
            bool isInstalled();
            std::string name();
            u8 * imgPtr();
            u32 imgSize();

            // Frees memory icon is stored in
            ~Title();
    };
};

#endif
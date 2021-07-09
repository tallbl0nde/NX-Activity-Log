#ifndef ELEMENT_LISTADJUST_HPP
#define ELEMENT_LISTADJUST_HPP

#include "Aether/Aether.hpp"

namespace CustomElm {
    // A list element storing an icon, name and two playtime strings
    class ListAdjust : public Aether::AsyncItem {
        private:
            // Height of item
            static const int height = 100;

            // Bounding lines
            Aether::Rectangle * topR;
            Aether::Rectangle * bottomR;

            // Title elements
            Aether::Image * icon;
            Aether::Text * title;
            Aether::Text * recordTime;
            Aether::Text * adjustTime;

            // Positions element
            void positionElements();

        public:
            // Contructs a new item using the passed title and playtime strings
            ListAdjust(const std::string &, const std::string &, const std::string &);

            // Override update to fade in/out lines
            void update(uint32_t);

            // Update the adjusted play time
            void setAdjustedTime(const std::string &);

            // Set the icon
            void setImage(uint8_t *, uint32_t);

            // Set colours
            void setAdjustColour(const Aether::Colour &);
            void setLineColour(const Aether::Colour &);
            void setRecordColour(const Aether::Colour &);
            void setTitleColour(const Aether::Colour &);
    };
};

#endif

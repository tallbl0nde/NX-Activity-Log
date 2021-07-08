#ifndef ELEMENT_LISTHIDE_HPP
#define ELEMENT_LISTHIDE_HPP

#include "Aether/Aether.hpp"

namespace CustomElm {
    // List item containing game icon, name, id + tick
    class ListHide : public Aether::AsyncItem {
        private:
            // Height of item in pixels
            static const int height = 80;

            // Pointers to elements (async)
            Aether::Image * icon;
            Aether::Text * title;
            Aether::Text * titleID;

            // Pointers to sync elements
            Aether::Rectangle * topR;
            Aether::Rectangle * bottomR;
            Aether::Box * circle;
            Aether::Tick * tick;

            // Position elements once rendering is complete
            void positionElements();

        public:
            // Constructor accepts title and ID
            ListHide(const std::string &, const std::string &);

            // Override update to fade in sync items
            void update(uint32_t);

            // Set icon using raw data
            void setImage(uint8_t *, uint32_t);

            // Set colours
            void setIDColour(const Aether::Colour &);
            void setLineColour(const Aether::Colour &);
            void setTitleColour(const Aether::Colour &);
            void setTickBackgroundColour(const Aether::Colour &);
            void setTickForegroundColour(const Aether::Colour &);

            // Toggle hidden state
            bool isTicked();
            void setTicked(const bool);
    };
};

#endif

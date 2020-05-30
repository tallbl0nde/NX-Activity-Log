#ifndef SCREEN_DETAILS_HPP
#define SCREEN_DETAILS_HPP

#include "ui/element/Graph.hpp"
#include "ui/overlay/PlaySession.hpp"

// Forward declaration due to circular dependency
namespace Main {
    class Application;
};

namespace Screen {
    // Detailed activity screen
    class Details : public Aether::Screen {
        private:
            // Pointer to app for theme
            Main::Application * app;
            // Used to udpate prev. screen
            bool popped;

            // Updates graph if data matching current time range
            void updateGraph();
            // Updates the list of play sessions matching current time range
            void updateSessions();
            // Element which marks top of sessions
            Aether::Element * topElm;

            // Container holding heading and L/R
            Aether::Container * header;

            // Pointers to elements
            CustomElm::Graph * graph;
            Aether::Text * graphHeading;
            Aether::Text * graphSubheading;
            Aether::Element * graphTotal;
            Aether::Text * graphTotalHead;
            Aether::Text * graphTotalSub;

            Aether::Text * playtime;
            Aether::Text * avgplaytime;
            Aether::Text * timeplayed;
            Aether::Text * firstplayed;
            Aether::Text * lastplayed;

            Aether::Image * icon;
            Aether::List * list;
            Aether::ListHeadingHelp * playHeading;
            Aether::Text * noStats;
            Aether::Text * title;
            Aether::Image * updateElm;
            Aether::Image * userimage;
            Aether::Text * username;

            // MessageBox overlay (used for multiple things)
            Aether::MessageBox * msgbox;
            // Popup for session breakdown
            CustomOvl::PlaySession * panel;

            // Prepare msgbox for session help
            void setupSessionHelp();

            // Prepare panel for session breakdown
            // Takes PlaySession
            void setupSessionBreakdown(NX::PlaySession);

            // Updates the "recent activity" part of the screen
            void updateActivity();

        public:
            // Sets up elements
            Details(Main::Application *);

            // Update checks for change in application's time and regenerates graph
            void update(uint32_t);

            // Create user + title reliant elements
            void onLoad();
            // Free elements created in onLoad()
            void onUnload();
    };
};

#endif
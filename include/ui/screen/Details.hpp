#ifndef SCREEN_DETAILS_HPP
#define SCREEN_DETAILS_HPP

#include "Application.hpp"
#include "Graph.hpp"

// Forward declaration due to circular dependency
namespace Main {
    class Application;
};

// Graph display period
enum class GraphViewType {
    MinPerHour,      // Show minutes per hour in day
    HourPerDay,      // Show hours per day in month
    HourPerMonth     // Show hours per month in year
};

namespace Screen {
    // Detailed activity screen
    class Details : public Aether::Screen {
        private:
            // Pointer to app for theme
            Main::Application * app;

            // Creates relevant graph
            void createGraph();

            // Pointers to elements
            CustomElm::Graph * graph;
            Aether::Text * graphHeading;
            Aether::Text * graphSubheading;
            Aether::Text * graphTotal;
            Aether::Text * graphTotalSub;
            Aether::Text * graphPercentage;
            Aether::Text * graphPercentageSub;

            Aether::Image * icon;
            Aether::List * list;
            Aether::Text * title;
            Aether::Image * userimage;
            Aether::Text * username;

            // MessageBox overlay (used for multiple things)
            Aether::MessageBox * msgbox;

            // Prepare msgbox for session help
            void setupSessionHelp();

            // Prepare msgbox for session breakdown
            // Takes PlaySession
            void setupSessionBreakdown(NX::PlaySession);

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
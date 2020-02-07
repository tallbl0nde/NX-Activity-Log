#ifndef SCREEN_DETAILS_HPP
#define SCREEN_DETAILS_HPP

#include "Application.hpp"
#include "Graph.hpp"

// Forward declaration due to circular dependency
namespace Main {
    class Application;
};

// Graph display type
enum class GraphDataType {
    Playtime,
    Launches
};

// Graph display period
enum class GraphViewType {
    Day,
    Month
};

namespace Screen {
    // Detailed activity screen
    class Details : public Aether::Screen {
        private:
            // Pointer to app for theme
            Main::Application * app;

            // Graph things
            GraphDataType graphData;
            GraphViewType graphView;

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
            // Popuplist overlay (used for graph options)
            Aether::PopupList * popup;

            // Prepare popup for graph period
            void setupGraphPeriod();

            // Prepare popup for graph type
            void setupGraphType();

            // Prepare msgbox for session help
            void setupSessionHelp();

            // Prepare msgbox for session breakdown
            // Takes PlaySession
            void setupSessionBreakdown(NX::PlaySession);

        public:
            // Sets up elements
            Details(Main::Application *);

            // Create user + title reliant elements
            void onLoad();
            // Free elements created in onLoad()
            void onUnload();
    };
};

#endif
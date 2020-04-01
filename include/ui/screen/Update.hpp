#ifndef SCREEN_UPDATE_HPP
#define SCREEN_UPDATE_HPP

#include "Application.hpp"
#include <future>

// Forward declaration due to circular dependency
namespace Main {
    class Application;
};

namespace Screen {
    // "Update" page
    class Update : public Aether::Screen {
        private:
            // Pointer to main app object for user + titles
            Main::Application * app;

            // Controls so the colours can be changed
            Aether::Controls * controls;
            // Element containing other elements created based on outcome
            Aether::Element * el;

            // "Checking" overlay
            Aether::MessageBox * msgbox;

            // Future for update thread
            std::future<UpdateData> data;
            // Set true once thread is complete
            bool threadDone;

            // Create elements based on if there's an update or not
            void showElements();

        public:
            // Passed main application object
            Update(Main::Application *);

            // Update handles hiding overlay + updating status
            void update(uint32_t);

            void onLoad();
            // Delete elements created in onLoad()
            void onUnload();
    };
};


#endif
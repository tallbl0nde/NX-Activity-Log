#ifndef SCREEN_UPDATE_HPP
#define SCREEN_UPDATE_HPP

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
            Aether::ControlBar * controls;
            // Element containing other elements created based on outcome
            Aether::Element * el;
            // Progress elements
            Aether::RoundProgressBar * pbar;
            Aether::Text * ptext;

            // "Checking" overlay
            Aether::MessageBox * msgbox;

            // Future for check thread
            std::future<UpdateData> data;
            // Set true once thread is complete
            bool threadDone;

            // Percentage of download finished
            std::atomic<double> downloaded;
            // Is the update being downloaded (eg update UI?)
            bool isDownloading;
            // URL to download from
            std::string nroURL;
            // Future for update thread
            std::future<bool> updateThread;

            // Create a 'blank' message box without OK button
            void createMsgbox();
            // Callback for download to update progress
            void progressCallback(long long int, long long int);
            // Show download box
            void showDownloadProgress();
            // Create elements based on if there's an update or not
            void showElements();
            // Show error message for download
            void showError();
            // Download succeeded
            void showSuccess();

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
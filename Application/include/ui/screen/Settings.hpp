#ifndef SCREEN_SETTINGS_HPP
#define SCREEN_SETTINGS_HPP

#include "ui/overlay/ProgressBox.hpp"

// Forward declaration due to circular dependency
namespace Main {
    class Application;
};

namespace Screen {
    // Application Settings screen
    class Settings : public Aether::Screen {
        private:
            // Types of running jobs
            enum class Job {
                Import,
                Export,
                None
            };

            // Current job running
            Job job;

            // Pointer to main app object for config
            Main::Application * app;
            // Type of creation
            ScreenCreate createReason;

            // Pointers to elements
            Aether::Text * heading;
            Aether::Image * image;
            Aether::List * list;
            Aether::Menu * menu;
            Aether::Image * updateElm;

            // Pointers to options so the value can be updated
            Aether::ListOption * optionLang;
            Aether::ListOption * optionScreen;
            Aether::ListOption * optionSort;
            Aether::ListOption * optionTheme;
            Aether::ListButton * optionThemeEdit;
            Aether::ListOption * optionView;

            Aether::ListOption * option24H;
            Aether::ListOption * optionGraph;
            Aether::ListButton * optionAdjust;
            Aether::ListButton * optionHide;
            Aether::ListOption * optionPage;

            Aether::ListButton * optionDeleteImport;

            // MessageBox for forwarder/setting info
            Aether::MessageBox * msgbox;
            // PopupList for list-related options
            Aether::PopupList * popuplist;

            // MessageBox containing progress bar
            CustomOvl::ProgressBox * progressbox;
            std::atomic<double> progressValue;

            void installForwarder();

            // Set elements and highlight one in overlay
            void prepareMessageBox();
            void preparePopupList(std::string);
            void setupGenericMessageOverlay(const std::string &);
            void setupLangOverlay();
            void setupScreenOverlay();
            void setupSortOverlay();
            void setupThemeOverlay();
            void setupViewOverlay();

            // Show overlays
            void showExportOverlay();
            void showImportOverlay();
            void showDeleteImportedOverlay();

        public:
            // Passed main application object
            Settings(Main::Application *, ScreenCreate);

            void update(uint32_t);

            // Prepare user-specific elements
            void onLoad();
            // Delete elements created in onLoad()
            void onUnload();
    };
};

#endif

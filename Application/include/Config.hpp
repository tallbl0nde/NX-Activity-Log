#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <switch.h>
#include "SimpleIniParser.hpp"
#include "Types.hpp"

struct AdjustmentValue {
    uint64_t titleID;
    AccountUid userID;
    int value;
};

namespace Main {
    // Reads/writes and stores config of application
    class Config {
        private:
            // Ini object to read/write to/from file
            simpleIniParser::Ini * ini;

            // Vector of adjustments for each title ID
            std::vector<AdjustmentValue> adjustments;

            // Vector of hidden title IDs
            std::vector<uint64_t> hidden;

            bool gGraph_;
            bool gIs24H_;
            Language gLang_;
            ThemeType gTheme_;
            ScreenID lScreen_;
            SortType lSort_;
            ViewPeriod lView_;
            bool tImage_;

        public:
            // Sets ini to nullptr
            Config();
            // Deletes ini
            ~Config();

            // Reads in config from file (call first otherwise expect crashes!)
            void readConfig();
            // Writes config to file
            void writeConfig();

            // Getters + setters for all settings'
            std::vector<AdjustmentValue> adjustmentValues();
            std::vector<uint64_t> hiddenTitles();
            bool gGraph();
            bool gIs24H();
            Language gLang();
            ThemeType gTheme();
            ScreenID lScreen();
            SortType lSort();
            ViewPeriod lView();
            bool tImage();

            bool setAdjustmentValues(const std::vector<AdjustmentValue> &);
            bool setHiddenTitles(const std::vector<uint64_t> &);
            void setGGraph(bool);
            void setGIs24H(bool);
            void setGLang(Language);
            void setGTheme(ThemeType);
            void setLScreen(ScreenID);
            void setLSort(SortType);
            void setLView(ViewPeriod);
            void setTImage(bool);
    };
};

#endif

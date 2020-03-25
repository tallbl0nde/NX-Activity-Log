#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "SimpleIniParser.hpp"
#include "Types.hpp"

namespace Main {
    // Reads/writes and stores config of application
    class Config {
        private:
            // Ini object to read/write to/from file
            simpleIniParser::Ini * ini;

            bool gGraph_;
            bool gIs24H_;
            ThemeType gTheme_;
            bool hDeleted_;
            ScreenID lScreen_;
            SortType lSort_;
            ViewPeriod lView_;

        public:
            // Sets ini to nullptr
            Config();
            // Deletes ini
            ~Config();

            // Reads in config from file (call first otherwise expect crashes!)
            void readConfig();
            // Writes config to file
            void writeConfig();

            // Getters + setters for all settings
            bool gGraph();
            bool gIs24H();
            ThemeType gTheme();
            bool hDeleted();
            ScreenID lScreen();
            SortType lSort();
            ViewPeriod lView();

            void setGGraph(bool);
            void setGIs24H(bool);
            void setGTheme(ThemeType);
            void setHDeleted(bool);
            void setLScreen(ScreenID);
            void setLSort(SortType);
            void setLView(ViewPeriod);
    };
};

#endif
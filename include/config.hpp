#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Types.hpp"

namespace Main {
    // Reads/writes and stores config of application
    class Config {
        private:
            // Initializes config file (returns false if error)
            void initConfigFile();

            // Settings (first char indicates section of ini)
            // g: [general]
            // h: [hidden]
            bool gGraph_;
            SortType gSort_;
            ThemeType gTheme_;
            bool hDeleted_;

        public:
            // Reads in config from file (returns false if error)
            void readConfig();

            // Writes config to file (returns false if error)
            void writeConfig();

            // Getters + setters for all settings
            bool gGraph();
            SortType gSort();
            ThemeType gTheme();
            bool hDeleted();

            void setGGraph(bool);
            void setGSort(SortType);
            void setGTheme(ThemeType);
            void setHDeleted(bool);
    };
};

#endif
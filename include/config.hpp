#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "list.hpp"
#include "theme.hpp"

// Singleton class storing the application's config
class Config {
    private:
        // Single instance of this class
        static Config * instance;

        // Constructor is private!
        Config();

        // Initializes config file (returns false if error)
        void initConfigFile();

        // Settings
        SortType general_sort;
        ThemeType general_theme;
        bool hidden_deleted;
        bool hidden_unplayed;

    public:
        // Creates instance if doesn't exist, otherwise returns pointer
        static Config * getConfig();

        // Reads in config from file (returns false if error)
        void readConfig();

        // Writes config to file (returns false if error)
        void writeConfig();

        // Getters + setters for all settings
        SortType getGeneralSort();
        ThemeType getGeneralTheme();
        bool getHiddenDeleted();
        bool getHiddenUnplayed();

        void setGeneralSort(SortType);
        void setGeneralTheme(ThemeType);
        void setHiddenDeleted(bool);
        void setHiddenUnplayed(bool);
};

#endif
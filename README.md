# NX Activity Log

**NX Activity Log** is a homebrew application for the Nintendo Switch which displays more precise information about your play activity.

[Download](https://github.com/tallbl0nde/NX-Activity-Log/releases)

Curious about what's next? See my to-do list on [Trello](https://trello.com/b/HaJ1THGq/nx-activity-log)

## Contents

1. [Features](#features)
2. [Screenshots](#screenshots)
3. [Reporting Incorrect Data](#reporting-incorrect-data)
4. [Known Issues](#known-issues)
5. [Translations](#translations)
6. [Credits](#credits)
7. [Support](#support-3)

## Features

This application currently has the following features:

* Viewing Play Activity (per user)
  * All Time Activity
    * Total Playtime in minutes
    * Number of times a game has been launched
    * Date of first launch
    * Date/time of most recent launch
    * Average time spent in a game
    * Sorting games by time played, recently played, etc.
  * Recent Activity (see note below)
    * Graph visualizing play time
    * View by day, month or year
    * Total playtime in seconds
    * Number of launches
    * Viewing each 'Play Session'
  * Event-by-event breakdown of your activity
* Multi-language Support
  * English, Chinese (Simplified and Traditional), French, German, Italian, Korean, Portugese, Russian, Spanish and Turkish
* Theme Support
  * Automatic theme detection to match the Switch's colour scheme
  * Custom Themes
    * All colours used in the app can be adjusted
    * Set a custom background image
      * **Requires a PNG saved at /config/NX-Activity-Log/background.png**
* Update in-app
  * Checks for updates automatically and displays an icon if one is available
  * Update completely within the app
* User Page Replacement
  * Requires LayeredFS and either Atmosphere 0.10.0+, ReiNX or SXOS (or build and copy the forwarder to your CFW-specific titles folder)
  * **Requires .nro to be at /switch/NX-Activity-Log/NX-Activity-Log.nro**

_Note: The data shown in Recent Activity and Details may be slightly inaccurate over larger periods of time (ie. off by a few minutes) but I will try to improve this over time. If activity is not being shown for earlier periods of time it is likely your switch has been reset at some point, which wipes the data used to calculate playtime in this way._

## Screenshots

![Recent View](/Images/sc_recent.jpg)
![All Activity View](/Images/sc_activity.jpg)
![Detailed View](/Images/sc_detailed.jpg)

## Reporting Incorrect Data

If you are seeing wildly incorrect values/believe the playtime shown is incorrect within Recent Activity or the Details screen, please do the following:

1. Download and run the .nro from [here](https://github.com/tallbl0nde/PlayEventParser/releases)
2. Leave it run; if it appears to be frozen leave it for up to a minute!
3. Once it is done there should be a playlog.txt at the root of your SD card.
4. Create an issue with the following:
    * Screenshots of the incorrect data (make sure the date/time and playtime are visible!)
    * The playlog.txt acquired from the last steps

_Note: If the data shown under All Activity is incorrect there is nothing I can do! These values are read directly from the Switch and aren't the result of any calculations._

## Known Issues

* The main issue at the moment is that a few users' playtime is incorrect. This is due to the Switch being factory reset at some point and/or some games not requiring a user to be selected to play it. _I am looking into how to fix the former!_
* Having a lot of games logged can cause too much memory usage when launched via User Page/Album, leading to out of memory crashes and/or missing images/text.
  * I think I know a way to fix this but if it impacts you you'll have to launch this app in title mode for now.
* Having a lot of games logged causes a long pause/freeze when loading the 'All Activity' screen
  * Once I learn how to handle threads this won't be a problem :)

## Translations

If you'd like to translate the app or fix an issue with a translation, please make a pull request to [this repo](https://github.com/tallbl0nde/NX-Activity-Log-Translations)! I'll add all the relevant code here (if need be) once I see the request :)

## Credits

I'd like to thank:

* AtlasNX for [SimpleIniParser](https://github.com/AtlasNX/SimpleIniParser)
  * Used to read/write the config file
* nlohmann for his [JSON library](https://github.com/nlohmann/json)
* Switchbrew for [nx-hbloader](https://github.com/switchbrew/nx-hbloader)
  * Adapted to override User Page with this app
* Anyone else involved with the development of homebrew tools and reverse engineering of the Switch!

## Support <3

There is absolutely no obligation, however if you have found this software useful you can support me on Ko-fi!

[![ko-fi](https://www.ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/J3J718RRQ)

Knowing my software is being used is enough to motivate me to continue work on it!

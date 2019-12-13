# NX Activity Log

**NX Activity Log** is a homebrew application for the Nintendo Switch which displays more accurate information about your play activity. The UI has been designed to feel familiar and is navigated in a similar manner to the Switch's firmware.

Currently, this application includes the following features:

* Viewing Play Activity (per user)
  * All Time Activity
    * Total Playtime in minutes
    * Number of times a game has been launched
    * Date of first launch
    * Date/time of most recent launch
    * Average time spent in a game
    * Sorting games by time played, recently played, etc.
  * Recent Activity (see note below)
    * View by day, month or year
    * Total playtime in seconds
    * Number of launches
    * Sorted by most played
* Filtering Games
  * Ability to omit deleted games from 'All Activity'
* Miscellaneous
  * Automatic theme detection to match the Switch's colour scheme
  * Ability to override User Page
    * Requires LayeredFS and either Atmosphere 0.10.0+, ReiNX or SXOS (or build and copy the forwarder to your CFW-specific titles folder)
    * **Requires .nro to be at /switch/NX-Activity-Log.nro**

_Note: The data shown in Recent Activity may be slightly inaccurate over larger periods of time (ie. off by a few minutes) but I will try to improve this over time. If activity is not being shown for earlier periods of time it is likely your switch has been reset at some point, which wipes the data used to calculate playtime in this way._

## Screenshots

![Recent View](/img/sc_recent.jpg)
![All Activity View](/img/sc_activity.jpg)
![Detailed View](/img/sc_detailed.jpg)

## Known Issues

* The main issue at the moment is that a few users' playtime is incorrect. This is due to the Switch being factory reset at some point and/or some games not requiring a user to be selected to play it. _I am looking into how to fix the former!_
* The right half of the "game details" screen is blank
  * This isn't an issue, I just left it empty to place something there later ;)
* Tapping on a game in 'Recent Activity' does nothing
  * Again, not an issue as I haven't added anything to appear just yet

_Note that no support will be given by me for SX OS as I do not intend to run their CFW. I am happy to merge pull requests that fix bugs on SX OS should there be any, though._

## Support

There is absolutely no obligation, however if you have found this software useful you can support me on Ko-fi!

[![ko-fi](https://www.ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/J3J718RRQ)

Knowing this software is being used is enough to motivate me to keep working on it!

## Thanks to

* AtlasNX for [SimpleIniParser](https://github.com/AtlasNX/SimpleIniParser)
  * Used to read/write the config file
* Switchbrew for [nx-hbloader](https://github.com/switchbrew/nx-hbloader)
  * Adapted to override User Page with this app
# NX Activity Log

_Important: You must have some method of launching homebrew in **Title Takeover**  mode to use this application._

**NX Activity Log** is a homebrew application for the Nintendo Switch which provides users with the amount of information we should have been given on the User Page. The UI has been designed to feel familiar and is navigated in a similar manner to the Switch's firmware.

Currently, this application includes the following features:

* Viewing Play Activity (per user)
  * Total time the chosen user has spent playing games
  * Sorting games by time played, recently played, etc.
  * Total time spent in a title in _minutes, not 5 hour segments_
  * Number of times a game has been launched
  * Date of first launch
  * Date/time of most recent launch
  * Average time spent in a game
* Filtering Games
  * Ability to omit deleted games from your activity
  * Ability to hide the activity of games that haven't been played
* Miscellaneous
  * Automatic theme detection to match the Switch's colour scheme

## Screenshots

![Main View](/img/screenshot.jpg)
![Detailed View](/img/screenshot2.jpg)

## Known Issues

* The main issue at the moment is that a few users' playtime is incorrect. This is due to the Switch being factory reset at some point and/or some games not requiring a user to be selected to play it. _I am looking into how to fix the former!_
* The right half of the "game details" screen is blank
  * This isn't an issue, I just left it empty to place something there later ;)

_Note that no support will be given by me for SX OS as I do not intend to run their CFW. I am happy to merge pull requests that fix bugs on SX OS should there be any, though._

## Support

There is absolutely no obligation, however if you have found this software useful you can support me on Ko-fi!

[![ko-fi](https://www.ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/J3J718RRQ)

Knowing this software is being used is enough to motivate me to keep working on it!

## Thanks to

* AtlasNX for [SimpleIniParser](https://github.com/AtlasNX/SimpleIniParser)

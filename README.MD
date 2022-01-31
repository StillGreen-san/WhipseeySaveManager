# Whipseey Save Manager

**GUI for modifying *some* Values in 'Whipseey and the Lost Atlas' Savegames and Settings**

![Picture](WhipseeySaveManager.gif)

Intended to aid with Speedrun practice. Can modify *most* values present in the Savegame.

The Game needs to be restarted to load a modified Savegame or Settings. Autosaving ingame will override all Slots in the Savegame.

## Features

- Set Save-Slot values
  - Level-Progress, Lives, etc
- Change Options
  - Language, Volume, etc
- Enable Cheats

## Warnings

- Windows only
- only tested on Windows 10 (2004) x64
- still no darkmode! 😞
- all IO actions will block the GUI, so it may hang a bit if you store your files on a floppy

## Code

- written in/with C++ 17
- uses cmake (find_package / find_path)
- C++ rewrite mostly intended as an exercise, thus somewhat over engineered compared to the previous AutoIT version
- previous AutoIT version can be found in au3 branch

### Dependencies

- Registry : [WinReg](https://github.com/GiovanniDicanio/WinReg)
- INI : [simpleini](https://github.com/brofield/simpleini)
- GUI : [nana](https://github.com/cnjinhao/nana)
  - uses vcpkg specific name "unofficial-nana" in CMakeLists.txt

for testing only:
- Testing : [Catch2](https://github.com/catchorg/Catch2)

### Tests

some tests are hidden and need to be run explicitly. see tags below

- [Manual] some of there returns need to be checked by hand
- [Game] output varies depending on if the game is installed (with steam)
- [Save] output varies depending on if a savegame is available in the default location

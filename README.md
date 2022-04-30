EmulationStation
================

This is a fork of EmulationStation for RetroPie.
EmulationStation is a cross-platform graphical front-end for emulators with controller navigation.

Building
========

**Building on Linux**

EmulationStation uses some C++11 code, which means you'll need to use at least g++-4.7 on Linux to compile.

EmulationStation has a few dependencies. For building, you'll need CMake, SDL2, FreeImage and FreeType.  You also should probably install the `fonts-droid` package which contains fallback fonts for Chinese/Japanese/Korean characters, but ES will still work fine without it (this package is only used at run-time).

**On Debian/Ubuntu:**
All of this be easily installed with `apt-get`:
```bash
sudo apt-get install libsdl2-dev libfreeimage-dev libfreetype6-dev \
  libasound2-dev libgles2-mesa-dev build-essential cmake fonts-droid-fallback
```
**On Fedora:**
All of this be easily installed with `dnf` (with rpmfusion activated) :
```bash
sudo dnf install SDL2-devel freeimage-devel freetype-devel \
  alsa-lib-devel mesa-libGL-devel cmake
```

**Note**: this repository uses a git submodule - to checkout the source and all submodules, use

```bash
git clone --recursive https://github.com/nismoryco/emulationstation.git
```

or 

```bash
git clone https://github.com/nismoryco/emulationstation.git
cd emulationstation
git submodule update --init
```

Then, generate and build the Makefile with CMake:
```bash
cd emulationstation
mkdir build
cd build
cmake ..
make
```

NOTE: to generate a `Debug` build on Unix/Linux, run the Makefile generation step as:
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

**On the Raspberry Pi**  

* Choosing a GLES implementation.

   * if the Pi system uses the legacy/Broadcom driver, install the `libraspberry-dev` package before running `cmake` to configure the build
   * if the Pi system uses the Mesa VC3/V3D GL driver, build using `-DUSE_MESA_GLES=On` to choose the MESA GLES implementation. This option is _mandatory_ when compiling for a Pi4 system, since the legacy GL drivers are not supported anymore on this system.

**GLES build notes**

 If your system doesn't have a working GLESv2 implementation, the GLESv1 legacy renderer can be compiled in by adding `-DUSE_GLES1=On` to the build options.

Configuring
===========

**~/.emulationstation/es_systems.cfg:**
When first run, an example systems configuration file will be created at `~/.emulationstation/es_systems.cfg`.  `~` is `$HOME` on Linux.  This example has some comments explaining how to write the configuration file. See the "Writing an es_systems.cfg" section for more information.

**Keep in mind you'll have to set up your emulator separately from EmulationStation!**

**~/.emulationstation/es_input.cfg:**
When you first start EmulationStation, you will be prompted to configure an input device. The process is thus:

1. Hold a button on the device you want to configure.  This includes the keyboard.

2. Press the buttons as they appear in the list.  Some inputs can be skipped by holding any button down for a few seconds (e.g. page up/page down).

3. You can review your mappings by pressing up and down, making any changes by pressing A.

4. Choose "SAVE" to save this device and close the input configuration screen.

The new configuration will be added to the `~/.emulationstation/es_input.cfg` file.

**Both new and old devices can be (re)configured at any time by pressing the Start button and choosing "CONFIGURE INPUT".**  From here, you may unplug the device you used to open the menu and plug in a new one, if necessary.  New devices will be appended to the existing input configuration file, so your old devices will remain configured.

**If your controller stops working, you can delete the `~/.emulationstation/es_input.cfg` file to make the input configuration screen re-appear on next run.**


You can use `--help` or `-h` to view a list of command-line options. Briefly outlined here:
```
--resolution [width] [height]   try and force a particular resolution
--gamelist-only                 skip automatic game search, only read from gamelist.xml
--ignore-gamelist               ignore the gamelist (useful for troubleshooting)
--draw-framerate                display the framerate
--no-exit                       don't show the exit option in the menu
--no-splash                     don't show the splash screen
--debug                         more logging, show console on Windows
--windowed                      not fullscreen, should be used with --resolution
--vsync [1/on or 0/off]         turn vsync on or off (default is on)
--max-vram [size]               Max VRAM to use in Mb before swapping. 0 for unlimited
--force-kid             Force the UI mode to be Kid
--force-kiosk           Force the UI mode to be Kiosk
--force-disable-filters         Force the UI to ignore applied filters in gamelist
--help, -h                      summon a sentient, angry tuba
```

As long as ES hasn't frozen, you can always press F4 to close the application.


Writing an es_systems.cfg
=========================

Complete configuration instructions at [emulationstation.org](http://emulationstation.org/gettingstarted.html#config).

The `es_systems.cfg` file contains the system configuration data for EmulationStation, written in XML.  This tells EmulationStation what systems you have, what platform they correspond to (for scraping), and where the games are located.

ES will check two places for an es_systems.cfg file, in the following order, stopping after it finds one that works:
* `~/.emulationstation/es_systems.cfg`
* `/etc/emulationstation/es_systems.cfg`

The order EmulationStation displays systems reflects the order you define them in.

**NOTE:** A system *must* have at least one game present in its "path" directory, or ES will ignore it! If no valid systems are found, ES will report an error and quit!

Here's an example es_systems.cfg:

```xml
<!-- This is the EmulationStation Systems configuration file.
All systems must be contained within the <systemList> tag.-->

<systemList>
	<!-- Here's an example system to get you started. -->
	<system>
		<!-- A short name, used internally. -->
		<name>snes</name>

		<!-- A "pretty" name, displayed in the menus and such. This one is optional. -->
		<fullname>Super Nintendo Entertainment System</fullname>

		<!-- The path to start searching for ROMs in. '~' will be expanded to $HOME or %HOMEPATH%, depending on platform.
		All subdirectories (and non-recursive links) will be included. -->
		<path>~/roms/snes</path>

		<!-- A list of extensions to search for, delimited by any of the whitespace characters (", \r\n\t").
		You MUST include the period at the start of the extension! It's also case sensitive. -->
		<extension>.smc .sfc .SMC .SFC</extension>

		<!-- The shell command executed when a game is selected. A few special tags are replaced if found in a command, like %ROM% (see below). -->
		<command>snesemulator %ROM%</command>
		<!-- This example would run the bash command "snesemulator /home/user/roms/snes/Super\ Mario\ World.sfc". -->

		<!-- The platform(s) to use when scraping. You can see the full list of accepted platforms in src/PlatformIds.cpp.
		It's case sensitive, but everything is lowercase. This tag is optional.
		You can use multiple platforms too, delimited with any of the whitespace characters (", \r\n\t"), eg: "genesis, megadrive" -->
		<platform>snes</platform>

		<!-- The theme to load from the current theme set. See THEMES.md for more information.
		This tag is optional; if not set, it will use the value of <name>. -->
		<theme>snes</theme>
	</system>
</systemList>
```

The following "tags" are replaced by ES in launch commands:

`%ROM%`		- Replaced with absolute path to the selected ROM, with most Bash special characters escaped with a backslash.

`%BASENAME%`	- Replaced with the "base" name of the path to the selected ROM. For example, a path of "/foo/bar.rom", this tag would be "bar". This tag is useful for setting up AdvanceMAME.

`%ROM_RAW%`	- Replaced with the unescaped, absolute path to the selected ROM.  If your emulator is picky about paths, you might want to use this instead of %ROM%, but enclosed in quotes.

See [SYSTEMS.md](SYSTEMS.md) for some live examples in EmulationStation.

gamelist.xml
============

The gamelist.xml file for a system defines metadata for games, such as a name, image (like a screenshot or box art), description, release date, and rating.

If at least one game in a system has an image specified, ES will use the detailed view for that system (which displays metadata alongside the game list).

The switch `--ignore-gamelist` can be used to ignore the gamelist and force ES to use the non-detailed view.

If you're writing a tool to generate or parse gamelist.xml files, you should check out [GAMELISTS.md](GAMELISTS.md) for more detailed documentation.


Themes
======

By default, EmulationStation looks pretty ugly. You can fix that. If you want to know more about making your own themes (or editing existing ones), read [THEMES.md](THEMES.md)!

I've put some themes up for download on my EmulationStation webpage: http://aloshi.com/emulationstation#themes

If you're using RetroPie, you should already have a nice set of themes automatically installed!


-Alec "Aloshi" Lofquist
http://www.aloshi.com
http://www.emulationstation.org

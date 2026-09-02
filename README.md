[![Linux build](https://github.com/VoxelStorm-Ltd/lander/actions/workflows/build-linux.yml/badge.svg?branch=master)](https://github.com/VoxelStorm-Ltd/lander/actions/workflows/build-linux.yml)
[![macOS build](https://github.com/VoxelStorm-Ltd/lander/actions/workflows/build-mac.yml/badge.svg?branch=master)](https://github.com/VoxelStorm-Ltd/lander/actions/workflows/build-mac.yml)
[![Windows build](https://github.com/VoxelStorm-Ltd/lander/actions/workflows/build-win.yml/badge.svg?branch=master)](https://github.com/VoxelStorm-Ltd/lander/actions/workflows/build-win.yml)

# Lander game prototype

This is a prototype, begun in 2013 and never originally finished or released, finally rediscovered and open-sourced in 2026.

It consists of an accurate orbital physics simulation of the entire solar system.  Your craft has started in orbit around the earth after an unspecified disaster; you have limited controls.

At the time of writing, most technology in this game is obsolete - it was written to a much earlier C++ standard using bare pointers, and uses very old OpenGL idioms that have been obsolete for a long time.  There is scope for a complete rewrite (perhaps AI-assisted) to put the core concepts to use in a modernised engine.

<img width="1280" height="983" alt="image" src="https://github.com/user-attachments/assets/ef671d54-af15-4bc0-a331-ec32858850c6" />

## Controls
- WASD to move around the cockpit
- Mouse to look around
- Mouse wheel to zoom in and out
- Left click on buttons, switches etc to activate them
- Right click on objects to pull up a menu (currently not implemented, shows basic information only)
- Arrow keys, `[` and `]`: rotate the ship in three dimensions

## Playing & development

The intention was to have a fully player-buildable cockpit; every device can be wired arbitrarily - instruments can be wired in to displays, buttons can be wired to actuators, etc.

There's also some clever trickery with radiant lighting in this demo.

The current cockpit demonstrates a rudimentary setup defined in init_universe.cpp, mostly to demonstrate some of what can be done.  Wiring is displayed visually as green lines between devices.

The initial cockpit setup consists primarily of:
- Four main displays:
  - CRT connected to the flight computer (currently showing boot and error log)
  - CRT connected to the star mapper (showing bodies and orbital trajectories from a "top-down" view, as well as ship orientation)
  - LCD connected to the external camera (you may not see an image on here if you're in the dark side of a planet or if you're not facing one - rotate the ship to experiment)
  - CRT connected to a digital-to-analogue conveter connected to the same camera
- The star mapper has several inputs, in the form of memory modules, which themselves are connected to displays and buttons:
  - zoom 7-segment display, and buttons to halve and double the zoom value - this adjusts the display zoom of the star mapper
  - reference body id 7-segment display, and buttons to increase or decrease the number of the refence body; this is the body in the solar system that the star mapper shows trajectories relative to.  The default is 4, i.e. Earth  Setting it to 5 will select the moon, etc.
- A little "on" switch which activates your thrusters when turned on - the main mechanic of the game at present.

The present state of the game allows you to do things like fly to the moon - a good challenge is to try to leave Earth and enter a stable circular orbit around the moon.  Read about the basics of [Hohmann transfer orbits](https://en.wikipedia.org/wiki/Hohmann_transfer_orbit) to get started.  Orient your ship using the arrow keys and fire the thruster with the "on" switch on the console, and use the star mapper to understand the effects of your actions.

## Building and running

Configure, build, and run a 64-bit Release build with:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_32BIT=OFF
cmake --build build --parallel
./build/Lander
```

To switch the same build directory to Debug mode, reconfigure it with `CMAKE_BUILD_TYPE=Debug` and rebuild:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_32BIT=OFF
cmake --build build --parallel
./build/Lander
```

`CMAKE_BUILD_TYPE` accepts `Debug` or `Release` and defaults to `Release` when omitted. `BUILD_32BIT` selects the target architecture: it defaults to the host architecture, `ON` enables `-m32` and the `lib/<platform>32` libraries, and `OFF` enables `-m64` and the `lib/<platform>64` libraries. For example, build 32-bit Release with:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_32BIT=ON
cmake --build build --parallel
./build/Lander
```

On Windows, run `build/Lander.exe` instead.

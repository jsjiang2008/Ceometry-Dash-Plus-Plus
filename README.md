Build command:
```
cmake -S . -B build
cmake --build build --target ceometry-dash -j 2
```

Run:
```
./build/ceometry-dash
```

Build & run:
```
cmake --build build --target ceometry-dash -j 2
./build/ceometry-dash
```

Commit history documents the progress that I made to build this.
# Ceometry Dash++

A C++20 and SDL3 platformer prototype. Hold Space or Up to jump; press R to
restart the current attempt.

Reach the green flag to complete the level. It is placed twelve grid cells
past the final object. A completion banner pauses gameplay; R starts a new
attempt. The first level uses singles, doubles, and broad one-cell steps.

## Build and run

```bash
cmake -S . -B build
cmake --build build --target ceometry-dash -j 2
./build/ceometry-dash
```

The default level is `levels/first.level` beside the executable. CMake copies
the project's `levels/` directory there whenever you build the game, so the
default works even when launched from another working directory.

To load a particular file, pass its path:

```bash
./build/ceometry-dash levels/first.level
```

Relative paths are relative to your terminal's working directory. With an
explicit source-file path, you can edit the level and relaunch immediately;
no rebuild is needed. R resets the attempt using the already-loaded objects;
it does not reread the file.

## Ship practice level

`levels/ship_test.level` is a short flight course with a wide corridor, four
alternating floor and ceiling pillars, and two floor spikes. Leave room to
brake: holding Space or Up accelerates upward, and releasing accelerates
downward, so the ship takes time to reverse direction.

Set `startingMode` in `src/app/main.cpp` to `PlayerMode::Ship`, then run:

```bash
cmake --build build --target ceometry-dash -j 2
./build/ceometry-dash levels/ship_test.level
```

The course takes about eleven seconds. Climb over the floor pillars and
descend below the ceiling pillars. Hitting a pillar's side, the roof's
underside, or a spike plays the death animation and restarts the attempt.
The floor and block tops remain safe landing surfaces. Press R to restart
manually; both restart paths restore the configured starting mode.

Reach the green flag to finish. This course uses the current prototype ship
physics and square graphic.

## Portal test level

```bash
cmake --build build --target ceometry-dash -j 2
./build/ceometry-dash levels/portal_test.level
```

This clear test area has a cube gate, a ship gate, and another cube gate.
Green gates select cube mode; blue gates select ship mode. Touching a gate
changes movement mode while preserving position and vertical momentum.
The mode label at the top left shows which controls are active.

Each gate activates once per attempt and turns gray after use. R and death
reset every gate and restore `startingMode`. The gates are not solid and
do not cause damage. Overlapping gates activate in file order if touched
in the same physics step. The finish flag also accounts for portal positions.

## Level format

Each nonempty line describes one object:

```text
type column row width height
```

- `type` is `block`, `spike`, `portal_cube`, or `portal_ship`.
- All four numbers are in grid cells; one cell is currently 40 pixels.
- `column` is the left edge, measured rightward from the level's origin.
- `row` is the bottom edge, measured upward from the floor. Row 0 rests on it.
- Width and height must be positive. Coordinates must be nonnegative.
- Decimal values such as `37.5` are allowed.
- Blank lines and `#` comments, including comments after an object, are allowed.

For example:

```text
# A two-cell-tall block and a spike resting on top of it.
block 10 0 1 2
spike 10 2 1 1

# Three adjacent spikes on the floor.
spike 20 0 1 1
spike 21 0 1 1
spike 22 0 1 1

# A ship portal, then a cube portal; each is one cell wide and three tall.
portal_ship 40 0 1 3
portal_cube 100 0 1 3
```

The loader converts these to pixels with:

```text
x = column * blockSize
y = floorY - (row + height) * blockSize
widthPixels = width * blockSize
heightPixels = height * blockSize
```

An invalid line stops loading and reports the file name, line number, and
reason in the terminal. The format describes blocks, spikes, and mode portals;
player physics, floor height, and scrolling speed remain configured in C++.

`include/Level.hpp` owns vectors of blocks, spikes, and portals, and `src/core/Level.cpp`
reads the file. Vectors allow different levels to have different object counts.

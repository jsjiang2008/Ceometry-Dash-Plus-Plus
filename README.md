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

## Level format

Each nonempty line describes one object:

```text
type column row width height
```

- `type` is `block` or `spike`.
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
```

The loader converts these to pixels with:

```text
x = column * blockSize
y = floorY - (row + height) * blockSize
widthPixels = width * blockSize
heightPixels = height * blockSize
```

An invalid line stops loading and reports the file name, line number, and
reason in the terminal. The format currently describes blocks and spikes;
player physics, floor height, and scrolling speed remain configured in C++.

`include/Level.hpp` owns vectors of blocks and spikes, and `src/core/Level.cpp`
reads the file. Vectors allow different levels to have different object counts.

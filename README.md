# Escape Game

A simple terminal‑based puzzle game written in C. The player must navigate a map, avoid traps and flooding water,
and reach the goal. The project includes an undo feature and coloured output for better visibility.

---

## Features

- Text‑based map rendered with borders and colours.
- Player movement using `w`, `a`, `s`, `d` keys.
- Trap mechanics: stepping on a trap (`@`) opens trapdoors (`X`) and starts flooding water (`~`).
- Flood spreads every move after trap activation; touching water ends the game.
- Undo support – press `u` to revert the last move.
- Map validation ensures required elements are present.

---

## Requirements

- **GNU Make** and a **C compiler** (tested with `gcc`).
- POSIX‑compatible terminal (ANSI escape codes used for colours).
- The project was developed on Linux/WSL. On Windows, run inside WSL or similar environment.

---

## Project Structure

```
color.c      color.h      gameFunc.c   gameFunc.h
genMap.c     genMap.h     linkedLists.c linkedLists.h
main.c       map.txt      newSleep.c   newSleep.h
terminal.c   terminal.h   Makefile
```

- **`main.c`** – entry point and game loop.
- **`genMap.*`** – map loading, display, validation, and state management.
- **`gameFunc.*`** – movement logic, flood/trap handling, undo helpers.
- **`linkedLists.*`** – simple linked list used for undo history.
- **`terminal.*`** – disable/enable line buffering for immediate character input.
- **`color.*`** – helper for setting ANSI colours.
- **`newSleep.*`** – cross‑platform sleep function.
- **`map.txt`** – example level text file.
- **`Makefile`** – build/run/clean targets.

---

## Map Format

The first line of the map file must contain two integers: number of rows and columns. Subsequent lines represent the grid, with each cell encoded as an integer:

| Value | Symbol | Description          |
|-------|--------|----------------------|
| `0`   | (space)| Empty tile           |
| `1`   | `O`    | Wall                 |
| `2`   | `~`    | Water (blue)         |
| `3`   | `X`    | Trapdoor             |
| `4`   | `@`    | Trap (red)           |
| `5`   | `P`    | Player start         |
| `6`   | `G`    | Goal (green)         |

The map **must** include at least one player, a goal, a trap, and some water; otherwise the program will refuse to run.

Example (`map.txt`):
```
15 25
0 0 0 ... 2
1 1 1 ... 1
... (rows of numbers)
```

---

## Building and Running

Open a terminal in the project directory and run:

```sh
# compile everything
make

# run using the example map
make run
```

The `run` target automatically cleans previous artifacts and executes the `escape` binary with `map.txt`.

Other useful targets:

```sh
make clean      # remove object files and executable
make valgrind   # run under valgrind to check for leaks
```

To use a different map file:

```sh
./escape mymap.txt
```

---

## Controls

- `w` – move up
- `s` – move down
- `a` – move left
- `d` – move right
- `u` – undo last move

Moves are processed immediately (no need to press Enter). The screen is cleared and redrawn each turn.

---

## How the Game Works

1. **Load & validate** the map from the given file.
2. **Record the initial game state** (player position, map contents).
3. Enter the main loop until **`gameResult`** becomes true:
   - Display current map with a border and control instructions.
   - Wait for a movement key or undo command.
   - If a regular move, copy the current state for undo and attempt the move:
     - Hitting a trap activates all trapdoors and begins flooding.
     - Reaching the goal ends the game with a win.
     - If flooding is active, water spreads and stepping into water causes a loss.
   - If undo requested, restore previous state from linked‑list history.
4. After exiting the loop, show win/lose message and clean up.

---

## Extending the Project

This codebase is small and modular; here are some ideas for enhancements:

- Add multiple levels and a level‑select menu.
- Implement keyboard input without ANSI sequences (e.g., arrow keys).
- Store undo history to a file for replaying.
- Create additional tile types (keys, doors, enemies).

---

## License & Credits

This assignment project was developed by **Bhagwat Ajaykumar Patel** for a university course.

Feel free to modify and reuse for educational purposes.

---

Happy escaping! 🧠🕳️🏃‍♂️

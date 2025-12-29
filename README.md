# Conway's Game of Life in C++

A terminal-based implementation of Conway's Game of Life using C++ and ncurses.

This version uses the ncurses library and was developed on a Raspberry Pi 2 running Raspbian.
It has also been verified on OS X El Capitan 10.11.4 through macOS Sequoia.

## Author

Rob Sweet

## Building

```bash
make
```

## Running

```bash
./life
```

## Controls

- `q` - Quit
- `r` - Restart with a new random pattern

## Dependencies

- g++
- ncurses library (`libncurses-dev` on Debian/Ubuntu, `ncurses` on macOS via Homebrew)

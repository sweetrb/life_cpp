# Conway's Game of Life in C++

A terminal-based implementation of Conway's Game of Life using C++ and ncurses.

This version uses the ncurses library and was developed on a Raspberry Pi 2 running Raspbian.
It has also been verified on OS X El Capitan 10.11.4 through macOS Sequoia.

## Why This Project?

I implement Conway's Game of Life in various programming languages as a benchmark for comparing:

- Runtime performance across languages
- Development overhead and complexity
- Language-specific idioms and patterns

This modest-sized application provides consistent insight into each language's strengths and characteristics.

## About Me

I am a software developer and IT leader with over 35 years of experience in the industry.

As partner and CEO of Superior Technologies Research (<www.superiortech.io>) in Houghton, MI, I lead a team of highly skilled developers working on diverse projects including large enterprise systems, cybersecurity solutions, and custom software development.

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

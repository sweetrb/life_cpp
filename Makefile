# makefile for 'life'

TARGET = life

C = life.cpp

# Without this the build is -O0, which nearly halves the generation rate and
# makes any timing comparison against the other implementations meaningless.
# (NDEBUG is already defined at the top of life.cpp, so the asserts are out.)
CFLAGS = -O2

# Windows/MSYS2 detection using shell command
UNAME := $(shell uname -s)
ifneq (,$(findstring MINGW,$(UNAME)))
	# Running on MSYS2/MinGW
	export PATH := /c/msys64/mingw64/bin:$(PATH)
	export TEMP := /tmp
	export TMP := /tmp
	COMP = g++
	INCLUDES = -I/c/msys64/mingw64/include/ncurses
	LIBS = -L/c/msys64/mingw64/lib -lncursesw
	TARGET = life.exe
else ifneq (,$(findstring MSYS,$(UNAME)))
	# Running on MSYS
	export PATH := /c/msys64/mingw64/bin:$(PATH)
	export TEMP := /tmp
	export TMP := /tmp
	COMP = g++
	INCLUDES = -I/c/msys64/mingw64/include/ncurses
	LIBS = -L/c/msys64/mingw64/lib -lncursesw
	TARGET = life.exe
else
	# Linux/Mac
	COMP = g++
	INCLUDES =
	LIBS = -lncurses
endif

all:
	$(COMP) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(C) $(LIBS)
ifneq (,$(findstring MINGW,$(UNAME)))
	@cp -f /c/msys64/mingw64/bin/libncursesw6.dll . 2>/dev/null || true
else ifneq (,$(findstring MSYS,$(UNAME)))
	@cp -f /c/msys64/mingw64/bin/libncursesw6.dll . 2>/dev/null || true
endif

clean:
	rm -f $(TARGET) *.out libncursesw6.dll


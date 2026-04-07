#!/bin/bash

# boot.sh
# Build script for OS Final Project
# Author: Wenda

# 1. Clear old /bin and /log
# 2. Create new /bin and /log
# 3. Compile logger.c
# 4. Compile remaining .c files in /src and print status
# 5. Run main_menu

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SRC="$SCRIPT_DIR/src"
BIN="$SCRIPT_DIR/bin"
LOGS="$SCRIPT_DIR/logs"

#1. Clear old /bin and /log
echo "Clearing old /bin and /logs folders"
rm -rf "$BIN" "$LOGS"

#2. Create new /bin and /log
echo "Creating bin and log folders"
mkdir "$BIN" "$LOGS"

#3. Compile Logger
echo "compiling logger..."
gcc -Wall -o "$BIN/logger" "$SRC/logger.c" || { echo "error compiling logger"; exit 1; }

#4. Compile remaining .c files in /src
echo "compiling peterson..."
#gcc -Wall -o "$BIN/peterson" "$SRC/peterson.c" || { echo "error compiling peterson"; exit 1; }

echo "compiling file_management..."
gcc -Wall -o "$BIN/file_management" "$SRC/file_management.c" || { echo "error compiling file_management"; exit 1; }

echo "compiling memory..."
gcc -Wall -o "$BIN/memory" "$SRC/memory.c" || { echo "error compiling memory"; exit 1; }

echo "compiling amdahl..."
gcc -Wall -o "$BIN/amdahl" "$SRC/amdahl.c" || { echo "error compiling amdahl"; exit 1; }

echo "compiling main_menu..."
gcc -Wall -o "$BIN/main_menu" "$SRC/main_menu.c" || { echo "error compiling main_menu"; exit 1; }


#5. Once compiled, run main menu
echo "All modules compiled sucessfullly!"
echo "Running main menu..."
cd "$BIN"
./main_menu


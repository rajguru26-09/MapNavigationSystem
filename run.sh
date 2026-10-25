#!/usr/bin/env bash
# Compile and run Map Navigation System
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
cd "$DIR"

echo "Compiling Smart Campus Map Navigation System..."
make -s

echo "Starting Navigation System..."
./bin/map_navigator

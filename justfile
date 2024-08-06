set shell := ["nu", "-c"]

default:
    @just --list

# cmake configure
configure:
    mkdir build
    cd build; cmake .. -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1

build:
    cd build; ninja

alias b := build

fresh-build: configure build

clean:
    cd build; rm -rfi *

run:
    cd build; ./bin/sees

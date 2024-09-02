set shell := ["nu", "-c"]

default:
    @just --list

alias c := configure
# cmake configure
configure:
    mkdir build
    cd build; cmake .. -GNinja -DCMAKE_BUILD_TYPE=Debug

# cmake configure
configure-release:
    mkdir build
    cd build; cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release

configure-with-qml-debug:
    mkdir build
    cd build; cmake .. -GNinja -DCMAKE_BUILD_TYPE=Debug -DENABLE_QML_DEBUG=1


alias b := build
build:
    cd build; ninja

fresh-build: configure build

clean:
    cd build; rm -rfi *

alias r := run
run:
    cd build; ./bin/sees

br: build run

run-with-trace $QML_IMPORT_TRACE="1":
    cd build; ./bin/sees


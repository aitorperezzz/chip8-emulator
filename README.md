# chip8-emulator
Chip8 emulator written in C++

## Introduction
This is an ongoing project that will implement a Chip 8 interpreter emulator, in C++, for a modern Linux environment. The choice of C++ mainly has to do
with performance.
The display is intended to be implemented in SDL.

This is open source software, please feel free to grab it and adapt it to your needs.

## What's there
* All math, jump and load instructions have been implemented and tested.
* There is a testing suite, that runs on CppUnit, that can unit test all instructions that have been implemented to date. All these checks can be easily run by
typing `make check`.
* A collection of known games written for Chip 8, in the `games` folder.

## What's not there yet
* The software is missing all instructions related to sprites, keyboard or display. The display will be implemented using SDL library soon.

## How can I run it
1. `git clone` this repo to an empty directory on a Linux system.
2. Install CppUnit to be able to run the unit tests. On Ubuntu, this can be done with `apt install cppunit-devel`.
3. Run `make` to check the program can compile correctly (C++17 standard is required).
4. Run `make check` to confirm the build produces the correct results.
5. TODO: choose one of the games provided in the `games` folder and run the application with `./chip8 <filename>`.

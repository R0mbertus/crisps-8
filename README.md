# crisps-8
crisps-8 is a [CHIP-8](https://en.wikipedia.org/wiki/CHIP-8) interpreter written in C++ using SDL2.  

## Status
It's very much a W.I.P. but a basic working chip-8 interpreter (i.e. working logic, display, keypad) is implemented. 
It's currently missing features such as sound or toggles for different behaviors for vaguely described opcodes.
As this is/was my first undertaking with doing a solo project there will be countless beginner mistakes present,
so beware and be kind :). As it currently stands there probably won't be much more progress, as it's pretty
much finished.

## Motivation
Coming from a university course that build a simple [IJVM](https://en.wikipedia.org/wiki/IJVM) interpreter, I 
wanted to continue practicing coding with a similar project. This project is more-so meant as a way to learn
a plethora of stuff aside from how CHIP-8 works, such as cmake, modern c++, good code style, etc. This does
cause the project to take more time as a lot of time is spent on those aforementioned subjects.

## Dependencies and Building
To build this program, you need:
* A C++ compiler like ```g++``` or `clang++`
* `cmake`
* `SDL2`  

For debian based system running `sudo apt install g++ cmake libsdl2 libsdl2-dev` should be sufficient.

## Running
To run the program, build with cmake and do `./build/crisps-8 <Display Scale> <Delay> <ROM>`  
Here, `<Display Scale>` is the scale of the window, `<Delay>` is the delay between frames in ms and
`<ROM>` is the path to a ROM, which in this project have been supplied in the `data/` directory

## Resources used
* https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
* https://github.com/mattmikolay/chip-8/wiki/Mastering-CHIP%E2%80%908
* https://austinmorlan.com/posts/chip8_emulator/
* http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

## Project features and progress
This is a list of subjects that keeps track of what has been implemented so far. If something has been made
then it'll be checked, otherwise it'll be unchecked. The list is divided in three parts: Setup, basic CHIP-8
features and possible future ideas. The latter is moreso a list of potential future projects than what will
be added on.
- [x] Build enviroment/Project setup
    - [x] Create a cmake using modern cmake
    - [x] Provide instructions on project install
    - [x] Write descriptive `README.md`
- [x] Basic CHIP-8 features
    - [x] Implement CHIP-8 state
    - [x] Implement CHIP-8 opcodes
    - [x] Get working graphical output
    - [x] Working responsive keypad
    - [ ] Working sound
- [ ] Future ideas
    - [ ] Support for ambigious instruction switching
    - [ ] SUPER-CHIP support
    - [ ] XO-CHIP support
    - [ ] Add package manager to elimanate need for manual package install
    - [ ] Adding a GUI with GUI niceties, such as file selector or debugger

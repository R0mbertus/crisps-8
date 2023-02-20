# crisps-8
crisps-8 is a [chip-8](https://en.wikipedia.org/wiki/CHIP-8) interpreter written in C++ using SDL2.  
This is still a W.I.P. but a basic working chip-8 interpreter (i.e. working logic, display, keypad) is implemented, but 
is currently still missing features such as sound or support for switching between different settings for ambigious 
instructions and might also have some bugs.  
This project tries to stick to the google c++ code style.

## Motivation
This has been a project of mine for about half a year, working on during the summer and then next to my uni courses when
the course load allows it. I also refactor the code due to just learning better practices, either from uni or just 
self-study.

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

## TODO list
The TODO list lists all the current tasks of the program, which will eather have been completed (checked) or still have
to be done (unchecked). The tasks related to only the basic chip=8 are fleshed out more with ideas of what to implement
next being listed at the bottom.  
- [x] Build enviroment
    - [x] Create a cmake using modern cmake
    - [x] Provide instruction on project install
- [x] Basic chip-8 features
    - [x] Implement registers and memory
    - [x] Implement Chip-8 instructions
    - [x] Working display using SDL2
    - [x] Working keypad using SDL2
- [ ] Advanced chip-8 features
    - [ ] Working sound
    - [ ] Support for ambigious instruction switching
    - [ ] Testing suite
- [ ] Future ideas (some might be better as seperate projects)
    - [ ] Add package manager to elimanate need for manual package install
    - [ ] Modular debug GUI to have a live visual view of the chip-8 registers
    - [ ] SUPER-CHIP support
    - [ ] XO-CHIP support


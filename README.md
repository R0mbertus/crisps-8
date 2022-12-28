# crisps-8
crisps-8 is a [chip-8](https://en.wikipedia.org/wiki/CHIP-8) interpreter written in C++ using SDL2.
This is still a W.I.P. but the basic interpretation (i.e. working logic, display, keypad) is implemented, but is still missing sound and support for switching between different settings for ambigious instructions.
This has been a little project of mine for about half a year, working on it when my course load wasn't too high.

## Dependencies and Building
To build this program, you need:
* A C++ compiler like ```g++``` or `clang++`
* `cmake`
* `SDL2`  

For debian based system running `sudo apt install g++ cmake libsdl2 libsdl2-dev` should be sufficient.

## Running
To run the program, build with cmake and do `./build/crisps-8 <Display Scale> <Delay> <ROM>`  
Here, `<Display Scale>` is the scale of the window,
`<Delay>` is the delay between frames and
`<ROM>` is the path to a ROM, which are supplied in the `data/` directory

## Resources used
* https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
* https://github.com/mattmikolay/chip-8/wiki/Mastering-CHIP%E2%80%908
* https://austinmorlan.com/posts/chip8_emulator/
* http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

## To-do list
- [x] Implement chip-8 logic
- [x] Add display
- [x] Get keypad working
- [ ] Adding support for ambigious instruction switching
- [ ] Adding sound
- [ ] GUI with info about current running program
- [ ] Add package manager to elimanate need for manual package install
- [ ] SUPER-CHIP support
- [ ] XO-CHIP support


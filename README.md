# 8080 Emulator

A bare bones Intel 8080 emulator, not much else to say! Just designed to play Space Invaders. 
The program expects there to be the Space Invader rom files invaders.h, invaders.g, invaders.f, and invaders.e in the root directory. These aren't included and must be sourced by the user.


### How it works
8080Emulator.cpp is where the main function is. I don't think I planned it that way or I would have named it main.cpp.
The whole program is basically a giant switch case statement that runs each opcode when it gets fed in. 
Most cycles are (if I remember right...):
  Get time/use time difference to run at correct frequency
  if running a cpu cycle
    Check for special flag (interrupt, etc)
    Read byte(s)
    Execute function

  if enough time passes to update display
    get input
    update display
    draw interface

Most of the weird flags are for debug interfaces (pausing, running individual frames, etc)

A proper state machine probably would have made bugs added by debug flags easier to avoid/fix, but it worked well enough.
I improved on it with another emulator, but I don't really remember how that one works either (This was all written a while ago)




### Build info
This was built in Visual Studio Code on Windows and compiled with the tools included in MinGW (g++ toolchain).

### UI and Graphics
The UI was programmed using OpenGL. It's basically a texture slapped onto a background with DearImgui for the menus. Most of the basic code was informed and/or copied from LearnOpenGl.
https://learnopengl.com/
https://www.dearimgui.com/

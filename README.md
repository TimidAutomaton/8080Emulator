# 8080 Emulator

A bare bones Intel 8080 emulator, not much else to say! 

This expects there to be the Space Invader rom files invaders.h, invaders.g, invaders.f, and invaders.e in the root directory. These aren't included and must be sourced by the user.

This was built in Visual Studio Code on Windows and compiled with the tools included in MinGW (g++ toolchain).

The UI was programmed using OpenGL. It's basically a texture slapped onto a background with DearImgui for the menus. Most of the basic code was informed and/or copied from LearnOpenGl.
https://learnopengl.com/
https://www.dearimgui.com/

#ifndef INPUTS_H
#define INPUTS_H

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
#define insertCoin      0
#define player2Start    1
#define player1Start    2

#define p1Shoot         4 
#define p1Left          5
#define p1Right         6

#define livesBit1       0
#define livesBit2       1
#define tiltControls    2
#define bonusLifeAmount 3
#define player2Shoot    4
#define player2Left     5
#define player2Right    6
#define displayCoin     7

#include "intel8080.h"
#include <GLFW/glfw3.h>

void processInput(GLFWwindow *window, intel8080 *cpu);

#endif
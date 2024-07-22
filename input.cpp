#include "input.h"

void processInput(GLFWwindow *window, intel8080 *cpu)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    //if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //    cpu->port[1] |= 
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        cpu->IOPorts[1] |= 1 << p1Left;
    }
    else{
        cpu->IOPorts[1] &= ~(1 << p1Left);
    }
    //if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //    glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        cpu->IOPorts[1] |= 1 << p1Right;
    }
    else{
        cpu->IOPorts[1] &= ~(1 << p1Right);
    }
        
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
        cpu->IOPorts[1] |= 1 << insertCoin;
    }
    else{
        cpu->IOPorts[1] &= ~(1 << insertCoin);
    }
        
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
         cpu->IOPorts[1] |= 1 << p1Shoot;
    }
    else{
        cpu->IOPorts[1] &= ~(1 << p1Shoot);
    }
        
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
         cpu->IOPorts[1] |= 1 << player1Start;
    }
    else{
        cpu->IOPorts[1] &= ~(1 << player1Start);
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
         cpu->IOPorts[2] |= 1 << tiltControls;
    }
    else{
        cpu->IOPorts[2] &= ~(1 << tiltControls);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
         cpu->IOPorts[2] |= 1 << player2Left;
    }
    else{
        cpu->IOPorts[2] &= ~(1 << player2Left);
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
         cpu->IOPorts[2] |= 1 << player2Right;
    }
    else{
        cpu->IOPorts[2] &= ~(1 << player2Right);
    }

    if (glfwGetKey(window, GLFW_KEY_KP_DECIMAL) == GLFW_PRESS){
         cpu->IOPorts[2] |= 1 << player2Start;
    }
    else{
        cpu->IOPorts[2] &= ~(1 << player2Start);
    }

    if (glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_PRESS){
         cpu->IOPorts[2] |= 1 << player2Shoot;
    }
    else{
        cpu->IOPorts[2] &= ~(1 << player2Shoot);
    }
        
}

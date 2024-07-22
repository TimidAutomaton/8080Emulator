//basic console output and strings
#include <iostream>
#include <string>

//OpenGL graphics library
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "intel8080.h"
#include "ALU.h"
#include "HexByte.h"
#include "GUI.h"

//#define UNICODE
//#define UNICODE_WAS_UNDEFINED
#include <windows.h>

SYSTEMTIME st;
FILETIME currentTimeSystem;
unsigned long long currentTime, lastGuiUpdate = 0, clockCycle = 0, invadersUpdate = 0, interruptTimer1 = 0, interruptTimer2 = 0;

void ISR(intel8080* cpu, int intaddress){
    PushRegisterPair(cpu, ((cpu->PC & 0xff00) >> 8), (cpu->PC & 0xff));
    cpu->PC = intaddress;
    cpu->interrupts = false;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, intel8080 *cpu);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


//interrupts
//two interrupts, both at 60hz
//one when the beam is half screen, the other is at end of screen
//when interrupt is triggered, disable interrupt
//push PC and then go to ISR 


float fps = 60.0; //frames per second
float period = 1.0/fps; //period of a frame

float scanlines = 256.0; //scanlines per screen
float freq = 2000000; //clock frequency

float cycleperscanline = (period/scanlines)*freq;

int main(){
    uint16_t programCount;

    GLFWwindow* window = InitGUI();

    unsigned int shaderProgram = InitializeShader();
    unsigned int VAO = InitializeVAO();

    char PauseOnLine[5];
    int PauseOnLineInt;

    bool breakpointActive;

    const int MAXINSTRUCTIONS = 1000;
    int currentInstruction = 0;
    int previousInstructions[MAXINSTRUCTIONS*4];// = new int [30];

    for (int i = 0; i < MAXINSTRUCTIONS*4; i++){
        previousInstructions[i] = 0;
    }

    intel8080* cpu = new intel8080();

    std::string file1 = "\\invaders.h";
    std::string file2 = "\\invaders.g";
    std::string file3 = "\\invaders.f";
    std::string file4 = "\\invaders.e";

    if (LoadRomFile(cpu, file1, file2, file3, file4) == 2){
        return -1;
    }                 

    bool notHalted = false;
    bool runOnce = false;
    bool runFrame = true;

    int opcode = 0;
    int firstByte = 0;
    int secondByte = 0;

    int totalCycles = 0;

    long long int oneInstructionCycle = 166666;

    unsigned long long timePerFrame = 166666;

    unsigned int texture;
    glGenTextures(1, &texture);  

    bool haltatInterrupt = true;

    bool midInterruptTriggered = false;
    bool cleared = false;
    uint8_t previous = 0;
    bool runNext = false;

    uint16_t previousCoin = 0;
    uint16_t previousSwitch = 0;

    
    // program loop
    while(!glfwWindowShouldClose(window)){
        // update times
        GetSystemTime(&st);
        bool test = SystemTimeToFileTime(&st, &currentTimeSystem);
        unsigned long long upper = currentTimeSystem.dwHighDateTime;
        unsigned long long lower = currentTimeSystem.dwLowDateTime;
        currentTime = (upper << 32) | lower;


        if(oneInstructionCycle <= 10000){
            oneInstructionCycle = 0;
        }


        // wait for next frame or instruction
        if (runFrame){
            if(((currentTime - invadersUpdate) >= timePerFrame) && notHalted){
                runNext = true;
                invadersUpdate = currentTime;
                totalCycles = cycleperscanline*scanlines;
            }
        }
        else{
            if(((currentTime - invadersUpdate) >= oneInstructionCycle) && notHalted){
                runNext = true;
                invadersUpdate = currentTime;
                totalCycles = cpu->cycles + 1;
            }
        }

        

        // if time for next frame, execute opcodes and ISRs
        if(runNext){
        //if(((currentTime - clockCycle) >= (unsigned long long) (oneInstructionCycle)) && notHalted){
            while((int(cpu->cycles) < totalCycles) && notHalted){

                runNext = false;

                PauseOnLineInt = HexToByte(PauseOnLine);
                if ((cpu->PC == PauseOnLineInt) && breakpointActive){
                    std::cout << "breakpoint" << std::endl;
                    notHalted = false;
                }

                if (breakpointActive){
                    std::cout << "breakpoint" << std::endl;
                    breakpointActive = false;
                    notHalted = false;
                }

                if((cpu->cyclesInterrupt >= cycleperscanline*(scanlines/2)) &&  !midInterruptTriggered && cpu->interrupts){
                    ISR(cpu, 0x08);
                    midInterruptTriggered = true;
                    cpu->cyclesInterrupt = 0;
                }
                if((cpu->cyclesInterrupt >= cycleperscanline*(scanlines/2)) && cpu->interrupts){
                    ISR(cpu, 0x10);
                    midInterruptTriggered = false;
                    cpu->cyclesInterrupt = 0;
                }

                ExecuteOpCode(cpu->memory[cpu->PC], cpu);
                clockCycle = currentTime;

                // log instructions
                previousInstructions[(currentInstruction * 4)] = cpu->PC;
                previousInstructions[(currentInstruction * 4) + 1] = cpu->memory[cpu->PC];
                previousInstructions[(currentInstruction * 4) + 2] = cpu->memory[cpu->PC + 1];
                previousInstructions[(currentInstruction * 4) + 3] = cpu->memory[cpu->PC + 2];

                currentInstruction +=1;

                if (currentInstruction >= MAXINSTRUCTIONS){
                    currentInstruction = 0;
                }
                
                if (runOnce){
                    notHalted = false;
                    runOnce = false;
                }
            }
            cpu->cycles = 0;
        }
        

        //render GUI and process inputs

        //currentTime >>= 
        if ((currentTime - lastGuiUpdate) >= (unsigned long long) ((10000000/20))){            
            processInput(window, cpu); //input

            // render
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            DrawScreen(cpu, shaderProgram, VAO, texture);
            
            ImGUIFrame(oneInstructionCycle, notHalted, runOnce, runFrame, cpu, PauseOnLine, breakpointActive, MAXINSTRUCTIONS, currentInstruction, previousInstructions);
    
            
            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();

            //std::cout << currentTime - lastGuiUpdate << std::endl;
            lastGuiUpdate = currentTime;
        }
        
    }

    GraphicsCleanup(VAO, shaderProgram);

    return 0;
}
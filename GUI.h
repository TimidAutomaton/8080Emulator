#ifndef GUI_H
#define GUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "intel8080.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui_memory_editor.h"

static MemoryEditor mem_edit;


GLFWwindow* InitGUI();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
unsigned int InitializeShader();
unsigned int InitializeVAO();

void ImGUIFrame(long long &oneInstructionCycle, bool &notHalted, bool &runOnce, bool &runFrame, intel8080* cpu, char* PauseOnLine, 
                    bool &breakpointActive, int MAXINSTRUCTIONS, int currentInstruction, int* previousInstructions);

void RenderDebugData(long long &oneInstructionCycle, bool &notHalted, bool &runOnce, bool &runFrame, intel8080* cpu, char* PauseOnLine, 
                    bool &breakpointActive, int MAXINSTRUCTIONS, int* previousInstructions);

void DrawScreen(intel8080* cpu, unsigned int shaderProgram, unsigned int VAO, unsigned int texture);


void GraphicsCleanup(unsigned int VAO, unsigned int shaderProgram);

#endif
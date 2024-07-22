
#include "GUI.h"

#include <iostream>
#include "shaderProgam.h"
//GUI headers
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui_memory_editor.h"



#include <glad/glad.h>
#include <GLFW/glfw3.h>


GLFWwindow* InitGUI(){
    //glewInit();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Space Invaders", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    //Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    return window;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


unsigned int InitializeShader(){
// build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

unsigned int InitializeVAO(){//float x, float y, float width, float height, float rotation){
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    //float vertices[] = {
    //    -0.5f, -0.5f, 0.0f, // left  
    //     0.5f, -0.5f, 0.0f, // right 
    //     0.0f,  0.5f, 0.0f  // top   
    //}; 

    float vertices[] = {
        //vertex x,y,z      //texture u, v
        0.2f,  0.9f,  0.0f,   1.0f, 0.0f, //top left t1
        0.9f,  0.9f,  0.0f,   1.0f, 1.0f, //top right t1
        0.2f, -0.52f, 0.0f,   0.0f, 0.0f, //bottom left t1
        0.2f, -0.52f, 0.0f,   0.0f, 0.0f, //bottom left t2
        0.9f,  0.9f,  0.0f,   1.0f, 1.0f, //top right t2
        0.9f, -0.52f, 0.0f,   0.0f, 1.0f  //bottom right t2
    };

/*
    float vertices[] = {
        //vertex x,y,z      //texture u, v
        -0.535f,    1.61f,  0.0f,   1.0f, 0.0f, //top left t1
         1.635f,    1.61f,  0.0f,   1.0f, 1.0f, //top right t1
        -0.535f,   -1.23f, 0.0f,   0.0f, 0.0f, //bottom left t1
        -0.535f,   -1.23f, 0.0f,   0.0f, 0.0f, //bottom left t2
         1.635f,    1.61f,  0.0f,   1.0f, 1.0f, //top right t2
         1.635f,   -1.23f, 0.0f,   0.0f, 1.0f  //bottom right t2
    };
*/
/*
    float vertices[] = {
        //vertex x,y,z      //texture u, v
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f, //top left t1
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f, //top right t1
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, //bottom left t1
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, //bottom left t2
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f, //top right t2
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f  //bottom right t2
    };
*/
   // glm::mat4 ortho = glm::mat4(1.0f);
    //ortho = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 100.0f);
    //glm::mat4 trans = glm::mat4(1.0f);
    //trans = glm::translate(trans, glm::vec3(x, y, 0.0f));
    //trans = glm::rotate(trans, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.0)); 

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 


    // uncomment this call to draw in wireframe polygons
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    return VAO;
}

void RenderDebugData(long long &oneInstructionCycle, bool &notHalted, bool &runOnce, bool &runFrame, intel8080* cpu, char* PauseOnLine, bool &breakpointActive, int MAXINSTRUCTIONS, int* previousInstructions){

    int programCount = cpu->PC;
    int opcode = cpu->memory[cpu->PC];
    int firstByte = cpu->memory[cpu->PC + 1];
    int secondByte = cpu->memory[cpu->PC + 2];
    // render your GUI
    ImGui::Begin("Register Info");

    ImGui::Text("Current Instruction: %02X", opcode);
    ImGui::Text("Next Byte: %02X %02X", firstByte, secondByte);

    ImGui::Text("Instruction: %s", DISSAMBLER_STATES[opcode]);

    ImGui::Text("Reg A: %02X", cpu->A);
    ImGui::Text("Reg B: %02X", cpu->B);
    ImGui::Text("Reg C: %02X", cpu->C);
    ImGui::Text("Reg D, E: %02X %02X", cpu->D, cpu->E);
    ImGui::Text("Reg H, L: %02X %02X", cpu->H, cpu->L);
    ImGui::Text("Reg SP: %04X", cpu->SP);
    ImGui::Text("Reg PC: %04X", cpu->PC);
    ImGui::Text("Reg Status: %XS %XZ %XAC %XP %XCS", cpu->sf, cpu->zf, cpu->acf, cpu->pf, cpu->cf);

    ImGui::Text("CycleTime: %d mS", (oneInstructionCycle / 10000));

    ImGui::Text("Pause on Line: ");
    ImGui::SameLine();
    ImGui::InputText("0x0000", PauseOnLine, 5);

    if (ImGui::Button("Breakpoint"))
    {
        std::cout << "Breakpoint" << std::endl;
        breakpointActive ^= 1;
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset"))
    {
        cpu->PC = 0;
        for (int address = 0x2000; address <= 0xFFFF; address++){
            cpu->memory[address] = 0;
        }

        for (int address = 0; address <= MAXINSTRUCTIONS; address++){
            previousInstructions[address] = 0;
        }
    }

    if (ImGui::Button("Start/Pause"))
    {
        //std::cout << "Breakpoint" << std::endl;
        notHalted ^= 1;
    }

    if (ImGui::Button("ISR Enable"))
    {
        //std::cout << "Breakpoint" << std::endl;
        cpu->interrupts ^= 1;
    }

    if (ImGui::Button("Next Code"))
    {
        //std::cout << "Breakpoint" << std::endl;
        notHalted = true;
        runOnce = true;
    }

    if (ImGui::Button("Cycle Time Up"))
    {
        //std::cout << "Breakpoint" << std::endl;
        oneInstructionCycle *= (unsigned long long)2;
        if (oneInstructionCycle == 0)
        {
            oneInstructionCycle = 10000000;
        }
    }

    if (ImGui::Button("Cycle Time Down"))
    {
        //std::cout << "Breakpoint" << std::endl;
        oneInstructionCycle /= (unsigned long long)2;
    }

    if (ImGui::Button("Frame/Instruction"))
    {
        //std::cout << "Breakpoint" << std::endl;
        runFrame ^= 1;
    }

    //ImGui::ShowDemoWindow();

    mem_edit.DrawWindow("Memory Edit", cpu->memory, 0xFFFF);

    ImGui::End();
}


//read vram and print to screen
//read ram
//create texture map from vram
//output to graphics system


void DrawScreen(intel8080* cpu, unsigned int shaderProgram, unsigned int VAO, unsigned int texture){
    int startVRAM = 0x2400;
    int endVRAM = 0x4000;

    int width = 224;
    int height = 256;

    

    //unsigned char *display = new unsigned char[(endVRAM - startVRAM)*24];
    unsigned char *display = new unsigned char[width*height*3];

    int VRAMLength = (endVRAM - startVRAM);

    for (int i = 0; i <= VRAMLength; i++){
        unsigned int currentByte = cpu->memory[startVRAM + i];
        for(int k = 0; k < 8; k++){
            int indexOffset = ((i * 8) + (k))*3;
            if(((currentByte >> k) & 0x01) == 0){
                display[indexOffset + 0] = 0x00;
                display[indexOffset + 1] = 0x00;
                display[indexOffset + 2] = 0x00;
            }
            else{
                display[indexOffset + 0] = 0x00;
                display[indexOffset + 1] = 0xC0;
                display[indexOffset + 2] = 0xC0;
            }
        }
    }
    
    glBindTexture(GL_TEXTURE_2D, texture);  

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, height, width, 0, GL_RGB, GL_UNSIGNED_BYTE, display);
    glGenerateMipmap(GL_TEXTURE_2D);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 6);

}

void ImGUIFrame(long long &oneInstructionCycle, bool &notHalted, bool &runOnce, bool &runFrame, intel8080* cpu, char* PauseOnLine, bool &breakpointActive, int MAXINSTRUCTIONS, int currentInstruction, int* previousInstructions){
    // feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    RenderDebugData(oneInstructionCycle, notHalted, runOnce, runFrame, cpu, PauseOnLine, breakpointActive, MAXINSTRUCTIONS, previousInstructions);

    ImGui::Begin("Last Instructions");

    for (int k = 0; k < MAXINSTRUCTIONS; k++)
    {
        int index = currentInstruction - k - 1;
        if (index < 0)
        {
            index = MAXINSTRUCTIONS + index;
        }
        //programCount = cpu->PC;
        ImGui::Text("%04X %02X %02X %02X     %s", previousInstructions[(index * 4)], previousInstructions[(index * 4) + 1], previousInstructions[(index * 4) + 2], previousInstructions[(index * 4) + 3], DISSAMBLER_STATES[previousInstructions[(index * 4) + 1]]);
    }

    ImGui::End();

    ImGui::Begin("Important Stuff");
    ImGui::Text("Credits: %02X", cpu->memory[0x20EB]);
    ImGui::Text("CoinSwitch: %02X", cpu->memory[0x20EA]);
    ImGui::Text("GameMode: %02X", cpu->memory[0x20EF]);
    ImGui::Text("Suspend: %02X", cpu->memory[0x20E9]);
    ImGui::Text("Port1: %02X", cpu->IOPorts[1]);
    ImGui::Text("Port2: %02X", cpu->IOPorts[2]);

    ImGui::End();

    // Render dear imgui into screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void GraphicsCleanup(unsigned int VAO, unsigned int shaderProgram){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}
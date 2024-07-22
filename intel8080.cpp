#include <cstdint>
#include <libloaderapi.h>
#include "intel8080.h"
#include "ALU.h"
#include <windows.h>
#include <timezoneapi.h>
#include <iostream>
#include <fstream>
#include <sstream>

////////////
/* A Flags              Program Status Word
 * B C                  B Register
 * D E                  D Register
 * H L                  H (indirect address)
 * SP                   Stack Pointer
 * PC                   Program Counter
 * 
 * 
 * S Z - AC - P - C     Flags in status Word
 */



intel8080::intel8080(){
    PC = 0;
    SP = 0;
    A = 0;
    B = 0;
    C = 0;
    sf = 0;
    zf = 0;
    acf = 0;
    pf = 0;
    cf = 0;
    interrupts = false;

    cycles = 0;
    cyclesInterrupt = 0;

    for(int i = 0; i < 8; i++){
        IOPorts[i] = 0;
    }

    for(int i = 0; i < 0xFFFF; i++){
        memory[i] = 0;
    }
}

void intel8080::WriteMem(uint16_t address, uint16_t value){
    //mirror ram in 0x2000-> and 0x4000->
    if(address >= 0x2000){
        uint16_t baseMemory = address % 0x2000;
        this->memory[baseMemory + 0x2000] = (value & 0xff);
        this->memory[baseMemory + 0x4000] = (value & 0xff);
    }
}

void intel8080::Set_SZP_Flags(uint16_t val){
    this->sf = val >> 7;
    this->zf = val == 0;
    this->pf = (val % 2) == 0;
}


int instruction = 0;

void ExecuteOpCode(uint8_t OpCode, intel8080* cpu){
    instruction += 1;
    cpu->PC += 1;
    switch (OpCode){
        case 0x00 : break;
        case 0x08 : break;
        case 0x10 : break;
        case 0x18 : break;
        case 0x20 : break;
        case 0x28 : break;
        case 0x30 : break;
        case 0x38 : break;

        case 0x01 : SetBC(cpu, AdvanceWord(cpu)); break;
        case 0x11 : SetDE(cpu, AdvanceWord(cpu)); break;
        case 0x21 : SetHL(cpu, AdvanceWord(cpu)); break;
        case 0x31 : cpu->SP = AdvanceWord(cpu); break;

        case 0x02 : WriteRegisterMemory(cpu, GetBC(cpu), cpu->A); break;
        case 0x12 : WriteRegisterMemory(cpu, GetDE(cpu), cpu->A); break;
        case 0x22 : SHLD(cpu); break;
        case 0x32 : STA(cpu); break;

        case 0x03 : INX(cpu, &cpu->B, &cpu->C); break;
        case 0x13 : INX(cpu, &cpu->D, &cpu->E); break;
        case 0x23 : INX(cpu, &cpu->H, &cpu->L); break;
        case 0x33 : cpu->SP = cpu->SP + 1; break;

        case 0x0B : DCX(cpu, &cpu->B, &cpu->C); break;
        case 0x1B : DCX(cpu, &cpu->D, &cpu->E); break;
        case 0x2B : DCX(cpu, &cpu->H, &cpu->L); break;
        case 0x3B : cpu->SP = cpu->SP - 1; break;

        case 0x04 : cpu->B = INR(cpu, cpu->B); break;
        case 0x14 : cpu->D = INR(cpu, cpu->D); break;
        case 0x24 : cpu->H = INR(cpu, cpu->H); break;
        //case 0x34 : cpu->memory[ReadMemoryHL(cpu)] = INR(cpu, cpu->memory[ReadMemoryHL(cpu)]); break;
        case 0x34 : cpu->WriteMem((cpu->H << 8) | (cpu->L & 0xFF), INR(cpu, ReadMemoryHL(cpu))); break;

        case 0x0C : cpu->C = INR(cpu, cpu->C); break;
        case 0x1C : cpu->E = INR(cpu, cpu->E); break;
        case 0x2C : cpu->L = INR(cpu, cpu->L); break;
        case 0x3C : cpu->A = INR(cpu, cpu->A); break;

        case 0x05 : cpu->B = DCR(cpu, cpu->B); break;
        case 0x15 : cpu->D = DCR(cpu, cpu->D); break;
        case 0x25 : cpu->H = DCR(cpu, cpu->H); break;
        //case 0x35 : cpu->memory[ReadMemoryHL(cpu)] = DCR(cpu, cpu->memory[ReadMemoryHL(cpu)]); break;
        case 0x35 : cpu->WriteMem((cpu->H << 8) | (cpu->L & 0xFF), DCR(cpu, ReadMemoryHL(cpu))); break;

        case 0x0D : cpu->C = DCR(cpu, cpu->C); break;
        case 0x1D : cpu->E = DCR(cpu, cpu->E); break;
        case 0x2D : cpu->L = DCR(cpu, cpu->L); break;
        case 0x3D : cpu->A = DCR(cpu, cpu->A); break;

        case 0x06 : cpu->B = AdvanceByte(cpu); break;
        case 0x16 : cpu->D = AdvanceByte(cpu); break;
        case 0x26 : cpu->H = AdvanceByte(cpu); break;
        case 0x36 : WriteMemoryHL(cpu, AdvanceByte(cpu)); break;

        case 0x0E : cpu->C = AdvanceByte(cpu); break;
        case 0x1E : cpu->E = AdvanceByte(cpu); break;
        case 0x2E : cpu->L = AdvanceByte(cpu); break;
        case 0x3E : cpu->A = AdvanceByte(cpu); break;

        case 0x07 : RLC(cpu); break;
        case 0x17 : RAL(cpu); break;
        case 0x0F : RRC(cpu); break;
        case 0x1F : RAR(cpu); break;

        case 0x0A : cpu->A = ReadRegisterMemory(cpu, cpu->B, cpu->C); break;
        case 0x1A : cpu->A = ReadRegisterMemory(cpu, cpu->D, cpu->E); break;
        case 0x2A : LHLD(cpu); break;
        case 0x3A : cpu->A = cpu->memory[AdvanceWord(cpu)]; break;

        case 0x27 : DAA(cpu); break;
        case 0x37 : cpu->cf = 1; break;
        case 0x2F : cpu->A ^= 0xFF; break;
        case 0x3F : cpu->cf ^= 1; break;

        case 0x09 : DAD(cpu, cpu->B, cpu->C); break;
        case 0x19 : DAD(cpu, cpu->D, cpu->E); break;
        case 0x29 : DAD(cpu, cpu->H, cpu->L); break;
        case 0x39 : DAD(cpu, cpu->SP); break;


        case 0x40 : cpu->B = cpu->B; break;
        case 0x41 : cpu->B = cpu->C; break;
        case 0x42 : cpu->B = cpu->D; break;
        case 0x43 : cpu->B = cpu->E; break;
        case 0x44 : cpu->B = cpu->H; break;
        case 0x45 : cpu->B = cpu->L; break;
        case 0x46 : cpu->B = ReadMemoryHL(cpu); break;
        case 0x47 : cpu->B = cpu->A; break;

        case 0x48 : cpu->C = cpu->B; break;
        case 0x49 : cpu->C = cpu->C; break;
        case 0x4A : cpu->C = cpu->D; break;
        case 0x4B : cpu->C = cpu->E; break;
        case 0x4C : cpu->C = cpu->H; break;
        case 0x4D : cpu->C = cpu->L; break;
        case 0x4E : cpu->C = ReadMemoryHL(cpu); break;
        case 0x4F : cpu->C = cpu->A; break;

        case 0x50 : cpu->D = cpu->B; break;
        case 0x51 : cpu->D = cpu->C; break;
        case 0x52 : cpu->D = cpu->D; break;
        case 0x53 : cpu->D = cpu->E; break;
        case 0x54 : cpu->D = cpu->H; break;
        case 0x55 : cpu->D = cpu->L; break;
        case 0x56 : cpu->D = ReadMemoryHL(cpu); break;
        case 0x57 : cpu->D = cpu->A; break;

        case 0x58 : cpu->E = cpu->B; break;
        case 0x59 : cpu->E = cpu->C; break;
        case 0x5A : cpu->E = cpu->D; break;
        case 0x5B : cpu->E = cpu->E; break;
        case 0x5C : cpu->E = cpu->H; break;
        case 0x5D : cpu->E = cpu->L; break;
        case 0x5E : cpu->E = ReadMemoryHL(cpu); break;
        case 0x5F : cpu->E = cpu->A; break;

        case 0x60 : cpu->H = cpu->B; break;
        case 0x61 : cpu->H = cpu->C; break;
        case 0x62 : cpu->H = cpu->D; break;
        case 0x63 : cpu->H = cpu->E; break;
        case 0x64 : cpu->H = cpu->H; break;
        case 0x65 : cpu->H = cpu->L; break;
        case 0x66 : cpu->H = ReadMemoryHL(cpu); break;
        case 0x67 : cpu->H = cpu->A; break;

        case 0x68 : cpu->L = cpu->B; break;
        case 0x69 : cpu->L = cpu->C; break;
        case 0x6A : cpu->L = cpu->D; break;
        case 0x6B : cpu->L = cpu->E; break;
        case 0x6C : cpu->L = cpu->H; break;
        case 0x6D : cpu->L = cpu->L; break;
        case 0x6E : cpu->L = ReadMemoryHL(cpu); break;
        case 0x6F : cpu->L = cpu->A; break;

        case 0x70 : WriteMemoryHL(cpu, cpu->B); break;
        case 0x71 : WriteMemoryHL(cpu, cpu->C); break;
        case 0x72 : WriteMemoryHL(cpu, cpu->D); break;
        case 0x73 : WriteMemoryHL(cpu, cpu->E); break;
        case 0x74 : WriteMemoryHL(cpu, cpu->H); break;
        case 0x75 : WriteMemoryHL(cpu, cpu->L); break;
        case 0x76 : cpu->halted = 1; break;
        case 0x77 : WriteMemoryHL(cpu, cpu->A); break;

        case 0x78 : cpu->A = cpu->B; break;
        case 0x79 : cpu->A = cpu->C; break;
        case 0x7A : cpu->A = cpu->D; break;
        case 0x7B : cpu->A = cpu->E; break;
        case 0x7C : cpu->A = cpu->H; break;
        case 0x7D : cpu->A = cpu->L; break;
        case 0x7E : cpu->A = ReadMemoryHL(cpu); break;
        case 0x7F : cpu->A = cpu->A; break;

        case 0x80 : AddReg(cpu, &cpu->A, cpu->B, 0); break;
        case 0x81 : AddReg(cpu, &cpu->A, cpu->C, 0); break;
        case 0x82 : AddReg(cpu, &cpu->A, cpu->D, 0); break;
        case 0x83 : AddReg(cpu, &cpu->A, cpu->E, 0); break;
        case 0x84 : AddReg(cpu, &cpu->A, cpu->H, 0); break;
        case 0x85 : AddReg(cpu, &cpu->A, cpu->L, 0); break;
        case 0x86 : AddReg(cpu, &cpu->A, ReadMemoryHL(cpu), 0); break;
        case 0x87 : AddReg(cpu, &cpu->A, cpu->A, 0); break;

        case 0x88 : AddReg(cpu, &cpu->A, cpu->B, cpu->cf); break;
        case 0x89 : AddReg(cpu, &cpu->A, cpu->C, cpu->cf); break;
        case 0x8A : AddReg(cpu, &cpu->A, cpu->D, cpu->cf); break;
        case 0x8B : AddReg(cpu, &cpu->A, cpu->E, cpu->cf); break;
        case 0x8C : AddReg(cpu, &cpu->A, cpu->H, cpu->cf); break;
        case 0x8D : AddReg(cpu, &cpu->A, cpu->L, cpu->cf); break;
        case 0x8E : AddReg(cpu, &cpu->A, ReadMemoryHL(cpu), cpu->cf); break;
        case 0x8F : AddReg(cpu, &cpu->A, cpu->A, cpu->cf); break;

        case 0x90 : SubReg(cpu, &cpu->A, cpu->B, 0); break;    
        case 0x91 : SubReg(cpu, &cpu->A, cpu->C, 0); break;    
        case 0x92 : SubReg(cpu, &cpu->A, cpu->D, 0); break;    
        case 0x93 : SubReg(cpu, &cpu->A, cpu->E, 0); break;    
        case 0x94 : SubReg(cpu, &cpu->A, cpu->H, 0); break;    
        case 0x95 : SubReg(cpu, &cpu->A, cpu->L, 0); break;    
        case 0x96 : SubReg(cpu, &cpu->A, ReadMemoryHL(cpu), 0); break;    
        case 0x97 : SubReg(cpu, &cpu->A, cpu->A, 0); break;    

        case 0x98 : SubReg(cpu, &cpu->A, cpu->B, cpu->cf); break;    
        case 0x99 : SubReg(cpu, &cpu->A, cpu->C, cpu->cf); break;    
        case 0x9A : SubReg(cpu, &cpu->A, cpu->D, cpu->cf); break;    
        case 0x9B : SubReg(cpu, &cpu->A, cpu->E, cpu->cf); break;    
        case 0x9C : SubReg(cpu, &cpu->A, cpu->H, cpu->cf); break;    
        case 0x9D : SubReg(cpu, &cpu->A, cpu->L, cpu->cf); break;    
        case 0x9E : SubReg(cpu, &cpu->A, ReadMemoryHL(cpu), cpu->cf); break;    
        case 0x9F : SubReg(cpu, &cpu->A, cpu->A, cpu->cf); break;    
            
        case 0xA0 : ANA(cpu, &cpu->A, cpu->B); break;
        case 0xA1 : ANA(cpu, &cpu->A, cpu->C); break;
        case 0xA2 : ANA(cpu, &cpu->A, cpu->D); break;
        case 0xA3 : ANA(cpu, &cpu->A, cpu->E); break;
        case 0xA4 : ANA(cpu, &cpu->A, cpu->H); break;
        case 0xA5 : ANA(cpu, &cpu->A, cpu->L); break;
        case 0xA6 : ANA(cpu, &cpu->A, ReadMemoryHL(cpu)); break;
        case 0xA7 : ANA(cpu, &cpu->A, cpu->A); break;
            
        case 0xA8 : XRA(cpu, &cpu->A, cpu->B); break;
        case 0xA9 : XRA(cpu, &cpu->A, cpu->C); break;
        case 0xAA : XRA(cpu, &cpu->A, cpu->D); break;
        case 0xAB : XRA(cpu, &cpu->A, cpu->E); break;
        case 0xAC : XRA(cpu, &cpu->A, cpu->H); break;
        case 0xAD : XRA(cpu, &cpu->A, cpu->L); break;
        case 0xAE : XRA(cpu, &cpu->A, ReadMemoryHL(cpu)); break;
        case 0xAF : XRA(cpu, &cpu->A, cpu->A); break;
            
        case 0xB0 : ORA(cpu, &cpu->A, cpu->B); break;
        case 0xB1 : ORA(cpu, &cpu->A, cpu->C); break;
        case 0xB2 : ORA(cpu, &cpu->A, cpu->D); break;
        case 0xB3 : ORA(cpu, &cpu->A, cpu->E); break;
        case 0xB4 : ORA(cpu, &cpu->A, cpu->H); break;
        case 0xB5 : ORA(cpu, &cpu->A, cpu->L); break;
        case 0xB6 : ORA(cpu, &cpu->A, ReadMemoryHL(cpu)); break;
        case 0xB7 : ORA(cpu, &cpu->A, cpu->A); break;

        case 0xB8 : CMP(cpu, cpu->A, cpu->B); break;
        case 0xB9 : CMP(cpu, cpu->A, cpu->C); break;
        case 0xBA : CMP(cpu, cpu->A, cpu->D); break;
        case 0xBB : CMP(cpu, cpu->A, cpu->E); break;
        case 0xBC : CMP(cpu, cpu->A, cpu->H); break;
        case 0xBD : CMP(cpu, cpu->A, cpu->L); break;
        case 0xBE : CMP(cpu, cpu->A, ReadMemoryHL(cpu)); break;
        case 0xBF : CMP(cpu, cpu->A, cpu->A); break;


        case 0xC2 : JNZ(cpu); break;
        case 0xD2 : JNC(cpu); break;
        case 0xE2 : JPO(cpu); break;
        case 0xF2 : JP(cpu); break;

        case 0xCA : JZ(cpu); break;
        case 0xDA : JC(cpu); break;
        case 0xEA : JPE(cpu); break;
        case 0xFA : JM(cpu); break;

        case 0xC3 : JMP(cpu); break;
        case 0xCB : JMP(cpu); break;

        case 0xC0 : RNZ(cpu); break;
        case 0xD0 : RNC(cpu); break;
        case 0xE0 : RPO(cpu); break;
        case 0xF0 : RP(cpu); break;

        case 0xC8 : RZ(cpu); break;
        case 0xD8 : RC(cpu); break;
        case 0xE8 : RPE(cpu); break;
        case 0xF8 : RM(cpu); break;

        case 0xC9 : RET(cpu); break;
        case 0xD9 : RET(cpu); break;

        case 0xC1 : PopRegisterPair(cpu, &cpu->B, &cpu->C); break;
        case 0xD1 : PopRegisterPair(cpu, &cpu->D, &cpu->E); break;
        case 0xE1 : PopRegisterPair(cpu, &cpu->H, &cpu->L); break;
        case 0xF1 : PopPSW(cpu); break;

        case 0xC5 : PushRegisterPair(cpu, cpu->B, cpu->C); break;
        case 0xD5 : PushRegisterPair(cpu, cpu->D, cpu->E); break;
        case 0xE5 : PushRegisterPair(cpu, cpu->H, cpu->L); break;
        case 0xF5 : PushPSW(cpu); break;

        case 0xC4 : CNZ(cpu); break;
        case 0xD4 : CNC(cpu); break;
        case 0xE4 : CPO(cpu); break;
        case 0xF4 : CP(cpu); break;

        case 0xCC : CZ(cpu); break;
        case 0xDC : CC(cpu); break;
        case 0xEC : CPE(cpu); break;
        case 0xFC : CM(cpu); break;

        case 0xCD : Call(cpu); break;
        case 0xDD : Call(cpu); break;
        case 0xED : Call(cpu); break;
        case 0xFD : Call(cpu); break;

        case 0xC7 : RST(cpu, 0); break;
        case 0xD7 : RST(cpu, 2); break;
        case 0xE7 : RST(cpu, 4); break;
        case 0xF7 : RST(cpu, 6); break;

        case 0xCF : RST(cpu, 1); break;
        case 0xDF : RST(cpu, 3); break;
        case 0xEF : RST(cpu, 5); break;
        case 0xFF : RST(cpu, 7);  break;

        //case 0xD3 : cpu->IOPort[AdvanceByte(cpu)] = cpu->A; break;
        //case 0xDB : cpu->A = cpu->IOPort[AdvanceByte(cpu)]; break;

        case 0xD3 : MachineOut(cpu); break;
        case 0xDB : MachineIn(cpu); break;

        case 0xC6 : ADI(cpu); break;
        case 0xD6 : SUI(cpu); break;
        case 0xE6 : ANI(cpu); break;
        case 0xF6 : ORI(cpu); break;

        case 0xCE : ACI(cpu); break;
        case 0xDE : SBI(cpu); break;
        case 0xEE : XRI(cpu); break;
        case 0xFE : CPI(cpu); break;

        case 0xE3 : XTHL(cpu); break;
        case 0xF3 : cpu->interrupts = 0; break;

        case 0xE9 : PCHL(cpu); break;
        case 0xF9 : SPHL(cpu); break;
        case 0xEB : XCHG(cpu); break;
        case 0xFB : StartISR(cpu); break; //cpu->interrupts = 1; break;

    }

    cpu->cyclesInterrupt += OPCODE_CYCLES[OpCode];
    cpu->cycles += OPCODE_CYCLES[OpCode];

}


void LoadRomtoMem(intel8080*& cpu, char*& ROMData, int ROMSize, uint16_t memoryStart){
    if(ROMSize == 0 ){return;}
    if(memoryStart > 0x2000){
        std::cout << "Error: out of Memory. Attempting to write outside of 8080's ROM." << std::endl;
        return;
    }

    if (ROMSize > 0x2000 || (ROMSize + memoryStart) > 0x2000){
        std::cout << "ROM file too large. File will be truncated." << std::endl;
    }

    for (int i = 0; i < ROMSize; i++){
        if(i >= 0x2000){ break; }
        ROMData[i];
        cpu->memory[memoryStart + i] = (uint8_t) ROMData[i];
    }

    std::cout << "ROM loaded into memory." << std::endl;
}


int SimpleOpenFile(std::string fileName, char*& fileContent){
    if (fileName == ""){return 1;}
    int size;
    std::ifstream file (fileName, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        fileContent = new char [size];
        file.seekg (0, std::ios::beg);
        file.read (fileContent, size);
        file.close();
        
    }
    else{
        std::cout << "File can't be opened" << std::endl;
        return 2;
    }
    char test = fileContent[3];
    return size;
}

int LoadRomFile(intel8080* cpu, std::string fileName1, std::string fileName2, std::string fileName3, std::string fileName4){
    
    char pBuf[256];
    size_t len = sizeof(pBuf); 
    int bytes = GetModuleFileName(NULL, pBuf, len);
    //PathCchRemoveFileSpec(pBuf, len);
    std::string exePath(pBuf);

    std::string exeDir;
    const size_t last_slash_idx = exePath.rfind('\\');
    if (std::string::npos != last_slash_idx)
    {
        exeDir = exePath.substr(0, last_slash_idx);
    }



/* more "proper" but couldn't get this to work */
    //LPWSTR exeDir;
    //DWORD length = GetModuleFileNameW(NULL, exeDir, MAX_PATH);
    //#if (NTDDI_VERSION >= NTDDI_WIN8)
      //  PathCchRemoveFileSpec(exeDir, MAX_PATH);
    //#else
        //if (MAX_PATH > destSize) return NULL;
        //PathRemoveFileSpec(dest);
    //#endif
    
    
    char* fileContent;
    int fileSize = 0;
    int totalSize = 0;
    fileSize = SimpleOpenFile(exeDir + fileName1, fileContent);
    if (fileSize == -1){
        return -1;
    }
    fileContent[1];
    LoadRomtoMem(cpu, fileContent, fileSize, 0);
    delete fileContent;
    totalSize += fileSize;

    if(fileName2 == ""){
        return 0;
    }
    fileSize = SimpleOpenFile(exeDir + fileName2, fileContent);
    LoadRomtoMem(cpu, fileContent, fileSize, totalSize);
    delete fileContent;
    totalSize += fileSize;

     if(fileName3 == ""){
        return 0;
    }

    fileSize = SimpleOpenFile(exeDir + fileName3, fileContent);
    LoadRomtoMem(cpu, fileContent, fileSize, totalSize);
    delete fileContent;
    totalSize += fileSize;

     if(fileName4 == ""){
        return 0;
    }

    fileSize = SimpleOpenFile(exeDir + fileName4, fileContent);
    LoadRomtoMem(cpu, fileContent, fileSize, totalSize);
    delete fileContent;
    return 0;
}

void StartISR(intel8080* cpu){
    cpu->interrupts = 1;
}
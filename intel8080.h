#ifndef INTEL8080_H
#define INTEL8080_H

#include <string>

class intel8080 {
public:
    uint8_t A, B, C, D, E, H, L;
    bool sf, zf, acf, pf, cf;
    bool halted, interrupts;

    uint32_t cycles, cyclesInterrupt;

    uint8_t IOPorts[8];
    uint16_t shiftRegister;
    uint8_t shiftOffset;

    uint16_t SP, PC;

    uint8_t memory[0xFFFF];

    intel8080();
    void WriteMem(uint16_t address, uint16_t value);
    void Set_SZP_Flags(uint16_t val);

};

void ExecuteOpCode(uint8_t OpCode, intel8080* cpu);

void LoadRomtoMem(intel8080*& cpu, char*& ROMData, int ROMSize, uint16_t memoryStart);
int SimpleOpenFile(std::string fileName, char*& fileContent);
int LoadRomFile(intel8080* cpu, std::string fileName1, std::string fileName2 = "", std::string fileName3 ="", std::string fileName4 = "");

void StartISR(intel8080* cpu);

static const uint8_t  OPCODE_CYCLES[256] = {
// x0  x1  x2  x3  x4  x5  x6  x7  x8  x9  xA  xB  xC  xD  xE  xF
    4, 10,  7,  5,  5,  5,  7,  4,  4, 10,  7,  5,  5,  5,  7,  4, //0x
    4, 10,  7,  5,  5,  5,  7,  4,  4, 10,  7,  5,  5,  5,  7,  4, //1x
    4, 10, 16,  5,  5,  5,  7,  4,  4, 10, 16,  5,  5,  5,  7,  4, //2x
    4, 10, 13,  5, 10, 10, 10,  4,  4, 10, 13,  5,  5,  5,  7,  4, //3x
    5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5, //4x
    5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5, //5x
    5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5, //6x
    7,  7,  7,  7,  7,  7,  5,  7,  5,  5,  5,  5,  5,  5,  7,  5, //7x
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, //8x
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, //9x
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, //Ax
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, //Bx 
    5, 10, 10, 10, 11, 11,  7, 11,  5, 10, 10, 10, 11, 17,  7, 11, //Cx 
    5, 10, 10, 10, 11, 11,  7, 11,  5, 10, 10, 10, 11, 17,  7, 11, //Dx
    5, 10, 10, 18, 11, 11,  7, 11,  5,  5, 10,  4, 11, 17,  7, 11, //Ex
    5, 10, 10,  4, 11, 11,  7, 11,  5,  5, 10,  4, 11, 17,  7, 11  //Fx
};

static const char* DISSAMBLER_STATES[] = {
//     x0               x1         x2         x3         x4         x5            x6     x7          x8           x9          xA         xB         xC         xD            xE     xF
    "NOP",     "LXI B,D16",    "STAX B",   "INX B",   "INR B",   "DCR B",     "MVI B,D8", "RLC",      "NOP",     "DAD B",   "LDAX B",   "DCX B",   "INR C",   "DCR C",    "MVI C,D8", "RRC",      //0x
    "NOP",     "LXI D,D16",    "STAX D",   "INX D",   "INR D",   "DCR D",     "MVI D,D8", "RAL",      "NOP",     "DAD D",   "LDAX D",   "DCX D",   "INR E",   "DCR E",    "MVI E,D8", "RAR",      //1x
    "NOP",     "LXI H,D16",    "SHLD A16", "INX H",   "INR H",   "DCR H",     "MVI H,D8", "DAA",      "NOP",     "DAD H",   "LHLD A16", "DCX H",   "INR L",   "DCR L",    "MVI L,D8", "RMA",      //2x
    "NOP",     "LXI SP,D16",   "STA A16",  "INX SP",  "INR M",   "DCR M",     "MVI M,D8", "STC",      "NOP",     "DAD SP",  "LDA A16",  "DCX SP",  "INR A",   "DCR A",    "MVI A,D8", "CMC",      //3x
    "MOV B,B", "MOV B,C",      "MOV B,D",  "MOV B,E", "MOV B,H", "MOV B,L",   "MOV B,M",   "MOV B,A", "MOV C,B", "MOV C,C", "MOV C,D",  "MOV C,E", "MOV C,H", "MOV C,L",  "MOV C,M",  "MOV C,A",  //4x
    "MOV D,B", "MOV D,C",      "MOV D,D",  "MOV D,E", "MOV D,H", "MOV D,L",   "MOV D,M",   "MOV D,A", "MOV E,B", "MOV E,C", "MOV E,D",  "MOV E,E", "MOV E,H", "MOV E,L",  "MOV E,M",  "MOV E,A",  //5x
    "MOV H,B", "MOV H,C",      "MOV H,D",  "MOV H,E", "MOV H,H", "MOV H,L",   "MOV H,M",   "MOV H,A", "MOV L,B", "MOV L,C", "MOV L,D",  "MOV L,E", "MOV L,H", "MOV L,L",  "MOV L,M",  "MOV L,A",  //6x
    "MOV M,B", "MOV M,C",      "MOV M,D",  "MOV M,E", "MOV M,H", "MOV M,L",   "HLT",       "MOV M,A", "MOV A,B", "MOV A,C", "MOV A,D",  "MOV A,E", "MOV A,H", "MOV A,L",  "MOV A,M",  "MOV A,A",  //7x
    "ADD B",   "ADD C",        "ADD D",    "ADD E",   "ADD H",   "ADD L",     "ADD M",     "ADD A",   "ADC B",   "ADC C",   "ADC D",    "ADC E",   "ADC H",   "ADC L",    "ADC M",    "ADC A",    //8x
    "SUB B",   "SUB C",        "SUB D",    "SUB E",   "SUB H",   "SUB L",     "SUB M",     "SUB A",   "SBB B",   "SBB C",   "SBB D",    "SBB E",   "SBB H",   "SBB L",    "SBB M",    "SBB A",    //9x
    "ANA B",   "ANA C",        "ANA D",    "ANA E",   "ANA H",   "ANA L",     "ANA M",     "ANA A",   "XRA B",   "XRA C",   "XRA D",    "XRA E",   "XRA H",   "XRA L",    "XRA M",    "XRA A",    //Ax
    "ORA B",   "ORA C",        "ORA D",    "ORA E",   "ORA H",   "ORA L",     "ORA M",     "ORA A",   "CMP B",   "CMP C",   "CMP D",    "CMP E",   "CMP H",   "CMP L",    "CMP M",    "CMP A",    //Bx
    "RNZ",     "POP B",        "JNZ A16",  "JMP A16", "CNZ A16", "PUSH B",    "ADI D8",    "RST 0",   "RZ",      "RET",     "JZ A16",   "JMP A16", "CZ A16",  "CALL A16", "ACI D8",   "RST 1",    //Cx
    "RNC",     "POP D",        "JNC A16",  "OUT D8",  "CNC A16", "PUSH D",    "AUI D8",    "RST 2",   "RC",      "RET",     "JC A16",   "IN  D8",  "CC A16",  "CALL A16", "SBI D8",   "RST 3",    //Dx
    "RPO",     "POP H",        "JPO A16",  "XTHL",    "CPO A16", "PUSH H",    "ANI D8",    "RST 4",   "RPE",     "PCHL",    "JPE A16",  "XCHG",    "CPE A16", "CALL A16", "XRI D8",   "RST 5",    //Ex
    "RP",      "POP PSW",      "JP  A16",  "DI",      "CP  A16", "PUSH PSW",  "ORI D8",    "RST 6",   "RM",      "SPHL",    "JM A16",   "EI",      "CM A16",  "CALL A16", "CPI D8",   "RST 7"     //Fx
};

#endif
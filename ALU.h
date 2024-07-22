#ifndef ALU_H
#define ALU_H

#include <stdint.h>
#include "intel8080.h"



 uint16_t ReadWord(intel8080* cpu, uint16_t pc);
 uint16_t AdvanceWord(intel8080* const cpu);
 uint8_t ReadByte(intel8080* cpu, uint16_t pc);
 uint8_t AdvanceByte(intel8080* const cpu);
 uint8_t ReadMemoryHL(intel8080* const cpu);
 void WriteMemoryHL(intel8080* const cpu, uint8_t val);
 uint16_t ReadRegisterMemory(intel8080* const cpu, uint8_t a, uint8_t b);
 void WriteRegisterMemory(intel8080* const cpu, uint16_t memoryAddress, uint8_t val);
 uint16_t GetBC(intel8080* const cpu);
 uint16_t GetDE(intel8080* const cpu);
 void SetBC(intel8080* const cpu, uint16_t value);
 void SetDE(intel8080* const cpu, uint16_t value);
 void SetHL(intel8080* const cpu, uint16_t value);
 bool FullCarry(uint8_t a, uint8_t b, bool cf);
 bool HalfCarry(uint8_t a, uint8_t b, bool cf);
 void AddReg(intel8080* cpu, uint8_t* reg, uint8_t val, bool cf);
 void SubReg(intel8080* cpu, uint8_t* reg, uint8_t val, bool cf);
 void INX(intel8080* cpu, uint8_t* a, uint8_t* b);
 void DCX(intel8080* cpu, uint8_t* rega, uint8_t* regb);
 uint8_t INR(intel8080* cpu, uint8_t registerVal);
 uint8_t DCR(intel8080* cpu, uint8_t registerVal);
 void ANA(intel8080* cpu, uint8_t* reg, uint8_t val);
 void XRA(intel8080* cpu, uint8_t* reg, uint8_t val);
 void ORA(intel8080* cpu, uint8_t* reg, uint8_t val);
 void CMP(intel8080* cpu, uint8_t reg, uint8_t val);
 void SHLD(intel8080* cpu);
 void STA(intel8080* cpu);
 void DAD(intel8080* cpu, uint8_t rega, uint8_t regb);
 void DAD(intel8080* cpu, uint16_t SP);
 void RLC(intel8080* cpu);
 void RAL(intel8080* cpu);
 void RRC(intel8080* cpu);
 void RAR(intel8080* cpu);
 void DAA(intel8080* cpu);
 void LHLD(intel8080* cpu);
 void JMP(intel8080* cpu);
 void JNZ(intel8080* cpu);
 void JNC(intel8080* cpu);
 void JPO(intel8080* cpu);
 void JP(intel8080* cpu);
 void JZ(intel8080* cpu);
 void JC(intel8080* cpu);
 void JPE(intel8080* cpu);
 void JM(intel8080* cpu);
 void RET(intel8080* cpu);
 void RNZ(intel8080* cpu);
 void RNC(intel8080* cpu);
 void RPO(intel8080* cpu);
 void RP(intel8080* cpu);
 void RZ(intel8080* cpu);
 void RC(intel8080* cpu);
 void RPE(intel8080* cpu);
 void RM(intel8080* cpu);
 void PushRegisterPair(intel8080* cpu, uint8_t HReg, uint8_t LReg);
 void PopRegisterPair(intel8080* cpu, uint8_t* HReg, uint8_t* LReg);
 void PushPSW(intel8080* cpu);
 void PopPSW(intel8080* cpu);
 void Call(intel8080* cpu);
 void CNZ(intel8080* cpu);
 void CNC(intel8080* cpu);
 void CPO(intel8080* cpu);
 void CP(intel8080* cpu);
 void CZ(intel8080* cpu);
 void CC(intel8080* cpu);
 void CPE(intel8080* cpu);
 void CM(intel8080* cpu);
 void RST(intel8080* cpu, uint8_t N);
 void ADI(intel8080* cpu);
 void ACI(intel8080* cpu);
 void SUI(intel8080* cpu);
 void SBI(intel8080* cpu);
 void ANI(intel8080* cpu);
 void ORI(intel8080* cpu);
 void XRI(intel8080* cpu);
 void CPI(intel8080* cpu);
 void XTHL(intel8080* cpu);
 void PCHL(intel8080* cpu);
 void SPHL(intel8080* cpu);
 void XCHG(intel8080* cpu);
 void MachineIn(intel8080* cpu);
 void MachineOut(intel8080* cpu);

#endif
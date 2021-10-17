#pragma once
#include <iostream>
class VirtualMachine
{

public:
	VirtualMachine();
	~VirtualMachine();
	//cpy ctr
	VirtualMachine(const VirtualMachine& old) = delete;
	//move ctr
	VirtualMachine(VirtualMachine&& old) = delete;
	VirtualMachine& operator=(const VirtualMachine& other) = delete;
	VirtualMachine& operator=(const VirtualMachine&& other) = delete;


private:

	//ScreenSize (native 64 x 32)
	const uint16_t m_ScreenWidth;
	const uint16_t m_ScreenHeight;
	//monochrome --> pixel has 0 or 1 state
	const static uint16_t m_TotalPixelCount{ 0x800 };
	bool m_PixelArray[m_TotalPixelCount];

	const static uint16_t m_MemSize{ 0xFFF };
	uint8_t m_Memory[m_MemSize];
	//Most CHIP-8 programs start at location 0x200, everything below is for interpreter
	const uint8_t m_ProgramMemStart{ 0x200 };

	//REGISTERS
	const static uint16_t m_RegistersAmount{ 0xFFF };
	//general purpose registers 0 to F
	uint8_t m_Vx[16];
	//used to store memory addresses
	uint8_t m_Vi;
	//Program counter, holds currently executed address
	uint16_t m_PC;
	//Stack pointer, points to upmost level of the stack
	uint8_t m_SP;

	//special purpose sound and time registers
	//delay timer register
	uint8_t m_DT;
	//sound timer register
	uint8_t m_ST;

	//used to store the address that the interpreter shoud return to when finished with a subroutine. Chip-8 allows for up to 16 levels of nested subroutines.
	uint16_t m_Stack[16];

	

	//INPUT
	uint8_t m_Input[16];
};


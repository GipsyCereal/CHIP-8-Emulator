#pragma once
#include <iostream>
#include <SDL.h>
namespace InstructionLib { class OpcodeManager; }
class InstructionLib::OpcodeManager;
class VirtualMachine
{
	struct Vector2 { float width, height; };
public:
	VirtualMachine(const int& widthScale , const int& heightScale);
	~VirtualMachine();
	//cpy ctr
	VirtualMachine(const VirtualMachine& old) = delete;
	//move ctr
	VirtualMachine(VirtualMachine&& old) = delete;
	VirtualMachine& operator=(const VirtualMachine& other) = delete;
	VirtualMachine& operator=(const VirtualMachine&& other) = delete;

	void LoadROM(const std::string& path);
	void ClearScreen();
	void Update(const float elapsedSec);
	bool ProcessInput();

	const static uint16_t m_TotalPixelCount{ 0x800 };
	//monochrome --> pixel has 0 or 1 state
	uint32_t m_PixelArray[m_TotalPixelCount];

	uint8_t GetSP() const { return m_SP; }
	void SetSP(const uint8_t sp) { m_SP = sp; }
	void DecrementSP() { --m_SP; }
	void IncrementSP() { ++m_SP; }

	uint16_t GetPC() const { return m_PC; }
	void SetPC(const uint16_t& memAddress) { m_PC = memAddress; }
	void IncrementPCByTwo() { m_PC += 2; }
	void DecrementPCByTwo() { m_PC -= 2; }

	//used to store the address that the interpreter shoud return to when finished with a subroutine. Chip-8 allows for up to 16 levels of nested subroutines.
	uint16_t m_Stack[16];

	//REGISTERS
	//general purpose registers 0 to F
	uint8_t m_Vx[16];
	//used to store memory addresses
	uint16_t m_Vi;

	const static uint16_t m_MemSize{ 0x1000 };
	uint8_t m_Memory[m_MemSize];

	Vector2 m_ScreenDimensions;

	//INPUT
	uint8_t m_Input[16];

	//special purpose sound and time registers
	//delay timer register
	uint8_t m_DT;
	//sound timer register
	uint8_t m_ST;

	//Stack pointer, points to upmost level of the stack
	uint8_t m_SP;

	bool m_IsPaused;
private:
	//METHODS
	void Init(const int& widthScale, const int& heightScale);
	void InitSDL(const int& widthScale, const int& heightScale);
	void InitFont();

	void UpdateApp(const float elapsedSec);
	//ScreenSize (native 64 x 32)
	const uint16_t m_TextureWidth;
	const uint16_t m_TextureHeight;
	//Most CHIP-8 programs start at location 0x200, everything below is for interpreter
	const uint16_t m_ProgramMemStart{ 0x200 };

	//Program counter, holds currently executed address
	uint16_t m_PC;

	InstructionLib::OpcodeManager* m_pOpcodeManager;

	//SDL
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
	SDL_Texture* m_Texture;
};


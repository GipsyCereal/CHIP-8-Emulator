#include "VirtualMachine.h"
#include "InstructionLib.h"
#include <SDL_main.h>
#include <SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>
VirtualMachine::VirtualMachine(const int& width, const int& height)
	:m_TextureHeight{ 32 }
	, m_TextureWidth{ 64 }
	, m_Vi{}
	, m_Vx{}
	, m_DT{}
	, m_ST{}
	, m_Input{}
	, m_Memory{}
	, m_PC{}
	, m_Stack{}
	, m_PixelArray{}
	, m_ScreenDimensions{}
{
	Init(width, height);
}

VirtualMachine::~VirtualMachine()
{
	delete m_pOpcodeManager;
	m_pOpcodeManager = nullptr;
	SDL_DestroyTexture(m_Texture);
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

void VirtualMachine::Init(const int& widthScale, const int& heightScale)
{
	
	InitSDL(widthScale, heightScale);
	InitFont();
	m_PC = m_ProgramMemStart;
	m_pOpcodeManager = new InstructionLib::OpcodeManager();
	m_ScreenDimensions.width = m_TextureWidth;
	m_ScreenDimensions.height = m_TextureHeight;
}

void VirtualMachine::InitSDL(const int& widthScale, const int& heightScale)
{
	const int scaledWidth{ widthScale * m_TextureWidth };
	const int scaledHeight{ heightScale * m_TextureHeight };
	SDL_Init(SDL_INIT_VIDEO);
	m_Window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, scaledWidth, scaledHeight, SDL_WINDOW_SHOWN);
	//ACCELERATED --> Uses hardware
	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
	m_Texture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, m_TextureWidth, m_TextureHeight);
}

void VirtualMachine::InitFont()
{
	//http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#3.1
	// 80 bytes are font loaded in interpreter memory (interpreter are 1st 512 bytes --> 0x000 -> 0x200
	m_Memory[0x050] = 0xF0;
	m_Memory[0x051] = 0x90;
	m_Memory[0x052] = 0x90;
	m_Memory[0x053] = 0x90;
	m_Memory[0x054] = 0xF0;
	// 1
	m_Memory[0x055] = 0x20;
	m_Memory[0x056] = 0x60;
	m_Memory[0x057] = 0x20;
	m_Memory[0x058] = 0x20;
	m_Memory[0x059] = 0x70;
	// 2
	m_Memory[0x05A] = 0xF0;
	m_Memory[0x05B] = 0x10;
	m_Memory[0x05C] = 0xF0;
	m_Memory[0x05D] = 0x80;
	m_Memory[0x05E] = 0xF0;
	// 3
	m_Memory[0x05F] = 0xF0;
	m_Memory[0x060] = 0x10;
	m_Memory[0x061] = 0xF0;
	m_Memory[0x062] = 0x10;
	m_Memory[0x063] = 0xF0;
	// 4
	m_Memory[0x064] = 0x90;
	m_Memory[0x065] = 0x90;
	m_Memory[0x066] = 0xF0;
	m_Memory[0x067] = 0x10;
	m_Memory[0x068] = 0x10;
	// 5
	m_Memory[0x069] = 0xF0;
	m_Memory[0x06A] = 0x80;
	m_Memory[0x06B] = 0xF0;
	m_Memory[0x06C] = 0x10;
	m_Memory[0x06D] = 0xF0;
	// 6
	m_Memory[0x06E] = 0xF0;
	m_Memory[0x06F] = 0x80;
	m_Memory[0x070] = 0xF0;
	m_Memory[0x071] = 0x90;
	m_Memory[0x072] = 0xF0;
	// 7
	m_Memory[0x073] = 0xF0;
	m_Memory[0x074] = 0x10;
	m_Memory[0x075] = 0x20;
	m_Memory[0x076] = 0x40;
	m_Memory[0x077] = 0x40;
	// 8
	m_Memory[0x078] = 0xF0;
	m_Memory[0x079] = 0x90;
	m_Memory[0x07A] = 0xF0;
	m_Memory[0x07B] = 0x90;
	m_Memory[0x07C] = 0xF0;
	// 9
	m_Memory[0x07D] = 0xF0;
	m_Memory[0x07E] = 0x90;
	m_Memory[0x07F] = 0xF0;
	m_Memory[0x080] = 0x10;
	m_Memory[0x081] = 0xF0;
	// A
	m_Memory[0x082] = 0xF0;
	m_Memory[0x083] = 0x90;
	m_Memory[0x084] = 0xF0;
	m_Memory[0x085] = 0x90;
	m_Memory[0x086] = 0x90;
	// B
	m_Memory[0x087] = 0xE0;
	m_Memory[0x088] = 0x90;
	m_Memory[0x089] = 0xE0;
	m_Memory[0x08A] = 0x90;
	m_Memory[0x08B] = 0xE0;
	// C
	m_Memory[0x08C] = 0xF0;
	m_Memory[0x08D] = 0x80;
	m_Memory[0x08E] = 0x80;
	m_Memory[0x08F] = 0x80;
	m_Memory[0x090] = 0xF0;
	// D
	m_Memory[0x091] = 0xE0;
	m_Memory[0x092] = 0x90;
	m_Memory[0x093] = 0x90;
	m_Memory[0x094] = 0x90;
	m_Memory[0x095] = 0xE0;
	// E
	m_Memory[0x096] = 0xF0;
	m_Memory[0x097] = 0x80;
	m_Memory[0x098] = 0xF0;
	m_Memory[0x099] = 0x80;
	m_Memory[0x09A] = 0xF0;
	// F
	m_Memory[0x09B] = 0xF0;
	m_Memory[0x09C] = 0x80;
	m_Memory[0x09D] = 0xF0;
	m_Memory[0x09E] = 0x80;
	m_Memory[0x09F] = 0x80;
}

void VirtualMachine::UpdateApp(const float elapsedSec)
{
	if (m_PC >= m_MemSize)
	{
		std::cerr << "PC encountered an overflow" << std::endl;
		return;
	}

	if (m_DT > 0)
		--m_DT;
	if (m_ST > 0)
		--m_ST;

	// Construct opcode from the 2 bytes divided over PC and PC + 1
	//shift left cuz higher order
	const uint16_t opcode = (m_Memory[m_PC] << 8u) | m_Memory[m_PC + 1];
	m_PC += 2;
	m_pOpcodeManager->ExecuteOpcode(*this, opcode);

}


void VirtualMachine::LoadROM(const std::string& path)
{
	// Open the file as a stream of binary and move the file pointer to the end
	std::ifstream file(path, std::ios::binary | std::ios::in);

	if (!file.good())
	{
		std::cerr << "Rom not found" << std::endl;
		return;
	}
	else if (!file.is_open())
	{
		std::cerr << "Cant open ROM" << std::endl;
		return;
	}

	int counter{ 0 };
	//read byte by byte
	while (file)
	{
		char byte;
		file.read((char*)&byte, sizeof(byte));
		m_Memory[m_ProgramMemStart + counter] = byte;
		++counter;
	}
}
void VirtualMachine::ClearScreen()
{
	//Clear everything from prev frame render
	SDL_RenderClear(m_Renderer);
	SDL_RenderPresent(m_Renderer);
	SDL_SetRenderDrawColor(m_Renderer, 1, 1, 1, 1);

}
void VirtualMachine::Update(const float elapsedSec)
{
	UpdateApp(elapsedSec);
	int pitch = m_TextureWidth * sizeof(uint32_t);
	SDL_UpdateTexture(m_Texture, nullptr, &m_PixelArray, pitch);
	//copy this frame texture into renderer
	SDL_RenderClear(m_Renderer);
	SDL_RenderCopy(m_Renderer, m_Texture, nullptr, nullptr);
	//present renderer
	SDL_RenderPresent(m_Renderer);
}

bool VirtualMachine::ProcessInput()
{
	bool quit = false;

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
		{
			quit = true;
		} break;

		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
			{
				quit = true;
			} break;

			case SDLK_x:
			{
				m_Input[0] = 1;
			} break;

			case SDLK_1:
			{
				m_Input[1] = 1;
			} break;

			case SDLK_2:
			{
				m_Input[2] = 1;
			} break;

			case SDLK_3:
			{
				m_Input[3] = 1;
			} break;

			case SDLK_q:
			{
				m_Input[4] = 1;
			} break;

			case SDLK_w:
			{
				m_Input[5] = 1;
			} break;

			case SDLK_e:
			{
				m_Input[6] = 1;
			} break;

			case SDLK_a:
			{
				m_Input[7] = 1;
			} break;

			case SDLK_s:
			{
				m_Input[8] = 1;
			} break;

			case SDLK_d:
			{
				m_Input[9] = 1;
			} break;

			case SDLK_z:
			{
				m_Input[0xA] = 1;
			} break;

			case SDLK_c:
			{
				m_Input[0xB] = 1;
			} break;

			case SDLK_4:
			{
				m_Input[0xC] = 1;
			} break;

			case SDLK_r:
			{
				m_Input[0xD] = 1;
			} break;

			case SDLK_f:
			{
				m_Input[0xE] = 1;
			} break;

			case SDLK_v:
			{
				m_Input[0xF] = 1;
			} break;
			}
		} break;

		case SDL_KEYUP:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_x:
			{
				m_Input[0] = 0;
			} break;

			case SDLK_1:
			{
				m_Input[1] = 0;
			} break;

			case SDLK_2:
			{
				m_Input[2] = 0;
			} break;

			case SDLK_3:
			{
				m_Input[3] = 0;
			} break;

			case SDLK_q:
			{
				m_Input[4] = 0;
			} break;

			case SDLK_w:
			{
				m_Input[5] = 0;
			} break;

			case SDLK_e:
			{
				m_Input[6] = 0;
			} break;

			case SDLK_a:
			{
				m_Input[7] = 0;
			} break;

			case SDLK_s:
			{
				m_Input[8] = 0;
			} break;

			case SDLK_d:
			{
				m_Input[9] = 0;
			} break;

			case SDLK_z:
			{
				m_Input[0xA] = 0;
			} break;

			case SDLK_c:
			{
				m_Input[0xB] = 0;
			} break;

			case SDLK_4:
			{
				m_Input[0xC] = 0;
			} break;

			case SDLK_r:
			{
				m_Input[0xD] = 0;
			} break;

			case SDLK_f:
			{
				m_Input[0xE] = 0;
			} break;

			case SDLK_v:
			{
				m_Input[0xF] = 0;
			} break;
			}
		} break;
		}
	}

	return quit;
}
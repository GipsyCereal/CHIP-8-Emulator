#pragma once
#include "VirtualMachine.h"
#include <functional>
#include <vector>
#include <cstdint>

namespace InstructionLib
{
	class OpcodeManager
	{
		struct Opcode
		{
			const uint16_t instruction;
			const uint16_t mask;
			//Needs to know what instance the function is part of
			const std::function<void(OpcodeManager*, VirtualMachine&, const uint16_t&)> executableMethod;
		};

	public:
		OpcodeManager()
			:m_Instructions{ 
			Opcode{ 0x00E0, 0xFFFF, &OpcodeManager::Instruction00E0 },
			Opcode{ 0x00EE, 0xFFFF, &OpcodeManager::Instruction00EE },
			Opcode{ 0x1000, 0xF000, &OpcodeManager::Instruction1NNN },
			Opcode{ 0x2000, 0xF000, &OpcodeManager::Instruction2NNN },
			Opcode{ 0x3000, 0xF000, &OpcodeManager::Instruction3XKK },
			Opcode{ 0x4000, 0xF000, &OpcodeManager::Instruction4XKK },
			Opcode{ 0x5000, 0xF00F, &OpcodeManager::Instruction5XY0 },
			Opcode{ 0x6000, 0xF000, &OpcodeManager::Instruction6XKK },
			Opcode{ 0x7000, 0xF000, &OpcodeManager::Instruction7XKK },
			Opcode{ 0x8000, 0xF00F, &OpcodeManager::Instruction8XY0 },
			Opcode{ 0x8001, 0xF00F, &OpcodeManager::Instruction8XY1 },
			Opcode{ 0x8002, 0xF00F, &OpcodeManager::Instruction8XY2 },
			Opcode{ 0x8003, 0xF00F, &OpcodeManager::Instruction8XY3 },
			Opcode{ 0x8004, 0xF00F, &OpcodeManager::Instruction8XY4 },
			Opcode{ 0x8005, 0xF00F, &OpcodeManager::Instruction8XY5 },
			Opcode{ 0x8006, 0xF00F, &OpcodeManager::Instruction8XY6 },
			Opcode{ 0x8007, 0xF00F, &OpcodeManager::Instruction8XY7 },
			Opcode{ 0x800E, 0xF00F, &OpcodeManager::Instruction8XYE },
			Opcode{ 0x9000, 0xF00F, &OpcodeManager::Instruction9XY0 },
			Opcode{ 0xA000, 0xF000, &OpcodeManager::InstructionANNN },
			Opcode{ 0xB000, 0xF000, &OpcodeManager::InstructionBNNN },
			Opcode{ 0xC000, 0xF000, &OpcodeManager::InstructionCXKK },
			Opcode{ 0xD000, 0xF000, &OpcodeManager::InstructionDXYN },
			Opcode{ 0xE09E, 0xF0FF, &OpcodeManager::InstructionEX9E },
			Opcode{ 0xE0A1, 0xF0FF, &OpcodeManager::InstructionEXA1 },
			Opcode{ 0xF007, 0xF0FF, &OpcodeManager::InstructionFX07 },
			Opcode{ 0xF00A, 0xF0FF, &OpcodeManager::InstructionFX0A },
			Opcode{ 0xF015, 0xF0FF, &OpcodeManager::InstructionFX15 },
			Opcode{ 0xF018, 0xF0FF, &OpcodeManager::InstructionFX18 },
			Opcode{ 0xF01E, 0xF0FF, &OpcodeManager::InstructionFX1E },
			Opcode{ 0xF029, 0xF0FF, &OpcodeManager::InstructionFX29 },
			Opcode{ 0xF033, 0xF0FF, &OpcodeManager::InstructionFX33 },
			Opcode{ 0xF055, 0xF0FF, &OpcodeManager::InstructionFX55 },
			Opcode{ 0xF065, 0xF0FF, &OpcodeManager::InstructionFX65 }
		} //hold fp to all possible opcode instructions
		{}
		void ExecuteOpcode(VirtualMachine& vm, const uint16_t& opC)
		{
			for (const Opcode& opCode : m_Instructions)
			{
				if (opCode.instruction == (opC & opCode.mask))
					opCode.executableMethod(this, vm, opC); //if given opCode is equal to 1 of the instructions, execute that one
			}
		}

	private:
		std::vector<Opcode> m_Instructions;
		void Instruction00E0(VirtualMachine& vm, const uint16_t& instruction)
		{
			//sets all values in array to 0 (spatial locality memory)
			std::memset(vm.m_PixelArray, 0, sizeof(uint32_t));	
		}

		void Instruction00EE(VirtualMachine& vm, const uint16_t& instruction)
		{
			vm.SetPC(vm.m_Stack[vm.GetSP()]);
			vm.DecrementSP();
		}
		//jump to address NNN
		void Instruction1NNN(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint16_t address = GetNNN(instruction);
			vm.SetPC(address);
		}

		//Call subroutine at NNN
		void Instruction2NNN(VirtualMachine& vm, const uint16_t& instruction)
		{
			vm.IncrementSP();
			vm.m_Stack[vm.GetSP()] = vm.GetPC();
			vm.SetPC(GetNNN(instruction));
		}

		//Skip next instruction if Vx == kk.
		void Instruction3XKK(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			//compare, if equal, increment PC by 2
			if (vm.m_Vx[x] == GetKK(instruction))
			{
				//Next address --> PC & PC+1 --> increase by 2
				vm.IncrementPCByTwo();
			}
		}

		//Skip next instruction if Vx != kk.
		void Instruction4XKK(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			//compare, if equal, increment PC by 2
			if (vm.m_Vx[x] != GetKK(instruction))
			{
				//Next address --> PC & PC+1 --> increase by 2
				vm.IncrementPCByTwo();
			}
		}

		//Skip next instruction if Vx == Vy.
		void Instruction5XY0(VirtualMachine& vm, const uint16_t& instruction)
		{
			//Get register X index
			const uint8_t x = GetX(instruction);
			//Get register Y index
			const uint8_t y = GetY(instruction);

			//compare, if equal, increment PC by 2
			if (vm.m_Vx[x] == vm.m_Vx[y])
			{
				//Next address --> PC & PC+1 --> increase by 2
				vm.IncrementPCByTwo();
			}
		}

		//Set Vx = kk.
		void Instruction6XKK(VirtualMachine& vm, const uint16_t& instruction)
		{
			//Get register X index
			const uint8_t x = GetX(instruction);
			//Get register Y index
			const uint8_t kk = GetKK(instruction);

			vm.m_Vx[x] = kk;
		}

		//Set Vx = Vx + kk.
		void Instruction7XKK(VirtualMachine& vm, const uint16_t& instruction)
		{
			//Get register X index
			const uint8_t x = GetX(instruction);
			//Get register Y index
			const uint8_t kk = GetKK(instruction);

			vm.m_Vx[x] += kk;
		}

		//Set Vx = Vy.
		void Instruction8XY0(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			const uint8_t y = GetY(instruction);
			vm.m_Vx[x] = vm.m_Vx[y];
		}

		//Set Vx = Vx OR Vy
		void Instruction8XY1(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			const uint8_t y = GetY(instruction);
			//Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.
			//A bitwise OR compares the corrseponding bits from two values, and if either bit is 1,
			//then the same bit in the result is also 1. Otherwise, it is 0.
			vm.m_Vx[x] = vm.m_Vx[x] | vm.m_Vx[y];

		}

		//Set Vx = Vx AND Vy.
		void Instruction8XY2(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			const uint8_t y = GetY(instruction);
			//Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
			//A bitwise AND compares the corrseponding bits from two values, and if both bits are 1,
			//then the same bit in the result is also 1. Otherwise, it is 0.
			vm.m_Vx[x] = vm.m_Vx[x] & vm.m_Vx[y];
		}

		//Set Vx = Vx XOR Vy.
		void Instruction8XY3(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			const uint8_t y = GetY(instruction);
			//Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx.
			//An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same,
			//then the corresponding bit in the result is set to 1. Otherwise, it is 0.
			vm.m_Vx[x] = vm.m_Vx[x] ^ vm.m_Vx[y];
		}

		//Set Vx = Vx + kk.
		void Instruction8XY4(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			const uint8_t y = GetY(instruction);
			//The values of Vx and Vy are added together.
			//If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0.
			//Only the lowest 8 bits of the result are kept, and stored in Vx.
			const uint16_t xY = vm.m_Vx[x] + vm.m_Vx[y];
			vm.m_Vx[0xF] = xY > sizeof(uint8_t) ? 1 : 0;
			
			vm.m_Vx[x] = GetKK(xY);
		}

		//Set Vx = Vx - Vy, set VF = NOT borrow.
		void Instruction8XY5(VirtualMachine& vm, const uint16_t& instruction)
		{

			const uint8_t x = GetX(instruction);
			const uint8_t y = GetY(instruction);

			vm.m_Vx[0xF] = vm.m_Vx[x] > vm.m_Vx[y] ? 1 : 0;

			vm.m_Vx[x] -= vm.m_Vx[y];
		}

		//Set Vx = Vx SHR 1.
		//If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
		void Instruction8XY6(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			uint8_t VxVal = vm.m_Vx[x];
			//Get lowest bit
			VxVal = GetLeastSignificantBit(VxVal);

			vm.m_Vx[0xF] = VxVal == 1 ? 1 : 0;
			//divide by 2 once
			vm.m_Vx[x] >>= 1;
		}

		//Set Vx = Vy - Vx, set VF = NOT borrow.
		//If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
		void Instruction8XY7(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			const uint8_t y = GetY(instruction);
			vm.m_Vx[0xF] = vm.m_Vx[x] < vm.m_Vx[y] ? 1 : 0;
			vm.m_Vx[x] = vm.m_Vx[y] - vm.m_Vx[x];
		}

		//Set Vx = Vx SHL 1.
		//If the most - significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
		void Instruction8XYE(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			const uint8_t y = GetY(instruction);
			vm.m_Vx[0x0F] = (vm.m_Vx[x] >> 8) == 1 ? 1 : 0;
			//left shift == *2
			vm.m_Vx[x] <<= 1;
		}

		//Skip next instruction if Vx != Vy.
		void Instruction9XY0(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			const uint8_t y = GetY(instruction);
			if (vm.m_Vx[x] != vm.m_Vx[y])
			{
				vm.IncrementPCByTwo();
			}
		}

		//Set register I = nnn
		void InstructionANNN(VirtualMachine& vm, const uint16_t& instruction)
		{
			vm.m_Vi = GetNNN(instruction);
		}

		//Jump to location nnn + V0
		void InstructionBNNN(VirtualMachine& vm, const uint16_t& instruction)
		{
			vm.SetPC(vm.m_Vx[0] + GetNNN(instruction));
		}

		//Set Vx = random byte AND kk.
		void InstructionCXKK(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			const uint8_t random = (std::rand() % 256);
			vm.m_Vx[x] = random & GetNNN(instruction);
		}

		//Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
		void InstructionDXYN(VirtualMachine& vm, const uint16_t& instruction)
		{
			//The interpreter reads n bytes from memory, starting at the address stored in I.
			const uint8_t height = GetN(instruction);//sprite Height (rows)
			//We know width is 8 pixels wide --> 1 byte, 1 px per bit
			//These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
			//Sprites are XORed onto the existing screen. If this causes any pixels to be erased,
			//VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of the display,
			//it wraps around to the opposite side of the screen.
			const uint32_t textureWidth = 64;
			uint8_t xCoord = GetX(instruction);
			xCoord = vm.m_Vx[xCoord] % textureWidth; //64 pix total width

			uint8_t yCoord = GetY(instruction);
			const uint32_t textureHeight = 32;
			yCoord = vm.m_Vx[yCoord] % textureHeight; //32 pix total height

			const uint8_t width = 8;
			bool collisionFlag = false;

			// The sprite is rendered 8 pixels wide (a pixel per bit) and n bytes high
			for (int currRow = 0; currRow < height; ++currRow)
			{
				for (int currCol = 0; currCol < width; ++currCol)
				{
					const uint16_t address = vm.m_Vi;
					const uint8_t value = vm.m_Memory[address + currRow];

					// Create mask to see if bit is on in the current column
					const uint32_t mask = (((0xFF + 1) >> 1) >> currCol);
					//e.g. 224 == 1110 0000 --> 128 == 0100 0000 ==> true 32 == 0001 0000 ==> false
					const bool isPixelActive = (value & mask);


					if (!isPixelActive)
						continue;

					//Get Pixel relative to windowsize
					const uint32_t relativeXPos = (xCoord + currCol) % uint32_t(vm.m_ScreenDimensions.width); // rest of division is relative xPos
					const uint32_t relativeYPos = (yCoord + currRow) % uint32_t(vm.m_ScreenDimensions.height);

					const uint32_t relativeIdx = uint32_t(vm.m_ScreenDimensions.width) * relativeYPos + relativeXPos; // is current pos in relative pixelarray

					//  xor pixel onto the display if true
					if (vm.m_PixelArray[relativeIdx])
					{
						collisionFlag = true;
						vm.m_PixelArray[relativeIdx] = !collisionFlag;
						continue;
					}

					vm.m_PixelArray[relativeIdx] = 0xFFFFFFFF;
				}
			}

			// Set CollisionFlag in F register
			vm.m_Vx[0xF] = collisionFlag ? 1 : 0;
			//vm.m_DisplayUpdated = true;


		}
		//Skip next instruction if key with the value of Vx is pressed.
		void InstructionEX9E(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			if (vm.m_Input[vm.m_Vx[x]])
				vm.IncrementPCByTwo();
		}

		//Skip next instruction if key with the value of Vx is not pressed.
		void InstructionEXA1(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			if (!vm.m_Input[vm.m_Vx[x]])
				vm.IncrementPCByTwo();
		}

		//Set Vx = delay timer value.
		void InstructionFX07(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			vm.m_Vx[x] = vm.m_DT;
		}

		//Wait for a key press, store the value of the key in Vx.
		void InstructionFX0A(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			for (uint8_t i{ 0 }; i < 16; i++)
			{
				//Go over input, if there is save it in Vx
				if (vm.m_Input[i])
				{
					vm.m_Vx[x] = i;
					return;
				}
			}
			//else repeat code
			vm.DecrementPCByTwo();
		}

		//Set delay timer = Vx.
		void InstructionFX15(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			vm.m_DT = vm.m_Vx[x];
		}

		//Set sound timer = Vx.
		void InstructionFX18(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			vm.m_ST = vm.m_Vx[x];
		}

		//Set I = I + Vx.
		void InstructionFX1E(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			vm.m_Vi += vm.m_Vx[x];
		}

		//Set I = location of sprite for digit Vx.
		void InstructionFX29(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			const uint8_t digit = vm.m_Vx[x];

			//letter sprites start at mem 0x050, eacht has a size of 5
			vm.m_Vi = 0x050 + (5 * digit);

		}

		//Store BCD representation of Vx in memory locations I, I+1, and I+2.
		void InstructionFX33(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);
			uint8_t decimalVal = vm.m_Vx[x];

			// Ones-place
			vm.m_Memory[vm.m_Vi + 2] = decimalVal % 10;
			decimalVal /= 10;

			// Tens-place
			vm.m_Memory[vm.m_Vi + 1] = decimalVal % 10;
			decimalVal /= 10;

			// Hundreds-place
			vm.m_Memory[vm.m_Vi] = decimalVal % 10;
		}

		//Store registers V0 through Vx in memory starting at location I.
		void InstructionFX55(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);

			for (uint8_t i = 0; i <= x; ++i)
			{
				vm.m_Memory[vm.m_Vi + i] = vm.m_Vx[i];
			}
		}

		//Read registers V0 through Vx from memory starting at location I.
		void InstructionFX65(VirtualMachine& vm, const uint16_t& instruction)
		{
			const uint8_t x = GetX(instruction);

			for (uint8_t i = 0; i <= x; ++i)
			{
				vm.m_Vx[i] = vm.m_Memory[vm.m_Vi + i];
			}
		}




		//ON 0x0FFFu
		//The opcode is the instruction. A jump is encoded as 16 bytes,
		//the first 4 (left side) of which identify it as a jump (that's the 1), 
		//and the remaining twelve (nnn, probably for "nibble" meaning half a byte) tell the CPU where to jump to.
		// 
		//The AND is to zero out the top nibble which contains a 1 and is what identifies the instruction as a jump.<-----
		uint16_t GetNNN(const uint16_t& instruction) const
		{
			return instruction & 0x0FFFu; // u == unsigned
		}

		uint8_t GetX(const uint16_t& instruction) const
		{
			//Opcode layout: Instruction RegisterX N N
			return (instruction & 0x0F00u) >> 8; // u == unsigned
		}

		uint8_t GetY(const uint16_t& instruction) const
		{
			//Opcode layout: Instruction RegisterX RegisterY N
			return (instruction & 0x00F0u) >> 4; // u == unsigned
		}
		uint8_t GetN(const uint16_t& instruction) const
		{
			//Opcode layout: Instruction RegisterX RegisterY N
			return instruction & 0x000Fu; // u == unsigned
		}
		uint8_t GetKK(const uint16_t& instruction) const
		{
			//Opcode layout: Instruction RegisterX K K
			return instruction & 0x00FFu; // u == unsigned
		}
		uint8_t GetLeastSignificantBit(uint16_t Val) const
		{
			//Opcode layout: Instruction RegisterX K K
			return Val &= ~Val + 1; // u == unsigned
		}
	};
}
// CHIP-8-Emulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "VirtualMachine.h"
#include <iostream>
#include <SDL.h>
#include <chrono>
int main(int argc, char* argv[])
{
	VirtualMachine* pVM = new VirtualMachine(12,12);
	pVM->ClearScreen();
	bool quit = false;
	int ticks = 6;
	pVM->LoadROM("../Roms/brix.rom");
	while (!quit)
	{
		float elapsedSec = 0.f;
		auto t_start = std::chrono::high_resolution_clock::now();
		quit = pVM->ProcessInput();
		pVM->Update(elapsedSec);
		auto t_end = std::chrono::high_resolution_clock::now();
		elapsedSec = std::chrono::duration<float, std::milli>(t_end - t_start).count();
		if(!quit)
			SDL_Delay(float(100)/ticks);
	}
	delete pVM;
	pVM = nullptr;
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

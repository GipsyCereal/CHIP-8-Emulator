#include "VirtualMachine.h"


VirtualMachine::VirtualMachine()
	:m_ScreenHeight{ 32 }
	, m_ScreenWidth{ 64 }
	, m_Vi{}
	, m_Vx{}
	, m_DT{}
	, m_ST{}
	, m_Input{}
	, m_Memory{}
	, m_PC{}
	, m_Stack{}
	, m_PixelArray{}
{

}
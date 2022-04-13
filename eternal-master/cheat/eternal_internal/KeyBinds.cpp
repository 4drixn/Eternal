#include "KeyBinds.hpp"

#include "FastPlace.hpp"
#include "GuiScreen.hpp"

void keybinds::onUpdate(c_context* ctx)
{
	HWND meinkraft = FindWindowA("LWJGL", nullptr);
	auto minecraft = ctx->minecraft->get();

	auto current_gui = std::make_unique<c_guiscreen>(minecraft->getCurrentScreen());
	auto timer_t = std::make_unique<timer>(minecraft->getTimer());

	if (GetForegroundWindow() == meinkraft && current_gui->get_object() == nullptr)
	{
		if (GetAsyncKeyState(VK_DELETE) & 0x8000)
			selfdestruct::should_destruct = true;

		
		//if (GetAsyncKeyState(0x59) & 0x8000)
		//{
		//	if (killaura::m_enabled)
		//	{
		//		while (GetAsyncKeyState(0x59) & 0x8000) {
		//			killaura::m_enabled = false;
		//			killaura::target = nullptr;
		//			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		//		}
		//	}
		//	else
		//	{
		//		while (GetAsyncKeyState(0x59) & 0x8000) {
		//			killaura::m_enabled = true;
		//			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		//		}
		//	}
		//}

		//if (GetAsyncKeyState(0x4F) & 0x8000)
		//{
		//	if (speed::m_enabled)
		//	{
		//		while (GetAsyncKeyState(0x4F) & 0x8000) {

		//			speed::m_enabled = false;

		//			speed::speed__ = 0.05 * 11;
		//			timer_t->set_timer_speed(1.0f);
		//			timer_t->set_ticksPerSecond(20);

		//			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		//		}
		//	}
		//	else
		//	{
		//		while (GetAsyncKeyState(0x4F) & 0x8000) {
		//			speed::m_enabled = true;
		//			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		//		}
		//	}
		//}

		//if (GetAsyncKeyState(0x46) & 0x8000)
		//{
		//	if (flight::m_enabled)
		//	{
		//		while (GetAsyncKeyState(0x46) & 0x8000) {
		//			flight::m_enabled = false;
		//			timer_t->set_timer_speed(1.0f);
		//			timer_t->set_ticksPerSecond(20);
		//			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		//		}
		//	}
		//	else
		//	{
		//		while (GetAsyncKeyState(0x46) & 0x8000) {
		//			flight::m_enabled = true;


		//			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		//		}
		//	}
		//}
		//

		/* keathiz */

		//if (GetAsyncKeyState(0x47) & 0x8000)
		//{
		//	if (misc::fastplace::m_enabled)
		//	{
		//		while (GetAsyncKeyState(0x47) & 0x8000) {
		//			misc::fastplace::m_enabled = false;
		//			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		//		}
		//	}
		//	else
		//	{
		//		while (GetAsyncKeyState(0x47) & 0x8000) {
		//			misc::fastplace::m_enabled = true;


		//			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		//		}
		//	}
		//}

		//if (GetAsyncKeyState(0x46) & 0x8000)
		//{
		//	if (aimassist::m_enabled)
		//	{
		//		while (GetAsyncKeyState(0x46) & 0x8000) {
		//			aimassist::m_enabled = false;
		//			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		//		}
		//	}
		//	else
		//	{
		//		while (GetAsyncKeyState(0x46) & 0x8000) {
		//			aimassist::m_enabled = true;


		//			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		//		}
		//	}
		//}
		//
		current_gui->~c_guiscreen();
		timer_t->~timer();
	}

}
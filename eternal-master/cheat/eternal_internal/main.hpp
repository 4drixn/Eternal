#pragma once
#define _WINSOCKAPI_
#pragma comment(lib, "Ws2_32.lib")

#include <winsock.h> 
#include <winsock2.h> 

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "winsock.hpp"

#include <gl\GLU.h>
#include <gl\GL.h>
#include "utils.hpp"
#include <memory>
#include "Minecraft.hpp"

#include "c_notification.hpp"

#include "Libs/imgui/imgui.h"
#include "Libs/imgui/imgui_impl_win32.h"
#include "Libs/imgui/imgui_impl_opengl2.h"
#include "Libs/imgui/imgui_internal.h"

// One big mess 

namespace initialize {
	extern void __stdcall init();
}

class c_main : public singleton<c_main> {
public:

	void hook(void);
	void unhook(void);

	HWND m_hwnd;
	WNDPROC m_wndproc;

	HMODULE m_opengl_dll = nullptr;
	HMODULE m_kernel32_dll = nullptr;

	void* m_wgl_swap_buffers;
	void* m_open_process;
	void* m_create_remote_thread;
	void* m_enum_process_modules;

	bool b_open = true;

};

using fn_wgl_swap_buffers = bool(__stdcall*)(_In_ HDC);
using fn_open_process = void* (__stdcall*)(unsigned long, bool, unsigned long);
using fn_create_remote_thread = void* (__stdcall*)(void*, LPSECURITY_ATTRIBUTES, unsigned long*, LPTHREAD_START_ROUTINE, void*, DWORD, void*);
using fn_enum_process_modules = bool(__stdcall*)(void*, HMODULE, DWORD, void*);


typedef BOOL(*type_wglSwapLayerBuffers)(HDC, UINT);
extern type_wglSwapLayerBuffers owglSwapLayerBuffers;

extern fn_open_process original_open_process;
extern fn_create_remote_thread original_create_remote_thread;
extern fn_enum_process_modules original_enum_process_modules;


namespace notification {
	inline std::vector<c_notification> notifications;
}

namespace gameinfo
{
	inline std::vector<glm::vec2> player_locations;
	inline std::vector<glm::vec4> player_box_locations;
}

namespace hooks {
	BOOL wgl_swap_buffers(HDC hdc, UINT uint);
	LRESULT CALLBACK wndproc(HWND window_handle, UINT user_msg, WPARAM wparam, LPARAM lparam);
	void* __stdcall open_process(unsigned long, bool, unsigned long);
	void* __stdcall create_remote_thread(void*, LPSECURITY_ATTRIBUTES, unsigned long*, LPTHREAD_START_ROUTINE, void*, DWORD, void*);
	bool __stdcall enum_process_modules(void*, HMODULE, DWORD, void*);
	inline 	std::vector<void(*)()> callbacks = std::vector<void(*)()>();

	extern void onRender(void(*func)());
//	extern int WINAPI WSAReceiveHook(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
	inline utils::s_context gl_context{};

}

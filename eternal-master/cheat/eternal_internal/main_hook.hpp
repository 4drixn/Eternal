
//#include "winsock2.h"
#include "main.hpp"
#include <thread>
#include "Libs/Minhook/MinHook.h"
#include <iostream>
#include "FakeLag.hpp"
#include "Chams.hpp"



//typedef int(WINAPI* WSArecv_)(SOCKET, LPWSABUF, DWORD, LPDWORD, LPDWORD, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);


//WSArecv_ WSAReceive = NULL;

//int packets;
//int WINAPI WSAReceiveHook(
//	SOCKET                             s,
//	LPWSABUF                           lpBuffers,
//	DWORD                              dwBufferCount,
//	LPDWORD                            lpNumberOfBytesRecvd,
//	LPDWORD                            lpFlags,
//	LPWSAOVERLAPPED                    lpOverlapped,
//	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
//)
//{
//	//static auto start = std::chrono::high_resolution_clock::now();
//
//	//packets++;
//
//	if (fakelag::m_enabled == true || fakelag::should_do_it == true) {
//
//		//auto finish = std::chrono::high_resolution_clock::now();
//
//		//std::chrono::duration<double> elapsed = finish - start;
//		//std::cout << elapsed.count() << std::endl;
//
//		//float gey = fakelag::delay / ( (elapsed.count()) / packets);
//		std::this_thread::sleep_for(std::chrono::milliseconds((LONG)fakelag::delay / 2));
//		//lmfao(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
//	  //  WSAPACKET packet = {s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine}
//
//		return WSAReceive(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine); //
//	}
//	else {
//
//		return WSAReceive(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
//	}
//	return 0;
//}

namespace hooks {
    typedef void (APIENTRY* glScalef_t)(GLfloat, GLfloat, GLfloat);
	
	void* glScale_addr;
    glScalef_t oglScalef;

    void hk_glScalef(GLfloat x, GLfloat y, GLfloat z)
    {


        if (chams::m_enabled)
        {
            if (x == 0.9375)
            {
                glEnable(GL_POLYGON_OFFSET_FILL);
                glPolygonOffset(1.0f, -1100000.0f);

                if (chams::wireframe)
                {
                    glLineWidth(3.f);

                    glPolygonMode(1032, 6913);
                    glEnable(10754);
                    glDepthRange(0.0, 0.01);
                    glDisable(3553);
                    glEnable(2848);
                    glHint(3154, 4354);
                }

                if (chams::fill_chams)
                {
                    glDisable(GL_TEXTURE_2D);
                    glColor3f(chams::color[0], chams::color[1], chams::color[2]);
                    //glColor4f(chams::color[0], chams::color[1], chams::color[2], 0.f);

                    //glDisable(GL_DEPTH_TEST);
                    //glDepthMask(true);
                    //glAlphaFunc(516, 0.1F);
                    glDisable(GL_LIGHTING);

                }


            }
            else {
                // No Chest
                //if (x == 1)
                //{
                //  //  if (!chest_chams)
                //    {
                //        glDisable(GL_POLYGON_OFFSET_FILL);
                //        glPolygonOffset(1.0f, 1100000.0f);

                //        if (chams::fill_chams)
                //        {
                //            glEnable(GL_TEXTURE_2D);
                //            glColor4f(1, 1, 1, 1);
                //        }
                //    	
                //    }
                // //   else {}

                //}

                // idk
                //-0.0266667
                //1
                if (x == 0.00390625 || x == 0.666667 || x == 0.0104167 || x == -0.0266667)
                {
                    glDisable(GL_POLYGON_OFFSET_FILL);
                    glPolygonOffset(1.0f, 1100000.0f);

                    if (chams::wireframe)
                    {
                        glPolygonMode(1032, 6914);
                        glDepthRange(0.0, 1.0);
                        glEnable(3553);
                        glDisable(2848);
                    }
                	
                    if (chams::fill_chams)
                    {
                        glEnable(GL_TEXTURE_2D);
                        glColor4f(1, 1, 1, 1);
                    }
                }
                else {
                    glDisable(GL_POLYGON_OFFSET_FILL);
                    glPolygonOffset(1.0f, 1100000.0f);

                    if (chams::wireframe)
                    {
                        glPolygonMode(1032, 6914);
                        glDepthRange(0.0, 1.0);
                        glEnable(3553);
                        glDisable(2848);
                    }
                	
                    if (chams::fill_chams)
                    {
                        glEnable(GL_TEXTURE_2D);
                        glColor4f(1, 1, 1, 1);
                    }
                }

                if (x > 0.f)
                {
                    glDisable(GL_POLYGON_OFFSET_FILL);
                    glPolygonOffset(1.0f, 1100000.0f);

                	if(chams::wireframe)
                    {
                        glPolygonMode(1032, 6914);
                        glDepthRange(0.0, 1.0);
                        glEnable(3553);
                        glDisable(2848);
                    }

                    if (chams::fill_chams)
                    {
                        glEnable(GL_TEXTURE_2D);
                        glColor4f(1, 1, 1, 1);
                    }

                }

            }

        }


        oglScalef(x, y, z);
    }

	
}
void c_main::hook(void)
{

	while (!(c_main::m_opengl_dll = GetModuleHandleA("opengl32.dll"))) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	while (!(c_main::m_kernel32_dll = GetModuleHandleA("kernel32.dll"))) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	auto get_window_title = [&](void) {
		char title[256];

		m_hwnd = FindWindowA("LWJGL", NULL);
		GetWindowTextA(m_hwnd, title, sizeof(title));

		std::string m_title = title;
		return m_title;
	};

	c_main::m_wgl_swap_buffers = GetProcAddress(c_main::m_opengl_dll, "wglSwapBuffers");
	hooks::glScale_addr = GetProcAddress(c_main::m_opengl_dll, "glScalef");

	//m_wndproc = (WNDPROC)SetWindowLongPtrA(m_hwnd, GWLP_WNDPROC, (LONG_PTR)hooks::wndproc);

	MH_Initialize();

	if (MH_CreateHook(&WSARecv, &hooks::WSAReceiveHook, reinterpret_cast<LPVOID*>(&hooks::WSAReceive)) != MH_OK)
		std::cout << "[DEBUG] Failed to create hook [3]\n";

	if (MH_EnableHook(&WSARecv) != MH_OK)
		std::cout << "[DEBUG] Failed to enable hook\n";

	if (MH_CreateHook(&WSASend, &hooks::WSASendHook, reinterpret_cast<LPVOID*>(&hooks::_WSASend_)) != MH_OK)
		std::cout << "[DEBUG] Failed to create hook [4]\n";

	if (MH_EnableHook(&WSASend) != MH_OK)
		std::cout << "[DEBUG] Failed to enable hook\n";

	if (MH_CreateHook(c_main::m_wgl_swap_buffers, hooks::wgl_swap_buffers, reinterpret_cast<void**>(&owglSwapLayerBuffers)) != MH_OK)
		std::cout << "[DEBUG] Failed to create openGL hook\n";
	
	if (MH_EnableHook(c_main::m_wgl_swap_buffers) != MH_OK )
		std::cout << "[DEBUG] Failed to enable openGL hook\n";

	if (MH_CreateHook(hooks::glScale_addr, hooks::hk_glScalef, reinterpret_cast<void**>(&hooks::oglScalef)) != MH_OK)
		std::cout << "[DEBUG] Failed to create hook [4]\n";

	if (MH_EnableHook(hooks::glScale_addr) != MH_OK)
		std::cout << "[DEBUG] Failed to enable openGL hook\n";

}
void c_main::unhook(void)
{

	ImGui_ImplOpenGL2_Shutdown();
	wglDeleteContext(hooks::gl_context.m_glrenderctx);
	ImGui::DestroyContext();
	ImGui_ImplWin32_Shutdown();

	SetWindowLongPtrA(m_hwnd, GWLP_WNDPROC, (LONG_PTR)m_wndproc);
	MH_DisableHook(MH_ALL_HOOKS);


	if (MH_DisableHook(&WSARecv) == MB_OK)
	{
		MH_RemoveHook(&WSARecv);
	}
	if (MH_DisableHook(c_main::m_wgl_swap_buffers) == MB_OK)
	{
		MH_RemoveHook(c_main::m_wgl_swap_buffers);
	}
	//MH_RemoveHook(c_main::m_open_process);
	//MH_RemoveHook(c_main::m_create_remote_thread);
	//MH_RemoveHook(c_main::m_enum_process_modules);
	MH_Uninitialize();
}


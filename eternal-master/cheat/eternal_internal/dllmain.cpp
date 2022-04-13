#include <iostream>
#include <thread>
#include <chrono>

#include "main.hpp"
#include "global.hpp"
#include "AutoSDK.hpp"
#include "config.hpp"

#include "unload.h"

#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")

#pragma comment(lib, "legacy_stdio_definitions.lib")
HMODULE global::instance = 0;
HANDLE _main_2 = 0;


BOOL WINAPI DllMain( HINSTANCE hModule, DWORD reason, LPVOID lpReserved )
{
    if (reason == DLL_PROCESS_ATTACH)
    {

		//global::headers = ImageNtHeader(hModule);
		//if (!global::headers)
        //return 0;


		UnlinkModuleFromPEB(hModule);
		DisableThreadLibraryCalls(hModule);
		AllocConsole();
		ShowWindow(GetConsoleWindow(), SW_SHOW);

		freopen("CONOUT$", "w", stdout);

    	
		global::instance = hModule;

		auto autosdk = std::make_shared<c_AutoSDK>();
		autosdk->init();
		config::init();

		std::cout << "Initializing..." << std::endl;


		global::_main_ = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(initialize::init), nullptr, 0, nullptr);

		if (HideThread(global::_main_))
			printf("Thread successfully hidden.\n");

    }
    else if(reason == DLL_PROCESS_DETACH)
    {
        
    }

    return TRUE;
}


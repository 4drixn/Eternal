#pragma once
#include <Windows.h>
#include <jni.h>
#include <jvmti.h>
#include <memory>


namespace global {

	inline HMODULE jvmHandle = nullptr;
	inline FARPROC func_JNI_GetCreatedJavaVMs = nullptr;
	inline JavaVM* jvm = nullptr;
	inline JNIEnv* jenv = nullptr;
	inline jvmtiEnv* jvmti = nullptr;

	extern HMODULE instance;

	inline IMAGE_NT_HEADERS* headers;

	inline bool is_badlion = false;
	inline bool is_falcun = false;
	inline bool is_lunar = false;

	inline bool V_3 = false;

	// Animations | Gui helpers
	inline bool window_pos_changed = false;
	inline bool ok_init = false;

	inline HANDLE _main_ = 0;


}
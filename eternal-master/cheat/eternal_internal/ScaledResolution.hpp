#include <jni.h>
#include "functions.hpp"

class c_scaledresolution {
private:
	jobject scaledresolution_obj;
	JNIEnv* env;

public:
	c_scaledresolution(jobject obj)
	{
		scaledresolution_obj = obj;
		env = getJNI();
	}

	
	int get_scaled_width()
	{

		if (global::is_lunar)
		{
			auto this_class = mcc::ScaledResolution;
			jmethodID mid = env->GetMethodID(this_class, "heapheehhseehppeaapshapes", "()I");

			return env->CallIntMethod(scaledresolution_obj, mid);
		}
		else {
			auto this_class = env->GetObjectClass(scaledresolution_obj);
			jmethodID mid = env->GetMethodID(this_class, global::is_badlion ? "a" : "func_78326_a", "()I");
			env->DeleteLocalRef(this_class);

			return env->CallIntMethod(scaledresolution_obj, mid);
		}
	}

	int get_scaled_resolution() //scalefactor
	{
		if (global::is_lunar)
		{
			auto this_class = mcc::ScaledResolution;
			jmethodID mid = env->GetMethodID(this_class, "hpehhphhspaashpssehpsasee", "()I");

			return env->CallIntMethod(scaledresolution_obj, mid);
		}
		else {
			auto this_class = env->GetObjectClass(scaledresolution_obj);
			jmethodID mid = env->GetMethodID(this_class, global::is_badlion ? "e" : "func_78325_e", "()I");
			env->DeleteLocalRef(this_class);
			return env->CallIntMethod(scaledresolution_obj, mid);
		}
	}
};
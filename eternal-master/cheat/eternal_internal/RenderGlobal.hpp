#include <jni.h>
#include "functions.hpp"

class c_renderglobal {
private:
	jobject render_obj;
	JNIEnv* env;

public:
	c_renderglobal(jobject obj)
	{
		render_obj = obj;
		env = getJNI();
	}


	void drawSelectionBoundingBox(jobject boundingbox)
	{
		auto this_class = env->GetObjectClass(render_obj);
		jmethodID mid = env->GetStaticMethodID(this_class, global::is_badlion ? "a" : "func_181561_a", global::is_badlion ? "(Laug;)V" : "(Lnet/minecraft/util/AxisAlignedBB;)V");

		env->CallStaticVoidMethod(this_class, mid, boundingbox);
		env->DeleteLocalRef(this_class);
	}

	void drawSelectionBoundingBox(jobject boundingbox, int r, int g, int b, int alpha)
	{

		jmethodID mid;
		if(global::is_lunar)
		{
			mid = env->GetStaticMethodID(mcc::RenderGlobal, "spepesshehshaaheesasshshh", "(Lnet/minecraft/v1_8/hpspaspaasspaaapheppseaps;IIII)V");
			if (mid != nullptr) {
				env->CallStaticVoidMethod(mcc::RenderGlobal, mid, boundingbox, r, g, b, alpha);
			}
		}
		else {
			auto this_class = env->GetObjectClass(render_obj);
			mid = env->GetStaticMethodID(this_class, global::is_badlion ? "a" : "func_181563_a", global::is_badlion ? "(Laug;IIII)V" : "(Lnet/minecraft/util/AxisAlignedBB;IIII)V");

			env->CallStaticVoidMethod(this_class, mid, boundingbox, r, g, b, alpha);
			env->DeleteLocalRef(this_class);
		}
		

	}


};
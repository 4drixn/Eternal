#include "global.hpp"
#include <jni.h>
#include <vector>
#include <memory>
#include "WorldRenderer.hpp"
#include <iostream>
#include "functions.hpp"


class c_tesellator {
private:
	jobject tesel_obj;
	JNIEnv* env;
public:
	c_tesellator(jobject obj)
	{
		tesel_obj = obj;
		env = global::jenv;
	}

	//draw	func_78381_a
	//getWorldRenderer	func_178180_c

	std::shared_ptr<c_worldrenderer> get_world_renderer()
	{
		jclass c = getObject(env, "net.minecraft.client.renderer.Tessellator");
		jfieldID fid = env->GetFieldID(c, "field_178183_a", "Lnet/minecraft/client/renderer/WorldRenderer;");

		auto ret = std::make_shared<c_worldrenderer>(env->GetObjectField(c, fid));
		env->DeleteLocalRef(c);

		return ret;
	}

	void draw()
	{
		auto this_class = env->GetObjectClass(tesel_obj);
		jmethodID mid = env->GetMethodID(this_class, "func_78381_a", "()V");
		env->DeleteLocalRef(this_class);

		env->CallVoidMethod(tesel_obj, mid);
	}
	jobject get_object()
	{
		return tesel_obj;
	}
};
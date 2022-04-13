#include "global.hpp"

class c_worldrenderer {

private:
	jobject worldrenderer_obj;
	JNIEnv* env;

public:
	c_worldrenderer(jobject obj)
	{
		worldrenderer_obj = obj;
		env = global::jenv;
	}

	bool is_drawing()
	{
		jfieldID fid = env->GetFieldID(env->GetObjectClass(worldrenderer_obj), "field_179010_r", "Z");

		return env->GetBooleanField(worldrenderer_obj, fid);
	}
	void set_is_drawing(bool state)
	{
		jfieldID fid = env->GetFieldID(env->GetObjectClass(worldrenderer_obj), "field_179010_r", "Z");

		env->SetBooleanField(worldrenderer_obj, fid, state);
	}

	

	// begin


	//finishDrawing
	void addVertexData(int arr[])
	{
		jmethodID mid = env->GetMethodID(env->GetObjectClass(worldrenderer_obj), "func_178981_a", "([I)V");
		if (!mid)
			return;
		jintArray position = (jintArray)env->NewIntArray(3);
		if (!position)
			return;

		jint* f = (jint*)calloc(3, sizeof(jint));
		for (int i = 0; i < 3; i++) {
			f[i] = i;
		}
		env->SetIntArrayRegion(position, 0, 3, f);

		env->CallVoidMethod(worldrenderer_obj, mid, f);
	}

	void set_drawMode(int val)
	{
		jfieldID fid = env->GetFieldID(env->GetObjectClass(worldrenderer_obj), "field_179006_k", "I");

		env->SetIntField(worldrenderer_obj, fid, val);
	}
	void reset()
	{
		jmethodID mid = env->GetMethodID(env->GetObjectClass(worldrenderer_obj), "func_178965_a", "()V");

		env->CallVoidMethod(worldrenderer_obj, mid);

	}

	void startDrawing(int val)
	{
		if (this->is_drawing())
		{

		}
		else {
			this->set_is_drawing(true);
			this->reset();
			this->set_drawMode(val);

		}
	}
};
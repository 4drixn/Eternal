#ifndef FONTRENDERER_HPP
#define FONTRENDERER_HPP

#include <jni.h>
#include <string>
#include "global.hpp"


class c_fontrenderer {

private:
	jobject fontrenderer_obj;
	JNIEnv* env;

public:
	c_fontrenderer(jobject obj, JNIEnv* the_env)
	{
		fontrenderer_obj = obj;
		env = the_env;
	}
	~c_fontrenderer()
	{
		env->DeleteLocalRef(fontrenderer_obj);
	}

	void drawString(const char* text, float x, float y, int colour, bool dropshadow)
	{
		jstring* r = new jstring(env->NewStringUTF(text));
		jfloat x_jfloat = x;
		jfloat y_jfloat = y;
		jint color_jint = colour;
		jboolean shadow_jbool = dropshadow;

		if (global::is_lunar)
		{
			auto fontrenderer_class = env->GetObjectClass(fontrenderer_obj);
			//eassesepeeaapephaepesapea(String paramString, float paramFloat1, float paramFloat2, int paramInt, boolean paramBoolean) 
			jmethodID mid = env->GetMethodID(fontrenderer_class, "bridge$drawString", "(Ljava/lang/String;FFIZ)F");
			//haphpapsseppsaphespppasaa
			//bridge$drawString
			//float method
			if(!mid || !fontrenderer_obj)
			{
				std::cout << "method drawString not found!";
				return;
			}
			
			//env->CallIntMethod(fontrenderer_obj, mid, text_jstr, x_jfloat, y_jfloat, color_jint, shadow_jbool);
			env->CallFloatMethod(fontrenderer_obj, mid, *r, x_jfloat, y_jfloat, color_jint, shadow_jbool);
			env->DeleteLocalRef(*r);
			env->DeleteLocalRef(fontrenderer_class);
		}
		else {
			auto fontrenderer_class = env->GetObjectClass(fontrenderer_obj);

			jmethodID mid = env->GetMethodID(fontrenderer_class, global::is_badlion ? "a" : "func_175065_a", "(Ljava/lang/String;FFIZ)I");
			env->DeleteLocalRef(fontrenderer_class);

			env->CallIntMethod(fontrenderer_obj, mid, *r, x_jfloat, y_jfloat, color_jint, shadow_jbool);
			env->DeleteLocalRef(*r);
		}
		delete r;
	}

	int font_height()
	{
		auto fontrenderer_class = env->GetObjectClass(fontrenderer_obj);
		jfieldID fid = env->GetFieldID(fontrenderer_class, global::is_badlion ? "a" : "field_78288_b", "I");
		env->DeleteLocalRef(fontrenderer_class);

		return env->GetIntField(fontrenderer_obj, fid);
	}

	int get_string_width(std::string text)
	{

		jstring text_jstr = env->NewStringUTF(text.c_str());

		if (global::is_lunar)
		{
			auto fontrenderer_class = env->GetObjectClass(fontrenderer_obj);

			jmethodID mid = env->GetMethodID(fontrenderer_class, "original$getStringWidth", "(Ljava/lang/String;)I");
			int a = env->CallIntMethod(fontrenderer_obj, mid, text_jstr);
			env->DeleteLocalRef(text_jstr);
			env->DeleteLocalRef(fontrenderer_class);

			return a;
		}
		else {
			auto fontrenderer_class = env->GetObjectClass(fontrenderer_obj);

			jmethodID mid = env->GetMethodID(fontrenderer_class, global::is_badlion ? "a" : "func_78256_a", "(Ljava/lang/String;)I");

			env->DeleteLocalRef(fontrenderer_class);
			int a = env->CallIntMethod(fontrenderer_obj, mid, text_jstr);
			env->DeleteLocalRef(text_jstr);
			return a;
		}

	}

	jobject get_object()
	{
		return fontrenderer_obj;
	}

};

#endif
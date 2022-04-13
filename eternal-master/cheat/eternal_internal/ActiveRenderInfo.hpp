#pragma once

#include <jni.h>
#include <gl\GL.h>
#include "functions.hpp"

class c_activerenderinfo {
private:
	JNIEnv* env;

public:
	c_activerenderinfo(JNIEnv* env_)
	{
		env = env_;
	}

	std::vector<GLint> Get_VIEWPORT()
	{
		std::vector<GLint> vals = std::vector<GLint>(16);
		if(global::is_lunar)
		{
			static jclass CActiveRenderInfo = mcc::ActiveRenderInfo;
			jfieldID FIDVIEWPORT = env->GetStaticFieldID(CActiveRenderInfo, "phapaaeheahasapepespppaas", "Ljava/nio/IntBuffer;");
			if (!FIDVIEWPORT)
				return vals;
			jobject OVIEWPORT = env->GetStaticObjectField(CActiveRenderInfo, FIDVIEWPORT);

			static jclass CIntBuffer = env->FindClass("java/nio/IntBuffer");
			jmethodID MIDget = env->GetMethodID(CIntBuffer, "get", "(I)I");
			for (int i = 0; i < 16; i++)
			{
				vals[i] = env->CallIntMethod(OVIEWPORT, MIDget, i);
			}
			//env->DeleteLocalRef(OVIEWPORT);
		}
		else {
			static jclass CActiveRenderInfo = getObject(env, global::is_badlion ? "auz" : "net.minecraft.client.renderer.ActiveRenderInfo");
			jfieldID FIDVIEWPORT = env->GetStaticFieldID(CActiveRenderInfo, global::is_badlion ? "a" : "field_178814_a", "Ljava/nio/IntBuffer;");
			jobject OVIEWPORT = env->GetStaticObjectField(CActiveRenderInfo, FIDVIEWPORT);
			
			static jclass CIntBuffer = env->FindClass("java/nio/IntBuffer");
			jmethodID MIDget = env->GetMethodID(CIntBuffer, "get", "(I)I");
			for (int i = 0; i < 16; i++)
			{
				vals.at(i) = env->CallIntMethod(OVIEWPORT, MIDget, i);
			}

			env->DeleteLocalRef(OVIEWPORT);
		}
		return vals;
	}

	std::vector<GLfloat> Get_MODELVIEW()
	{
		
		std::vector<GLfloat> MODELVIEW = std::vector<GLfloat>(16);
		if (global::is_lunar)
		{
			static jclass CActiveRenderInfo = mcc::ActiveRenderInfo;
			if (CActiveRenderInfo != nullptr) {
				jfieldID FIDMODELVIEW = env->GetStaticFieldID(CActiveRenderInfo, "sesspaahsahsaphepeeehhssa", "Ljava/nio/FloatBuffer;");
				if (!FIDMODELVIEW)
					return MODELVIEW;
				jobject OMODELVIEW = env->GetStaticObjectField(CActiveRenderInfo, FIDMODELVIEW);

				static jclass CFloatBuffer = env->FindClass("java/nio/FloatBuffer");

				jmethodID MIDget = env->GetMethodID(CFloatBuffer, "get", "(I)F");

				for (int i = 0; i < 16; i++)
				{
					MODELVIEW[i] = env->CallFloatMethod(OMODELVIEW, MIDget, i);;
				}

			//	env->DeleteLocalRef(OMODELVIEW);
			}
		}
		else {
			static jclass CActiveRenderInfo = getObject(env, global::is_badlion ? "auz" : "net.minecraft.client.renderer.ActiveRenderInfo");
			jfieldID FIDMODELVIEW = env->GetStaticFieldID(CActiveRenderInfo, global::is_badlion ? "b" : "field_178812_b", "Ljava/nio/FloatBuffer;");
			if (!FIDMODELVIEW)
				return MODELVIEW;
			jobject OMODELVIEW = env->GetStaticObjectField(CActiveRenderInfo, FIDMODELVIEW);

			static jclass CFloatBuffer = env->FindClass("java/nio/FloatBuffer");
			jmethodID MIDget = env->GetMethodID(CFloatBuffer, "get", "(I)F");

			for (int i = 0; i < 16; i++)
			{
				float val = env->CallFloatMethod(OMODELVIEW, MIDget, i);
				MODELVIEW.at(i) = (GLfloat)val;
			}

			env->DeleteLocalRef(OMODELVIEW);
		}
		return MODELVIEW;
	}

	std::vector<GLfloat> Get_PROJECTION()
	{
		std::vector<GLfloat> PROJECTION = std::vector<GLfloat>(16);
		if (global::is_lunar)
		{
			static jclass CActiveRenderInfo = mcc::ActiveRenderInfo;
			jfieldID FIDPROJECTION = env->GetStaticFieldID(CActiveRenderInfo, "spsaahesspasaphhhehaehpsh", "Ljava/nio/FloatBuffer;");
			if (!FIDPROJECTION)
				return PROJECTION;
			jobject OPROJECTION = env->GetStaticObjectField(CActiveRenderInfo, FIDPROJECTION);
			static jclass CFloatBuffer = env->FindClass("java/nio/FloatBuffer");
			jmethodID MIDget = env->GetMethodID(CFloatBuffer, "get", "(I)F");

			for (int i = 0; i < 16; i++)
			{
				PROJECTION[i] = env->CallFloatMethod(OPROJECTION, MIDget, i);
			}

		//	env->DeleteLocalRef(OPROJECTION);
		}
		else {
			static jclass CActiveRenderInfo = getObject(env, global::is_badlion ? "auz" : "net.minecraft.client.renderer.ActiveRenderInfo");
			jfieldID FIDPROJECTION = env->GetStaticFieldID(CActiveRenderInfo, global::is_badlion ? "c" : "field_178813_c", "Ljava/nio/FloatBuffer;");
			jobject OPROJECTION = env->GetStaticObjectField(CActiveRenderInfo, FIDPROJECTION);
			jclass CFloatBuffer = env->FindClass("java/nio/FloatBuffer");
			jmethodID MIDget = env->GetMethodID(CFloatBuffer, "get", "(I)F");

			for (int i = 0; i < 16; i++)
			{
				PROJECTION.at(i) = (GLfloat)env->CallFloatMethod(OPROJECTION, MIDget, i);
			}

			env->DeleteLocalRef(OPROJECTION);
		}
		return PROJECTION;
	}
};
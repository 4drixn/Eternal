#include <jni.h>
#include "global.hpp"
#include <string>

class IChatComponent {
private:
	jobject chat_obj;
	JNIEnv* env;
public:
	IChatComponent(jobject obj)
	{
		chat_obj = obj;
		env = global::jenv;
	}
	jobject get_object()
	{
		return chat_obj;
	}

	std::string getUnformattedText()
	{
		jclass this_class = env->GetObjectClass(chat_obj);

		jmethodID mid = env->GetMethodID(this_class, global::is_badlion ? "c" : "func_150260_c", "()Ljava/lang/String;");
		jstring jstr = (jstring)env->CallObjectMethod(chat_obj, mid);
		const char* strReturn = env->GetStringUTFChars(jstr, 0);
		env->ReleaseStringUTFChars(jstr, strReturn);
		env->DeleteLocalRef(jstr);
		env->DeleteLocalRef(this_class);

		return strReturn;
	}

	std::string getFormattedText()
	{
		jclass this_class = env->GetObjectClass(chat_obj);

		jmethodID mid = env->GetMethodID(this_class, global::is_badlion ? "d" : "func_150254_d", "()Ljava/lang/String;");
		jstring jstr = (jstring)env->CallObjectMethod(chat_obj, mid);
		const char* strReturn = env->GetStringUTFChars(jstr, 0);
		env->ReleaseStringUTFChars(jstr, strReturn);
		env->DeleteLocalRef(jstr);
		env->DeleteLocalRef(this_class);

		return strReturn;
	}
};
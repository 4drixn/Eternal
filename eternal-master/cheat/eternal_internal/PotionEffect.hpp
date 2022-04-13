#include "global.hpp"

class c_potioneffect {
private:
	jobject potion_obj;
	JNIEnv* env;

public:
	c_potioneffect(jobject obj)
	{
		potion_obj = obj;
		env = global::jenv;
	}

	int get_amplifier()
	{
		auto this_class = env->GetObjectClass(potion_obj);
		jfieldID fid;

		//public static final Logger ahssphhsehseepeaspsppehah = LogManager.getLogger();
		//public int pphashspaaeeeepesaaehaash;
		//public int hssheaasappsehassespeahea;
		//public int epeehphpeppashppaassaspae;
		if(global::is_lunar)
			fid = env->GetFieldID(this_class, "hphsaephsahepaapasspepesh", "I");
		else
			fid = env->GetFieldID(this_class, global::is_badlion ? "d" : "field_76461_c", "I");

		env->DeleteLocalRef(this_class);

		return env->GetIntField(potion_obj, fid);
	}
	
	jobject get_object()
	{
		return potion_obj;
	}
};
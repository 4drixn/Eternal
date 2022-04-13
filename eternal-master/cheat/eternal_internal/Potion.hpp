#include "global.hpp"
#include "functions.hpp"

class c_potion {
private:

	jobject potion_obj;
	JNIEnv* env;

public:

	c_potion()
	{
		env = global::jenv;
	//	potion_obj = global::is_lunar ? mcc::Potion : getObject(env, global::is_badlion ? "pe" : "net.minecraft.potion.Potion");
	}

	jobject get_speed_potion()
	{
		jfieldID fid;
		
		if (global::is_lunar)
		{
			static auto field = mcc::impl::Potion::fields[3];
			fid = global::jenv->GetStaticFieldID(mcc::Potion, field.name, field.sig);

			return global::jenv->GetStaticObjectField(mcc::Potion, fid);
		}
		else {
			jclass klass_ = getObject(global::jenv, global::is_badlion ? "pe" : "net.minecraft.potion.Potion");

			fid = global::jenv->GetStaticFieldID(klass_, global::is_badlion ? "c" : "field_76424_c", global::is_badlion ? "Lpe;" : "Lnet/minecraft/potion/Potion;");
			global::jenv->DeleteLocalRef(klass_);

			return global::jenv->GetStaticObjectField(klass_, fid);
		}


	}

};

__forceinline jobject get_speed_potion()
{
	jfieldID fid;

	if (global::is_lunar)
	{
		static auto field = mcc::impl::Potion::fields[3];
		fid = global::jenv->GetStaticFieldID(mcc::Potion, field.name, field.sig);		

		return global::jenv->GetStaticObjectField(mcc::Potion, fid);
	}
	else {
		jclass klass_ = getObject(global::jenv, global::is_badlion ? "pe" : "net.minecraft.potion.Potion");

		fid = global::jenv->GetStaticFieldID(klass_, global::is_badlion ? "c" : "field_76424_c", global::is_badlion ? "Lpe;" : "Lnet/minecraft/potion/Potion;");
		global::jenv->DeleteLocalRef(klass_);

		return global::jenv->GetStaticObjectField(klass_, fid);
	}



}
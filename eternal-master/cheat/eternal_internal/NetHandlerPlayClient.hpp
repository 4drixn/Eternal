#include "functions.hpp"
#include "NetworkPlayerInfo.h"

class c_nethandlerplayclient {
private:
	jobject the_obj;
	JNIEnv* env;
public:
	c_nethandlerplayclient(jobject obj)
	{
		the_obj = obj;
		env = global::jenv;
	}

	void addToSendQueue(jobject packet)
	{	
		static jmethodID mid;

		if (static bool once = false; !once) {
			if (global::is_lunar)
				mid = env->GetMethodID(mcc::NetHandlerPlayClient, "asepsehphhehhsssppsshsphp", "(Lnet/minecraft/v1_8/shhpappahppshssseshsesphe;)V");
			else {
				static auto this_class = env->GetObjectClass(the_obj);
				mid = env->GetMethodID(this_class, global::is_badlion ? "a" : "func_147297_a", global::is_badlion ? "(Lff;)V" : "(Lnet/minecraft/network/Packet;)V");
			}
			once = true;
		}

		if (packet == nullptr || mid == nullptr)
			return;

		return env->CallVoidMethod(the_obj, mid, packet);
	}

	std::shared_ptr<c_networkplayerinfo> getPlayerInfo(const char* player_name)
	{

		if (the_obj != nullptr) {
			static auto this_class = env->GetObjectClass(the_obj);
			jmethodID mid;

			if (global::is_lunar)
				mid = env->GetMethodID(this_class, "eshseassshepsephsapspesas", "(Ljava/lang/String;)Lnet/minecraft/v1_8/papphpppseseehasehsahaehh;");
			else
				mid = env->GetMethodID(this_class, global::is_badlion ? "a" : "func_175104_a", global::is_badlion ? "(Ljava/lang/String;)Lbdc;" : "(Ljava/lang/String;)Lnet/minecraft/client/network/NetworkPlayerInfo;");


			auto jstr = env->NewStringUTF(player_name);
			if (the_obj != nullptr && mid != nullptr) {
				
				return std::make_shared<c_networkplayerinfo>(env->CallObjectMethod(the_obj, mid, jstr));
			}
		}

		return nullptr;
	}
	
	jobject get_object()
	{
		return the_obj;
	}

};

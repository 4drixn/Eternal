#ifndef MINECRAFT_HPP
#define MINECRAFT_HPP

#include <jni.h>
#include <memory>
#include "Classes.hpp"
#include "FontRenderer.hpp"
#include <sstream>

class c_minecraft {
private:
	jobject mc;
	JNIEnv* env;

public:

	c_minecraft(jobject minecraft, JNIEnv* env_)
	{
		mc = minecraft;
		env = env_;
	}

	void set_left_click_counter(int count)
	{
		if(global::is_lunar)
		{ 
			static const auto& field = mcc::impl::ave::fields[37];
			jfieldID fid = env->GetFieldID(mcc::ave, field.name, field.sig);

			env->SetIntField(mc, fid, count);
		}
		else {
			static auto mc_class = env->GetObjectClass(mc);
			jfieldID fid = env->GetFieldID(mc_class, global::is_badlion ? "ag" : "field_71429_W", "I");
			//env->DeleteLocalRef(mc_class);

			env->SetIntField(mc, fid, count);
		}
	}
	void set_r_click_delay(int ticks)
	{
		if (global::is_lunar)
		{
			static const auto& field = mcc::impl::ave::fields[53];
			jfieldID fid = env->GetFieldID(mcc::ave, field.name, field.sig);

			env->SetIntField(mc, fid, ticks);
		}
		else {
			auto mc_class = env->GetObjectClass(mc);
			jfieldID fid = env->GetFieldID(mc_class, global::is_badlion ? "ap" : "field_71467_ac", "I");
			env->DeleteLocalRef(mc_class);

			env->SetIntField(mc, fid, ticks);
		}
	}

	jobject getrenderviewentity()
	{
		auto mc_class = env->GetObjectClass(mc);
		auto mid = env->GetMethodID(mc_class, global::is_badlion ? "ac" : "func_175606_aa", global::is_badlion ? "()Lpk;" : "()Lnet/minecraft/entity/Entity;");
		env->DeleteLocalRef(mc_class);

		return env->CallObjectMethod(mc, mid);
	}
	jobject getPlayer()
	{
		if (global::is_lunar)
		{
			static auto v = mapper->searchItem(mcc::impl::ave::methods, "bridge$getPlayer");

			auto mid = env->GetMethodID(mcc::ave, v.name, v.sig);

			return env->CallObjectMethod(mc, mid);
		}
		else {

			auto mc_class = env->GetObjectClass(mc);
			jfieldID fid = env->GetFieldID(mc_class, global::is_badlion ? "h" : "field_71439_g", global::is_badlion ? "Lbew;" : "Lnet/minecraft/client/entity/EntityPlayerSP;");
			env->DeleteLocalRef(mc_class);

			return env->GetObjectField(mc, fid);
		}
	}

	jobject getWorld()
	{
		if (global::is_lunar)
		{
			const auto& mc_class = mcc::ave;

			static auto v = mapper->searchItem(mcc::impl::ave::methods, "bridge$getWorld");

			static jmethodID mid = env->GetMethodID(mc_class, v.name, v.sig);

			return env->CallObjectMethod(mc, mid);
			//jfieldID world_fid = env->GetFieldID(mc_class, "hhspeeahhahaspshhhhahpeap", "Lnet/minecraft/v1_8/peppepaesshaaepseheeasapa;");
			//jobject world_obj = env->GetObjectField(mc, world_fid);

			//return world_obj;
		}
		else {

			auto mc_class = env->GetObjectClass(mc);
			jfieldID fid = env->GetFieldID(mc_class, global::is_badlion ? "f" : "field_71441_e", global::is_badlion ? "Lbdb;" : "Lnet/minecraft/client/multiplayer/WorldClient;");
			env->DeleteLocalRef(mc_class);

			return env->GetObjectField(mc, fid);
		}
	}

	jobject get_object()
	{
		return mc;
	}
	jobject getTimer()
	{
		if (global::is_lunar)
		{	
			static auto v = mapper->searchItem(mcc::impl::ave::methods, "bridge$getTimer");
			auto mid = env->GetMethodID(mcc::ave, v.name, v.sig);

			return env->CallObjectMethod(mc, mid);
		}
		else {
			jclass mc_class = env->GetObjectClass(mc);
			jfieldID fid = env->GetFieldID(mc_class, global::is_badlion ? "Y" : "field_71428_T", global::is_badlion ? "Lavl;" : "Lnet/minecraft/util/Timer;");
			env->DeleteLocalRef(mc_class);

			return env->GetObjectField(mc, fid);
		}
	}
	jobject getGameSettings()
	{
		if (global::is_lunar)
		{
			auto mc_class = env->GetObjectClass(mc);

			//static const auto& method = mapper->searchItem(mcc::impl::ave::methods, "bridge$getGameSettings");

			//auto mid = env->GetMethodID(mc_class, method.name, method.sig);
			//env->DeleteLocalRef(mc_class);

			//return env->CallObjectMethod(mc, mid);
			//auto mc_class = env->GetObjectClass(mc);

			///45: paaaesapaheepheaashsppsse | sig: Lnet/minecraft/v1_8/haehsehspsphppssaeshspapa;
			static const auto& field = mcc::impl::ave::fields[45];
			jfieldID fid = env->GetFieldID(mc_class, field.name, field.sig);
			env->DeleteLocalRef(mc_class);

			return env->GetObjectField(mc, fid);
		}
		else {
			auto mc_class = env->GetObjectClass(mc);
			jfieldID fid = env->GetFieldID(mc_class, global::is_badlion ? "t" : "field_71474_y", global::is_badlion ? "Lavh;" : "Lnet/minecraft/client/settings/GameSettings;");
			env->DeleteLocalRef(mc_class);

			return env->GetObjectField(mc, fid);
		}
	}

	jobject getCurrentScreen()
	{
		if (global::is_lunar)
		{
			static auto mc_class = mcc::ave;//env->GetObjectClass(mc);
			static const auto& method = mapper->searchItem(mcc::impl::ave::methods, "bridge$getCurrentScreen");

			auto mid = env->GetMethodID(mc_class, method.name, method.sig);


			return env->CallObjectMethod(mc, mid);

			//auto mc_class = env->GetObjectClass(mc);
			//jfieldID fid = env->GetFieldID(mc_class, "aepapapehhshppppaasehaaps", "Lnet/minecraft/v1_8/hhaeahpapsasshsahshasphhs;");
			//env->DeleteLocalRef(mc_class);

			//jobject ret = env->GetObjectField(mc, fid);
			//return ret;
		}
		else {
			auto mc_class = env->GetObjectClass(mc);
			jfieldID fid = env->GetFieldID(mc_class, global::is_badlion ? "m" : "field_71462_r", global::is_badlion ? "Laxu;" : "Lnet/minecraft/client/gui/GuiScreen;");
			env->DeleteLocalRef(mc_class);

			jobject ret = env->GetObjectField(mc, fid);
			return ret;
		}

	}
	jobject getRenderManager()
	{

		jfieldID fid;
		if(global::is_lunar)
		{
			static auto mc_class = mcc::ave;
			static auto v = mcc::impl::ave::fields[23];
			fid = env->GetFieldID(mc_class, v.name, v.sig);
		}
		else
		{
			static auto mc_class = env->GetObjectClass(mc);
			fid = env->GetFieldID(mc_class, global::is_badlion ? "aa" : "field_175616_W", global::is_badlion ? "Lbiu;" : "Lnet/minecraft/client/renderer/entity/RenderManager;");
		}


		return env->GetObjectField(mc, fid);
	}
	

	jobject getFontRenderer()
	{
		if (global::is_lunar)
		{
			//auto mc_class = env->GetObjectClass(mc);

			//static const auto& method = mapper->searchItem(mcc::impl::ave::methods, "bridge$getFontRenderer");

			//auto mid = env->GetMethodID(mc_class, method.name, method.sig);
			//env->DeleteLocalRef(mc_class);

			//return env->CallObjectMethod(mc, mid);

			auto mc_class = mcc::ave;
			static const auto& field = mcc::impl::ave::fields[32];
			jfieldID fid = env->GetFieldID(mc_class, field.name, field.sig);

			return env->GetObjectField(mc, fid);
		}
		else {
			auto mc_class = env->GetObjectClass(mc);
			jfieldID mid = env->GetFieldID(mc_class, global::is_badlion ? "k" : "field_71466_p", global::is_badlion ? "Lavn;" : "Lnet/minecraft/client/gui/FontRenderer;");
			env->DeleteLocalRef(mc_class);

			return env->GetObjectField(mc, mid);
		}
	}

	jobject objectMouseOver()
	{
		auto mc_class = env->GetObjectClass(mc);
		jfieldID mid = env->GetFieldID(mc_class, global::is_badlion ? "s" : "field_71476_x", global::is_badlion ? "Lauh;" : "Lnet/minecraft/util/MovingObjectPosition;");
		env->DeleteLocalRef(mc_class);

		return env->GetObjectField(mc, mid);
	}

	void set_objectMouseOver(jobject obj)
	{
		auto mc_class = env->GetObjectClass(mc);
		jfieldID mid = env->GetFieldID(mc_class, global::is_badlion ? "s" : "field_71476_x", global::is_badlion ? "Lauh;" : "Lnet/minecraft/util/MovingObjectPosition;");
		env->DeleteLocalRef(mc_class);

		env->SetObjectField(mc, mid, obj);
	}

	jobject get_pointed_entity()
	{
		if (global::is_lunar)
		{
			auto mc_class = mcc::ave;
			//  public phsephaaeeeeahheeshpahaas sepphpappaaasehhpeaehehap;
			static auto field = mcc::impl::ave::fields[28];
			jfieldID fid = env->GetFieldID(mc_class, field.name, field.sig);

			return env->GetObjectField(mc, fid);
		}
		else {
			auto mc_class = env->GetObjectClass(mc);
			jfieldID fid = env->GetFieldID(mc_class, global::is_badlion ? "i" : "field_147125_j", global::is_badlion ? "Lpk;" : "Lnet/minecraft/entity/Entity;");
			env->DeleteLocalRef(mc_class);

			return env->GetObjectField(mc, fid);
		}
	}

	void reset_pointedentity()
	{
		if (global::is_lunar)
		{
			auto mc_class = mcc::ave;
			//public phsephaaeeeeahheeshpahaas sepphpappaaasehhpeaehehap;
			jfieldID fid = env->GetFieldID(mc_class, "aseaspseashheasshphheeaaa", "Lnet/minecraft/v1_8/asheepepesehepppssaapseee;");

			env->SetObjectField(mc, fid, NULL);
		}
		else {
			auto mc_class = env->GetObjectClass(mc);
			jfieldID fid = env->GetFieldID(mc_class, global::is_badlion ? "i" : "field_147125_j", global::is_badlion ? "Lpk;" : "Lnet/minecraft/entity/Entity;");
			env->DeleteLocalRef(mc_class);

			env->SetObjectField(mc, fid, NULL);
		}
	}


	// to fix
	void set_pointedentity(jobject obj)
	{
		auto mc_class = env->GetObjectClass(mc);
		jfieldID fid = env->GetFieldID(mc_class, global::is_badlion ? "i" : "field_147125_j", global::is_badlion ? "Lpk;" : "Lnet/minecraft/entity/Entity;");
		env->DeleteLocalRef(mc_class);

		env->SetObjectField(mc, fid, obj);
	}

	jobject get_render_item()
	{
		auto mc_class = env->GetObjectClass(mc);
		jfieldID fid = env->GetFieldID(mc_class, global::is_badlion ? "ac" : "field_175620_Y", global::is_badlion ? "Lbfn;" : "Lnet/minecraft/client/renderer/ItemRenderer;");
		env->DeleteLocalRef(mc_class);

		return env->GetObjectField(mc, fid);
	}

	jobject get_renderglobal()
	{

		if(global::is_lunar)
		{
			auto mc_class = mcc::ave;
			static auto field = mcc::impl::ave::fields[22];
			jfieldID fid = env->GetFieldID(mc_class, field.name, field.sig);

			return env->GetObjectField(mc, fid);
		}
		else {
			auto mc_class = env->GetObjectClass(mc);
			jfieldID fid = env->GetFieldID(mc_class, global::is_badlion ? "g" : "field_71438_f", global::is_badlion ? "Lbfr;" : "Lnet/minecraft/client/renderer/RenderGlobal;");

			env->DeleteGlobalRef(mc_class);
			
			return env->GetObjectField(mc, fid);
		}

	}

	jobject get_scaled_resolution()
	{
		if (global::is_lunar)
		{
			jclass cls = mcc::ScaledResolution;

			//static std::string nig = jvmutil::getClassName(mcc::ave, true);

			//std::stringstream ss;
			//ss << "(" << nig << ")V";

			//jmethodID mid = env->GetMethodID(cls, "<init>", ss.str().c_str()); //"(Lnet/minecraft/v1_8/eshhespaaeaaassepsphhases;)V"
			static jmethodID mid = env->GetMethodID(cls, "<init>", "(Lnet/minecraft/v1_8/eehhpheaeaehehehapaeshhpe;)V");
			jobject newObj = env->NewObject(cls, mid, mc);

			return newObj;
		}
		else {
			if (!global::V_3)
			{
				jclass launch = env->FindClass("net/minecraft/launchwrapper/Launch");
				jfieldID sfid = env->GetStaticFieldID(launch, "classLoader", "Lnet/minecraft/launchwrapper/LaunchClassLoader;");
				jobject classLoader = env->GetStaticObjectField(launch, sfid);

				env->DeleteLocalRef(launch);

				auto cls = env->GetObjectClass(classLoader);
				jstring name = env->NewStringUTF(global::is_badlion ? "avr" : "net.minecraft.client.gui.ScaledResolution");
				static jmethodID mid1 = env->GetMethodID(cls, "findClass", "(Ljava/lang/String;)Ljava/lang/Class;");

				jclass c = (jclass)env->CallObjectMethod(classLoader, mid1, name);

				static jmethodID mid = env->GetMethodID(c, "<init>", global::is_badlion ? "(Lave;)V" : "(Lnet/minecraft/client/Minecraft;)V");
				jobject newObj = env->NewObject(c, mid, mc);

				env->DeleteLocalRef(c);
				env->DeleteLocalRef(name);
				env->DeleteLocalRef(cls);

				return newObj;
			}
			else {

				jclass cls = env->FindClass(global::is_badlion ? "avr" : "net.minecraft.client.gui.ScaledResolution");


				jmethodID mid = env->GetMethodID(cls, "<init>", global::is_badlion ? "(Lave;)V" : "(Lnet/minecraft/client/Minecraft;)V");
				jobject newObj = env->NewObject(cls, mid, mc);


				env->DeleteLocalRef(cls);

				return newObj;
			}
		}

	}
};

#endif
#pragma once

#include <jni.h>
#include "functions.hpp"

namespace packets {


	namespace C03PacketPlayer {

		__forceinline jobject init(bool onGround)
		{
			if (global::is_lunar)
			{
				jclass c = mcc::C03PacketPlayerPosition;
				jmethodID mid = global::jenv->GetMethodID(c, "<init>", "(Z)V");
				jobject newObj = global::jenv->NewObject(c, mid, onGround);

				return newObj;
			}
			else {
				jclass c = getObject(global::jenv, global::is_badlion ? "ip" : "net.minecraft.network.play.client.C03PacketPlayer");
				jmethodID mid = global::jenv->GetMethodID(c, "<init>", "(Z)V");
				jobject newObj = global::jenv->NewObject(c, mid, onGround);

				global::jenv->DeleteLocalRef(c);

				return newObj;
			}
		}

	}

	namespace C07PacketPlayerDigging {

		__forceinline jobject getAction()
		{
			jclass clSTATUS = getObject(global::jenv, global::is_badlion ? "ir$a" : "net.minecraft.network.play.client.C07PacketPlayerDigging$Action");
			jfieldID fidONE = global::jenv->GetStaticFieldID(clSTATUS, global::is_badlion ? "f" : "RELEASE_USE_ITEM", global::is_badlion ? "Lir$a;" : "Lnet.minecraft.network.play.client.C07PacketPlayerDigging$Action;");

			jobject STATUS_ONE = global::jenv->GetStaticObjectField(clSTATUS, fidONE);

			global::jenv->DeleteLocalRef(clSTATUS);

			return STATUS_ONE;
		}

		__forceinline jobject getEnum()
		{
			jclass clSTATUS = getObject(global::jenv, global::is_badlion ? "cq" : "net.minecraft.util.EnumFacing");
			jfieldID fidONE = global::jenv->GetStaticFieldID(clSTATUS, global::is_badlion ? "a" :"DOWN", global::is_badlion ? "Lcq;" : "Lnet.minecraft.util.EnumFacing;");
			jobject STATUS_ONE = global::jenv->GetStaticObjectField(clSTATUS, fidONE);

			global::jenv->DeleteLocalRef(clSTATUS);

			return STATUS_ONE;
		}

		__forceinline jobject getBlockPos()
		{
			jclass clSTATUS = getObject(global::jenv, global::is_badlion ? "cj" : "net.minecraft.util.BlockPos");
			jfieldID fidONE = global::jenv->GetStaticFieldID(clSTATUS, global::is_badlion ? "a" :"ORIGIN", global::is_badlion ? "Lcj;" : "Lnet.minecraft.util.BlockPos;");
			jobject STATUS_ONE = global::jenv->GetStaticObjectField(clSTATUS, fidONE);

			global::jenv->DeleteLocalRef(clSTATUS);

			return STATUS_ONE;
			//jclass c = getObject(global::jenv, "net.minecraft.util.BlockPos");
			//jmethodID mid = global::jenv->GetMethodID(c, "<init>", "(DDD)V");
			//
			//jfieldID fid = global::jenv->GetFieldID(c, "field_177992_a", "Lnet.minecraft.util.BlockPos;");

			//return global::jenv->GetObjectField(c, fid);
		}


		__forceinline jobject getPosition()
		{
			jclass clSTATUS = getObject(global::jenv, global::is_badlion ? "cj" : "net.minecraft.util.BlockPos");
			jfieldID fidONE = global::jenv->GetFieldID(clSTATUS, "field_177992_a", global::is_badlion ? ":cj;" : "Lnet.minecraft.util.BlockPos;");

			jobject STATUS_ONE = global::jenv->GetObjectField(clSTATUS, fidONE);

			global::jenv->DeleteLocalRef(clSTATUS);

			return STATUS_ONE;
		}

		__forceinline jobject init(std::unique_ptr<c_world>* _world_)
		{
			jclass c = getObject(global::jenv, global::is_badlion ? "ir" : "net.minecraft.network.play.client.C07PacketPlayerDigging");

			//Lnet.minecraft.network.play.client.C07PacketPlayerDigging$Action;Lnet.minecraft.util.BlockPos;Lnet.minecraft.util.EnumFacing;
			jmethodID mid = global::jenv->GetMethodID(c, "<init>", global::is_badlion ? "(Lir$a;Lcj;Lcq;)V" : "(Lnet.minecraft.network.play.client.C07PacketPlayerDigging$Action;Lnet.minecraft.util.BlockPos;Lnet.minecraft.util.EnumFacing;)V");

			auto enum_ = getEnum();
			//std::cout << "Enum packet address: " << std::hex << "0x" << enum_ << std::endl;
			auto action_ = getAction();
			//std::cout << "Action packet address: " << std::hex << "0x" << action_ << std::endl;
			auto blockpos_ = _world_->get()->get_blocknigger(vec3({ 0, 0, 0 }));//getBlockPos();
			//std::cout << "Blockpos packet address: " << std::hex << "0x" << blockpos_ << std::endl;


			jobject newObj = global::jenv->NewObject(c, mid, action_, blockpos_, enum_);
			// mid, getAction(action), getBlockPos(), getEnum()

			//jfieldID facing_f = global::jenv->GetFieldID(global::jenv->GetObjectClass(newObj), global::is_badlion ? "ir" : "b", global::is_badlion ? "cq" : "Lnet.minecraft.util.EnumFacing;");
			//jfieldID status_f = global::jenv->GetFieldID(global::jenv->GetObjectClass(newObj), global::is_badlion ? "ir" : "c", global::is_badlion ? "ir$a" : "Lnet.minecraft.network.play.client.C07PacketPlayerDigging$Action;");
			//jfieldID position_f = global::jenv->GetFieldID(global::jenv->GetObjectClass(newObj), global::is_badlion ? "ir" : "a", global::is_badlion ? "cj" : "Lnet.minecraft.util.BlockPos;");



			//global::jenv->SetObjectField(newObj, facing_f, enum_);
			//global::jenv->SetObjectField(newObj, status_f, action_);
			//global::jenv->SetObjectField(newObj, position_f, blockpos_);


			global::jenv->DeleteLocalRef(c);

			return newObj;
		}

		__forceinline jobject init(JNIEnv* env, jobject action, jobject blockpos, jobject _enum_)
		{
			static jclass c = getObject(env, global::is_badlion ? "ir" : "net.minecraft.network.play.client.C07PacketPlayerDigging");

			static jmethodID mid = env->GetMethodID(c, "<init>", global::is_badlion ? "(Lir$a;Lcj;Lcq;)V" : "(Lnet/minecraft/network/play/client/C07PacketPlayerDigging$Action;Lnet/minecraft/util/BlockPos;Lnet/minecraft/util/EnumFacing;)V");

			jobject newObj = env->NewObject(c, mid, action, blockpos, _enum_);

		//	global::jenv->DeleteLocalRef(c);

			return newObj;
		}
	}

	namespace C08PacketPlayerBlockPlacement {

		__forceinline jobject init(jobject itemstack)
		{
			jclass c = getObject(global::jenv, global::is_badlion ? "ja" : "net.minecraft.network.play.client.C08PacketPlayerBlockPlacement");
			jmethodID mid = global::jenv->GetMethodID(c, "<init>", global::is_badlion ? "(Lzx;)V" : "(Lnet/minecraft/item/ItemStack;)V");

			jobject newObj = global::jenv->NewObject(c, mid, itemstack);

			global::jenv->DeleteLocalRef(c);

			return newObj;
		}
	}

	namespace C04PacketPlayerPosition {

		__forceinline jobject init(double x, double y, double z, bool onground)
		{
			if (global::is_lunar)
			{
				jclass c = mcc::C04PacketPlayerPosition;
				static jmethodID mid = global::jenv->GetMethodID(c, "<init>", "(DDDZ)V");

				jobject newObj = global::jenv->NewObject(c, mid, (double)x, (double)y, (double)z, onground);

				return newObj;
			}
			else {
				jclass c = getObject(global::jenv, global::is_badlion ? "ip$a" : "net.minecraft.network.play.client.C03PacketPlayer$C04PacketPlayerPosition");
				jmethodID mid = global::jenv->GetMethodID(c, "<init>", "(DDDZ)V");

				jobject newObj = global::jenv->NewObject(c, mid, (double)x, (double)y, (double)z, onground);

				global::jenv->DeleteLocalRef(c);

				return newObj;
			}
		}
	}

	namespace C06PacketPlayerPosLook {


		__forceinline jobject init( double playerX, double playerY, double playerZ, float playerYaw, float playerPitch, bool playerIsOnGround )
		{
			if (global::is_lunar)
			{
				jclass klass_ = mcc::C06PacketPlayerPosition;
				jmethodID mid = global::jenv->GetMethodID(klass_, "<init>", "(DDDFFZ)V");

				jobject newObj = global::jenv->NewObject(klass_, mid, playerX, playerY, playerZ, playerYaw, playerPitch, playerIsOnGround);

				return newObj;
			}
			else {
				jclass klass_ = getObject(global::jenv, global::is_badlion ? "ip$b" : "net.minecraft.network.play.client.C03PacketPlayer$C06PacketPlayerPosLook");
				jmethodID mid = global::jenv->GetMethodID(klass_, "<init>", "(DDDFFZ)V");

				jobject newObj = global::jenv->NewObject(klass_, mid, playerX, playerY, playerZ, playerYaw, playerPitch, playerIsOnGround);

				global::jenv->DeleteLocalRef(klass_);

				return newObj;
			}
		}
	}
}
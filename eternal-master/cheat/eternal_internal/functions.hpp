#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <Windows.h>
#include <jni.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <thread>
#include "Minecraft.hpp"
#include "global.hpp"
#include <fstream>
#include <jni.h>

inline jobject mc;
inline jobject thePlayer;
inline jobject theWorld;

inline jobject renderViewEntity;
inline jobject pointedEntity;
inline jobject boundingBox;
inline jobject hitVec;
inline jobject ridingEntity;

inline jdouble xCoord;
inline jdouble yCoord;
inline jdouble zCoord;

inline jfieldID thePlayerf;
inline jfieldID renderViewEntityf;
inline jfieldID pointedEntityf;
inline jfieldID objectMouseOverf;
inline jfieldID boundingBoxf;
inline jfieldID ridingEntityf;
inline jfieldID xCoordf;
inline jfieldID yCoordf;
inline jfieldID zCoordf;
inline jfieldID renderPartialTicksf;
inline jfieldID timerf;
inline jfieldID hitVecf;
inline jfieldID playerControllerf;
inline jfieldID entityHitf;

inline jmethodID rayTracef;
inline jmethodID getPositionf;
inline jmethodID getLookf;
inline jmethodID addVectorf;
inline jmethodID getEntitiesWithinAABBExcludingEntityf;
inline jmethodID sizef;
inline jmethodID getf;
inline jmethodID canBeCollidedWithf;
inline jmethodID getCollisionBorderSizef;
inline jmethodID expandf;
inline jmethodID calculateInterceptf;
inline jmethodID isVecInsidef;
inline jmethodID distanceTof;
inline jmethodID addCoordf;
inline jmethodID attackEntityf;
inline jmethodID movingObjectPositionc;
inline jmethodID isUsingItemf;

inline jclass world;
inline jclass Vec3;
inline jclass entityLivingBaseClass;
inline jclass Entity;
inline jclass List;
inline jclass AxisAlignedBB;
inline jclass MovingObjectPosition;
inline jclass EntityRenderer;
inline jclass PlayerControllerMP;
inline jclass Timer;
inline jclass EntityPlayer;

typedef jint(*hJNI_GetCreatedJavaVMss)(JavaVM** vmBuf, jsize bufLen, jsize* nVMs);
inline hJNI_GetCreatedJavaVMss oJNI_GetCreatedJavaVMss;


__forceinline JNIEnv* getJNI()
{
	JavaVM* jvm;
	JNIEnv* jenv;

	HMODULE jvmHandle = GetModuleHandleA("jvm.dll");
	hJNI_GetCreatedJavaVMss oJNI_GetCreatedJavaVMss = (hJNI_GetCreatedJavaVMss)GetProcAddress(jvmHandle, "JNI_GetCreatedJavaVMs");
	oJNI_GetCreatedJavaVMss(&jvm, 1, NULL);
	jvm->AttachCurrentThread((void**)&jenv, NULL);

	return jenv;
}


__forceinline jobject getClassLoader(JNIEnv* env)
{
	//jclass launch = env->FindClass("net/minecraft/launchwrapper/Launch");
	//auto class_loader = env->FindClass("java/lang/ClassLoader");
	//auto thread_class = env->FindClass("java/lang/Thread");
	//jfieldID sfid = env->GetStaticFieldID(thread_class, "classLoader", "Lnet/minecraft/launchwrapper/LaunchClassLoader;");


	jclass launch = env->FindClass("net/minecraft/launchwrapper/Launch");
	//jfieldID sfid = env->GetStaticFieldID(launch, "classLoader", "Lnet/minecraft/launchwrapper/LaunchClassLoader;");
	jfieldID sfid = env->GetStaticFieldID(launch, "classLoader", "Lnet/minecraft/launchwrapper/LaunchClassLoader;");

	jobject classLoader = env->GetStaticObjectField(launch, sfid);
	env->DeleteLocalRef(launch);

	return classLoader;
}

__forceinline jclass getObject(JNIEnv* env, const char* className)
{
	if (!global::V_3)
	{
		jstring name = env->NewStringUTF(className);

		jobject classLoader = getClassLoader(env);
		jmethodID mid = env->GetMethodID(env->GetObjectClass(classLoader), "findClass", "(Ljava/lang/String;)Ljava/lang/Class;");

		jclass c = (jclass)env->CallObjectMethod(classLoader, mid, name);
		env->DeleteLocalRef(name);

		return c;
	}
	else {
		std::string s = className;
		std::replace(s.begin(), s.end(), '.', '/');

		jclass c = (jclass)env->FindClass(s.c_str());

		return c;
	}


}

__forceinline jobject getMC(JNIEnv* env)
{

	if (global::is_lunar)
	{
		static field_t field = mcc::impl::ave::fields[10];
		jfieldID mc_fid = env->GetStaticFieldID(mcc::ave, field.name, field.sig);
	
		jobject Minecraft = env->GetStaticObjectField(mcc::ave, mc_fid);

		return Minecraft;
		//jfieldID mc_fid = env->GetStaticFieldID(mcc::ave, "ppeaasppahpspaseeaahseehs", "Lnet/minecraft/v1_8/eshhespaaeaaassepsphhases;");
		//jobject Minecraft = env->GetStaticObjectField(mcc::ave, mc_fid);

	}
	else {
		jclass mcClass = getObject(env, global::is_badlion ? "ave" : "net.minecraft.client.Minecraft");
		jmethodID smid = env->GetStaticMethodID(mcClass, global::is_badlion ? "A" : "func_71410_x", global::is_badlion ? "()Lave;" : "()Lnet/minecraft/client/Minecraft;");

		jobject b = env->CallStaticObjectMethod(mcClass, smid);
		env->DeleteLocalRef(mcClass);
		return b;
	}

}

__forceinline void postPreInit(JNIEnv* env)
{
	mc = getMC(env);
//	mc = env->NewGlobalRef(mc);
	auto minekraft = std::make_unique<c_minecraft>(getMC(env), env);

	thePlayer = minekraft->getPlayer();
//	thePlayer = env->NewGlobalRef(thePlayer);
	theWorld = minekraft->getWorld();
//	theWorld = env->NewGlobalRef(theWorld);

	//entityLivingBaseClass = getObject(env, "net.minecraft.entity.EntityLivingBase");
	//Vec3 = getObject(env, "net.minecraft.util.Vec3");
	//world = getObject(env, "net.minecraft.world.World");
	//List = getObject(env, "java.util.List");
	//Entity = getObject(env, "net.minecraft.entity.Entity");
	//EntityPlayer = getObject(env, "net.minecraft.entity.player.EntityPlayer");
	//AxisAlignedBB = getObject(env, "net.minecraft.util.AxisAlignedBB");
	MovingObjectPosition = getObject(env, global::is_badlion ? "auh" : "net.minecraft.util.MovingObjectPosition");
	//EntityRenderer = getObject(env, "net.minecraft.client.renderer.EntityRenderer");
	//PlayerControllerMP = getObject(env, "net.minecraft.client.multiplayer.PlayerControllerMP");
	//Timer = getObject(env, "net.minecraft.util.Timer");

	//jclass mcClass = getObject(env, "net.minecraft.client.Minecraft");

	//renderViewEntityf = env->GetFieldID(mcClass, "field_71451_h", "Lnet/minecraft/entity/EntityLivingBase;");
	//pointedEntityf = env->GetFieldID(EntityRenderer, "field_78528_u", "Lnet/minecraft/entity/Entity;");
	//objectMouseOverf = env->GetFieldID(mcClass, "field_71476_x", "Lnet/minecraft/util/MovingObjectPosition;");
	//boundingBoxf = env->GetFieldID(Entity, "field_70121_D", "Lnet/minecraft/util/AxisAlignedBB;");
	hitVecf = env->GetFieldID(MovingObjectPosition, global::is_badlion ? "c" : "field_72307_f", global::is_badlion ? "Laui;" : "Lnet/minecraft/util/Vec3;");
	//ridingEntityf = env->GetFieldID(Entity, "field_70154_o", "Lnet/minecraft/entity/Entity;");
	//xCoordf = env->GetFieldID(Vec3, "field_72450_a", "D");
	//yCoordf = env->GetFieldID(Vec3, "field_72448_b", "D");
	//zCoordf = env->GetFieldID(Vec3, "field_72449_c", "D");
	//renderPartialTicksf = env->GetFieldID(Timer, "field_74281_c", "F");
	//timerf = env->GetFieldID(mcClass, "field_71428_T", "Lnet/minecraft/util/Timer;");
	//playerControllerf = env->GetFieldID(mcClass, "field_71442_b", "Lnet/minecraft/client/multiplayer/PlayerControllerMP;");
	//entityHitf = env->GetFieldID(MovingObjectPosition, "field_72308_g", "Lnet/minecraft/entity/Entity;");

	//rayTracef = env->GetMethodID(entityLivingBaseClass, "func_70614_a", "(DF)Lnet/minecraft/util/MovingObjectPosition;");
	//getPositionf = env->GetMethodID(entityLivingBaseClass, "func_70666_h", "(F)Lnet/minecraft/util/Vec3;");
	//getLookf = env->GetMethodID(entityLivingBaseClass, "func_70676_i", "(F)Lnet/minecraft/util/Vec3;");
	//addVectorf = env->GetMethodID(Vec3, "func_72441_c", "(DDD)Lnet/minecraft/util/Vec3;");
	//getEntitiesWithinAABBExcludingEntityf = env->GetMethodID(world, "func_72839_b", "(Lnet/minecraft/entity/Entity;Lnet/minecraft/util/AxisAlignedBB;)Ljava/util/List;");
	//sizef = env->GetMethodID(List, "size", "()I");
	//getf = env->GetMethodID(List, "get", "(I)Ljava/lang/Object;");
	//canBeCollidedWithf = env->GetMethodID(Entity, "func_70067_L", "()Z");
	//getCollisionBorderSizef = env->GetMethodID(Entity, "func_70111_Y", "()F");
	//expandf = env->GetMethodID(AxisAlignedBB, "func_72314_b", "(DDD)Lnet/minecraft/util/AxisAlignedBB;");
	//addCoordf = env->GetMethodID(AxisAlignedBB, "func_72321_a", "(DDD)Lnet/minecraft/util/AxisAlignedBB;");
	//calculateInterceptf = env->GetMethodID(AxisAlignedBB, "func_72327_a", "(Lnet/minecraft/util/Vec3;Lnet/minecraft/util/Vec3;)Lnet/minecraft/util/MovingObjectPosition;");
	//isVecInsidef = env->GetMethodID(AxisAlignedBB, "func_72318_a", "(Lnet/minecraft/util/Vec3;)Z");
	//distanceTof = env->GetMethodID(Vec3, "func_72438_d", "(Lnet/minecraft/util/Vec3;)D");
	//isUsingItemf = env->GetMethodID(EntityPlayer, "func_71039_bw", "()Z");
	//movingObjectPositionc = env->GetMethodID(MovingObjectPosition, "<init>", "(Lnet/minecraft/entity/Entity;Lnet/minecraft/util/Vec3;)V");

	//env->DeleteLocalRef(mcClass);

}

__forceinline jobject init_axisalignedbb(JNIEnv* env, double minX, double minY, double minZ, double maxX, double maxY, double maxZ)
{
	if(global::is_lunar)
	{
		jclass axis_class = mcc::AxisAlignedBB;
		jmethodID mid = env->GetMethodID(axis_class, "<init>", "(DDDDDD)V");

		jobject newObj = env->NewObject(axis_class, mid, minX, minY, minZ, maxX, maxY, maxZ);

		return newObj;
	}
	else {
		jclass axis_class = getObject(env, global::is_badlion ? "aug" : "net.minecraft.util.AxisAlignedBB");
		jmethodID mid = env->GetMethodID(axis_class, "<init>", "(DDDDDD)V");

		jobject newObj = env->NewObject(axis_class, mid, minX, minY, minZ, maxX, maxY, maxZ);
		env->DeleteLocalRef(axis_class);

		return newObj;
	}

}
__forceinline void cleanup(JNIEnv* env)
{
	env->DeleteLocalRef(mc);
	env->DeleteLocalRef(thePlayer);
	env->DeleteLocalRef(theWorld);
	env->DeleteLocalRef(MovingObjectPosition);
//	env->DeleteGlobalRef(mc);
//	env->DeleteGlobalRef(thePlayer);
//	env->DeleteGlobalRef(theWorld);
}

#endif
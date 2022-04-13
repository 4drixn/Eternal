#pragma once

#include "vec3.hpp"

class timer {
private:
	jobject timer_obj;
	JNIEnv* env;
public:
	timer(jobject timer_)
	{
		timer_obj = timer_;
		env = getJNI();
	}
	~timer()
	{
		env->DeleteLocalRef(timer_obj);
	}

	float get_timer_speed()
	{
		if (global::is_lunar)
		{
			auto this_class = mcc::Timer;
			static auto field = mcc::impl::Timer::fields[4];
			jfieldID fid = env->GetFieldID(this_class, "peeeehaapeseaaeeaahssahps", "F"); //peeeehaapeseaaeeaahssahps
			//jfieldID fid = env->GetFieldID(this_class, "peeeehaapeseaaeeaahssahps", "F"); //peeeehaapeseaaeeaahssahps

			return env->GetFloatField(timer_obj, fid);
		}
		else {
			auto this_class = env->GetObjectClass(timer_obj);
			jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "d" : "field_74278_d", "F");
			env->DeleteLocalRef(this_class);

			return env->GetFloatField(timer_obj, fid);
			// peeeehaapeseaaeeaahssahps
		}
	}
	void set_timer_speed(const float val)
	{
		if (global::is_lunar)
		{
			static auto field = mcc::impl::Timer::fields[4];
			jfieldID fid = env->GetFieldID(mcc::Timer, field.name, "F"); //peeeehaapeseaaeeaahssahps
			//jfieldID fid = env->GetFieldID(this_class, "peeeehaapeseaaeeaahssahps", "F"); //peeeehaapeseaaeeaahssahps

			env->SetFloatField(timer_obj, fid, val);
		}
		else {
			auto this_class = env->GetObjectClass(timer_obj);
			jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "d" : "field_74278_d", "F");
			env->DeleteLocalRef(this_class);

			env->SetFloatField(timer_obj, fid, val);
		}
	}

	void set_ticksPerSecond(const float val)
	{
		if (global::is_lunar)
		{
			static auto field = mcc::impl::Timer::fields[0];
			jfieldID fid = env->GetFieldID(mcc::Timer, field.name, "F"); //peeeehaapeseaaeeaahssahps
			//jfieldID fid = env->GetFieldID(this_class, "hasphshsehseahahphhesepeh", "F");

			env->SetFloatField(timer_obj, fid, val);
		}
		else {
			auto this_class = env->GetObjectClass(timer_obj);
			jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "a" : "field_74282_a", "F");
			env->DeleteLocalRef(this_class);

			env->SetFloatField(timer_obj, fid, val);
		}
	}

	float get_ticksPerSecond()
	{
		if (global::is_lunar)
		{
			static auto field = mcc::impl::Timer::fields[0];
			jfieldID fid = env->GetFieldID(mcc::Timer, field.name, "F");

			return env->GetFloatField(timer_obj, fid);
		}
		else {
			auto this_class = env->GetObjectClass(timer_obj);
			jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "a" : "field_74282_a", "F");
			env->DeleteLocalRef(this_class);

			return env->GetFloatField(timer_obj, fid);
		}
	}
	
	float get_renderPartialTicks()
	{
		if (global::is_lunar)
		{
			static auto field = mcc::impl::Timer::fields[3];
			jfieldID fid = env->GetFieldID(mcc::Timer, field.name, "F");

			return env->GetFloatField(timer_obj, fid);
		}
		else {
			auto this_class = env->GetObjectClass(timer_obj);
			jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "c" : "field_74281_c", "F");
			env->DeleteLocalRef(this_class);

			return env->GetFloatField(timer_obj, fid);
		}
	}
	void set_elapsedTicks(const int val)
	{
		if (global::is_lunar)
		{
			auto this_class = mcc::Timer;
			static auto field = mcc::impl::Timer::fields[5];
			jfieldID fid = env->GetFieldID(this_class, field.name, "F"); //peeeehaapeseaaeeaahssahps

			env->SetFloatField(timer_obj, fid, val);
		}
		else {
			auto this_class = env->GetObjectClass(timer_obj);
			jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "b" : "field_74280_b", "I");
			env->DeleteLocalRef(this_class);

			env->SetFloatField(timer_obj, fid, val);
		}
	}
};
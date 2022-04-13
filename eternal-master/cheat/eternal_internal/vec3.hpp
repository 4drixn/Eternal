#pragma once
#include <utility>
#include <math.h>
#include <cmath>
#include <cfenv>

#include "functions.hpp"
#include "global.hpp"
#define M_PI 3.14159265358979323846

struct vec3 {
	float x, y, z;

	auto distanceTo(const vec3 othervec) -> float
	{
		auto d0 = static_cast<float>(othervec.x - this->x);
		auto d1 = static_cast<float>(othervec.y - this->y);
		auto d2 = static_cast<float>(othervec.z - this->z);
		
		return sqrtf(d0 * d0 + d1 * d1 + d2 * d2);
	}
};

class c_vec3 {

private: 
	jobject vectorobj;
	JNIEnv* env;
public:
	c_vec3(jobject obj)
	{
		vectorobj = obj;
		env = getJNI();
	}

	vec3 get_native_vec3()
	{
		auto this_clazz = env->GetObjectClass(vectorobj);

		auto X_fid = env->GetFieldID(this_clazz, global::is_badlion ? "a" : "field_72450_a", "D"),
			 Y_fid = env->GetFieldID(this_clazz, global::is_badlion ? "b" : "field_72448_b", "D"),
			 Z_fid = env->GetFieldID(this_clazz, global::is_badlion ? "c" : "field_72449_c", "D");

		env->DeleteLocalRef(this_clazz);

		vec3 vector{};
		vector.x = static_cast<float>(env->GetDoubleField(vectorobj, X_fid));
		vector.y = static_cast<float>(env->GetDoubleField(vectorobj, Y_fid));
		vector.z = static_cast<float>(env->GetDoubleField(vectorobj, Z_fid));

		return vector;
	}

	std::shared_ptr<c_vec3> addVector(double x, double y, double z)
	{
		vec3 native_vec3 = this->get_native_vec3();
		native_vec3.x += x;
		native_vec3.y += y;
		native_vec3.z += z;

		jclass axis_class = getObject(env, global::is_badlion ? "aui" : "net.minecraft.util.Vec3");
		jmethodID mid = env->GetMethodID(axis_class, "<init>", "(DDD)V");

		jobject newObj = env->NewObject(axis_class, mid, native_vec3.x, native_vec3.y, native_vec3.z);
		env->DeleteLocalRef(axis_class);

		return std::make_shared<c_vec3>(newObj);
	}

	jobject get_object()
	{
		return vectorobj;
	}
};

namespace util {
	inline float wrap_to_180(float value)
	{
		value = std::fmod(value + 180, 360);
		if (value < 0)
			value += 360;
		
		return value - 180;
	}

	inline float radiants_to_deg(float x)
	{
		return x * 180.f / M_PI;
	}

	inline float deg_to_radiants(float r)
	{
		return r * M_PI / 180;
	}

	inline std::pair<float, float> get_angles(vec3 pos, vec3 pos1)
	{
		double d_x = (double)pos1.x - (double)pos.x;
		double d_y = (double)pos1.y - (double)pos.y;
		double d_z = (double)pos1.z - (double)pos.z;

		double hypothenuse = sqrt(d_x * d_x + d_z * d_z);
		float yaw = radiants_to_deg(atan2(d_z, d_x)) - 90.f;
		float pitch = radiants_to_deg(-atan2(d_y, hypothenuse));

		return std::make_pair(yaw, pitch);
	}

	inline static double distance(double x, double y) {
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	inline static double distance(double x1, double y1, double z1, double x2, double y2, double z2) {
		return distance(y1 - y2, distance(x1 - x2, z1 - z2));
	}
}
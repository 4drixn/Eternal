#include "axisalignedbb.hpp"
#include "functions.hpp"
#include "Classes.hpp"

c_axisalignedbb::c_axisalignedbb(jobject obj, JNIEnv* env_)
{
	axis_aligned_obj = obj;
	env = env_;
}

c_axisalignedbb::~c_axisalignedbb()
{
	env->DeleteLocalRef(axis_aligned_obj);
}

s_axisalignedbb c_axisalignedbb::get_native_boundingbox()
{
	

	jfieldID minX_fid, minY_fid, minZ_fid;
	jfieldID maxX_fid, maxY_fid, maxZ_fid;

	if (global::is_lunar)
	{
		auto this_clazz = mcc::AxisAlignedBB;
		static auto minx = mcc::impl::AxisAlignedBB::fields[0];
		static auto miny = mcc::impl::AxisAlignedBB::fields[1];
		static auto minz = mcc::impl::AxisAlignedBB::fields[2];

		static auto maxx = mcc::impl::AxisAlignedBB::fields[3];
		static auto maxy = mcc::impl::AxisAlignedBB::fields[4];
		static auto maxz = mcc::impl::AxisAlignedBB::fields[5];
		
		minX_fid = env->GetFieldID(this_clazz, minx.name,"D");
		minY_fid = env->GetFieldID(this_clazz, miny.name,"D");
		minZ_fid = env->GetFieldID(this_clazz, minz.name,"D");
														
		maxX_fid = env->GetFieldID(this_clazz, maxx.name,"D");
		maxY_fid = env->GetFieldID(this_clazz, maxy.name,"D");
		maxZ_fid = env->GetFieldID(this_clazz, maxz.name,"D");

	}
	else {
		auto this_clazz = env->GetObjectClass(axis_aligned_obj);
		minX_fid = env->GetFieldID(this_clazz, global::is_badlion ? "a" : "field_72340_a", "D");
		minY_fid = env->GetFieldID(this_clazz, global::is_badlion ? "b" : "field_72338_b", "D");
		minZ_fid = env->GetFieldID(this_clazz, global::is_badlion ? "c" : "field_72339_c", "D");

		maxX_fid = env->GetFieldID(this_clazz, global::is_badlion ? "d" : "field_72336_d", "D");
		maxY_fid = env->GetFieldID(this_clazz, global::is_badlion ? "e" : "field_72337_e", "D");
		maxZ_fid = env->GetFieldID(this_clazz, global::is_badlion ? "f" : "field_72334_f", "D");
		env->DeleteLocalRef(this_clazz);
	}


	s_axisalignedbb bb{};
	bb.minX = env->GetDoubleField(axis_aligned_obj, minX_fid);
	bb.minY = env->GetDoubleField(axis_aligned_obj, minY_fid);
	bb.minZ = env->GetDoubleField(axis_aligned_obj, minZ_fid);

	bb.maxX = env->GetDoubleField(axis_aligned_obj, maxX_fid);
	bb.maxY = env->GetDoubleField(axis_aligned_obj, maxY_fid);
	bb.maxZ = env->GetDoubleField(axis_aligned_obj, maxZ_fid);

	return bb;
}

void c_axisalignedbb::set_native_boundingbox(s_axisalignedbb target)
{
	set(target.minX, target.minY, target.minZ, target.maxX, target.maxY, target.maxZ);
}

jobject c_axisalignedbb::calculateIntercept(std::shared_ptr<c_vec3> first, std::shared_ptr<c_vec3> second)
{
	auto this_clazz = env->GetObjectClass(axis_aligned_obj);

	jmethodID mid = env->GetMethodID(this_clazz, global::is_badlion ? "a" : "func_72327_a", global::is_badlion ? "(Laui;Laui;)Lauh;" : "(Lnet/minecraft/util/Vec3;Lnet/minecraft/util/Vec3;)Lnet/minecraft/util/MovingObjectPosition;");

	env->DeleteLocalRef(this_clazz);

	return env->CallObjectMethod(axis_aligned_obj, mid, first->get_object(), second->get_object());
}

void c_axisalignedbb::set(double minX, double minY, double minZ, double maxX, double maxY, double maxZ)
{
	jfieldID minX_fid, minY_fid, minZ_fid;
	jfieldID maxX_fid, maxY_fid, maxZ_fid;

	if (global::is_lunar)
	{
		auto this_clazz = mcc::AxisAlignedBB;
		static auto minx = mcc::impl::AxisAlignedBB::fields[0];
		static auto miny = mcc::impl::AxisAlignedBB::fields[1];
		static auto minz = mcc::impl::AxisAlignedBB::fields[2];

		static auto maxx = mcc::impl::AxisAlignedBB::fields[3];
		static auto maxy = mcc::impl::AxisAlignedBB::fields[4];
		static auto maxz = mcc::impl::AxisAlignedBB::fields[5];

		minX_fid = env->GetFieldID(this_clazz, minx.name, "D");
		minY_fid = env->GetFieldID(this_clazz, miny.name, "D");
		minZ_fid = env->GetFieldID(this_clazz, minz.name, "D");

		maxX_fid = env->GetFieldID(this_clazz, maxx.name, "D");
		maxY_fid = env->GetFieldID(this_clazz, maxy.name, "D");
		maxZ_fid = env->GetFieldID(this_clazz, maxz.name, "D");
	}
	else {
		auto this_clazz = env->GetObjectClass(axis_aligned_obj);
		minX_fid = env->GetFieldID(this_clazz, global::is_badlion ? "a" : "field_72340_a", "D");
		minY_fid = env->GetFieldID(this_clazz, global::is_badlion ? "b" : "field_72338_b", "D");
		minZ_fid = env->GetFieldID(this_clazz, global::is_badlion ? "c" : "field_72339_c", "D");

		maxX_fid = env->GetFieldID(this_clazz, global::is_badlion ? "d" : "field_72336_d", "D");
		maxY_fid = env->GetFieldID(this_clazz, global::is_badlion ? "e" : "field_72337_e", "D");
		maxZ_fid = env->GetFieldID(this_clazz, global::is_badlion ? "f" : "field_72334_f", "D");

		env->DeleteLocalRef(this_clazz);
	}


	env->SetDoubleField(axis_aligned_obj, minX_fid, minX);
	env->SetDoubleField(axis_aligned_obj, minY_fid, minY);
	env->SetDoubleField(axis_aligned_obj, minZ_fid, minZ);

	env->SetDoubleField(axis_aligned_obj, maxX_fid, maxX);
	env->SetDoubleField(axis_aligned_obj, maxY_fid, maxY);
	env->SetDoubleField(axis_aligned_obj, maxZ_fid, maxZ);

}

std::shared_ptr<c_axisalignedbb> c_axisalignedbb::addCoord(double x, double y, double z)
{
	auto native = this->get_native_boundingbox();
	double d0 = native.minX;
	double d1 = native.minY;
	double d2 = native.minZ;
	double d3 = native.maxX;
	double d4 = native.maxY;
	double d5 = native.maxZ;

	if (x < 0.0f)
	{
		d0 += x;
	}
	else if (x > 0.0f)
	{
		d3 += x;
	}

	if (y < 0.0f)
	{
		d1 += y;
	}
	else if (y > 0.0f)
	{
		d4 += y;
	}

	if (z < 0.0f)
	{
		d2 += z;
	}
	else if (z > 0.0f)
	{
		d5 += z;
	}

	jclass axis_class = getObject(env, global::is_badlion ? "aug" : "net.minecraft.util.AxisAlignedBB");
	jmethodID mid = env->GetMethodID(axis_class, "<init>", "(DDDDDD)V");

	jobject newObj = env->NewObject(axis_class, mid, d0, d1, d2, d3, d4, d5);
	env->DeleteLocalRef(axis_class);

	return std::make_shared<c_axisalignedbb>(newObj, env);
}

bool c_axisalignedbb::isVecInside(std::shared_ptr<c_vec3> vec_)
{
	auto vec = vec_->get_native_vec3();
	auto native = this->get_native_boundingbox();

	return vec.x > native.minX && vec.x < native.maxX ? (vec.y > native.minY && vec.y < native.maxY ? vec.z > native.minZ && vec.z < native.maxZ : false) : false;
}


std::shared_ptr<c_axisalignedbb> c_axisalignedbb::expand(double x, double y, double z, double x2, double y2, double z2)
{
	auto native = this->get_native_boundingbox();
	double d0 = native.minX - x;
	double d1 = native.minY - y;
	double d2 = native.minZ - z;
	double d3 = native.maxX + x2;
	double d4 = native.maxY + y2;
	double d5 = native.maxZ + z2;

	jclass axis_class;
	if (global::is_lunar)
		axis_class = mcc::AxisAlignedBB;
	else
		axis_class = getObject(env, global::is_badlion ? "aug" : "net.minecraft.util.AxisAlignedBB");

	if (axis_class != nullptr) {
		jmethodID mid = env->GetMethodID(axis_class, "<init>", "(DDDDDD)V");

		jobject newObj = env->NewObject(axis_class, mid, d0, d1, d2, d3, d4, d5);

		if (!global::is_lunar)
			env->DeleteLocalRef(axis_class);

		return std::make_shared<c_axisalignedbb>(newObj, env);
	}
}


std::shared_ptr<c_axisalignedbb> c_axisalignedbb::expand(double x, double y, double z)
{
	auto native = this->get_native_boundingbox();
	double d0 = native.minX - x;
	double d1 = native.minY - y;
	double d2 = native.minZ - z;
	double d3 = native.maxX + x;
	double d4 = native.maxY + y;
	double d5 = native.maxZ + z;

	jclass axis_class = getObject(env, global::is_badlion ? "aug" : "net.minecraft.util.AxisAlignedBB");
	jmethodID mid = env->GetMethodID(axis_class, "<init>", "(DDDDDD)V");

	jobject newObj = env->NewObject(axis_class, mid, d0, d1, d2, d3, d4, d5);
	env->DeleteLocalRef(axis_class);

	return std::make_shared<c_axisalignedbb>(newObj, env);
}

std::shared_ptr<c_axisalignedbb> c_axisalignedbb::offset(double x, double y, double z)
{

	static jmethodID mid = nullptr;

	if (static bool once; !once) {

		auto this_clazz = global::is_lunar ? mcc::AxisAlignedBB : env->GetObjectClass(axis_aligned_obj);
		
		if (global::is_lunar) {
			static auto method = mapper->searchItem(mcc::impl::AxisAlignedBB::methods, "bridge$offset");
			mid = env->GetMethodID(this_clazz, method.name, method.sig);
		}
		else
			mid = env->GetMethodID(this_clazz, global::is_badlion ? "c" : "func_72317_d", global::is_badlion ? "(DDD)Laug;" : "(DDD)Lnet/minecraft/util/AxisAlignedBB;");

		if (!global::is_lunar)
			env->DeleteLocalRef(this_clazz);


		once = true;
	}

	return std::make_shared<c_axisalignedbb>(env->CallObjectMethod(axis_aligned_obj, mid, x, y, z), env);
}

jobject c_axisalignedbb::get_object() {
	return axis_aligned_obj;
}
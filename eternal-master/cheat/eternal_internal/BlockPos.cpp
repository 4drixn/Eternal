#include "BlockPos.hpp"
#include "functions.hpp"


c_blockpos::c_blockpos(jobject obj, JNIEnv* env_)
{
	env = env_;
	blockpos_obj = obj;
}

c_blockpos::~c_blockpos()
{
	env->DeleteLocalRef(blockpos_obj);
}

void c_blockpos::set_blockpos(int x, int y, int z)
{
	jclass this_clazz;
	//if (blockpos_obj == NULL)
	//{
	//	this_clazz = getObject(env, "net.minecraft.util.Vec3i");
	//	blockpos_obj = env->AllocObject(getObject(env, "net.minecraft.util.Vec3i"));
	//}
	//else {
		this_clazz = env->GetObjectClass(blockpos_obj);
	//}

	if (this_clazz != NULL)
	{
		this_clazz = env->GetObjectClass(blockpos_obj);
		s_blockpos bp{};
		jfieldID x_field, y_field, z_field = nullptr;

		if (global::is_lunar)
		{
			x_field = env->GetFieldID(this_clazz, "field_177962_a", "I");
			y_field = env->GetFieldID(this_clazz, "field_177960_b", "I");
			z_field = env->GetFieldID(this_clazz, "field_177961_c", "I");
		}
		else {
			x_field = env->GetFieldID(this_clazz, "field_177962_a", "I");
			y_field = env->GetFieldID(this_clazz, "field_177960_b", "I");
			z_field = env->GetFieldID(this_clazz, "field_177961_c", "I");
		}

		env->SetIntField(this_clazz, x_field, x);
		env->SetIntField(this_clazz, y_field, y);
		env->SetIntField(this_clazz, z_field, z);

		bp.x = env->GetIntField(this_clazz, x_field);
		bp.y = env->GetIntField(this_clazz, y_field);
		bp.z = env->GetIntField(this_clazz, z_field);
	}

}


jobject c_blockpos::get_object()
{
	return blockpos_obj;
}
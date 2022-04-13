#pragma once
#include "global.hpp"

struct bone_t
{
	float x;
	float y;
	float z;

	bone_t(float _x, float _y, float _z)
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
	}
	bone_t()
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}
};


class c_playermodel
{
private:
	jobject obj;
	JNIEnv* env;
public:

	c_playermodel(jobject object)
	{
		obj = object;
		env = global::jenv;
	}
	c_playermodel(jobject object, JNIEnv* Env)
	{
		obj = object;
		env = Env;
	}

	jobject get_object()
	{
		return obj;
	}


	std::vector<bone_t> get_bones()
	{
		if (obj != nullptr) 
		{
			jfieldID bipedHead_fid;
			jfieldID bipedRightArm_fid;
			jfieldID bipedLeftArm_fid;
			jfieldID bipedRightLeg_fid;
			jfieldID bipedLeftLeg_fid;

			// add other mappings
			if( global::is_lunar )
			{
				static auto bipedHead_field = mcc::impl::ModelPlayer::fields[0];
				static auto bipedRightArm_field = mcc::impl::ModelPlayer::fields[3];
				static auto bipedLeftArm_field = mcc::impl::ModelPlayer::fields[4];
				static auto bipedRightLeg_field = mcc::impl::ModelPlayer::fields[5];
				static auto bipedLeftLeg_field = mcc::impl::ModelPlayer::fields[6];

				bipedHead_fid = env->GetFieldID( mcc::ModelPlayer, bipedHead_field.name, bipedHead_field.sig );
				bipedRightArm_fid = env->GetFieldID( mcc::ModelPlayer, bipedRightArm_field.name, bipedRightArm_field.sig );
				bipedLeftArm_fid = env->GetFieldID( mcc::ModelPlayer, bipedLeftArm_field.name, bipedLeftArm_field.sig );
				bipedRightLeg_fid = env->GetFieldID( mcc::ModelPlayer, bipedRightLeg_field.name, bipedRightLeg_field.sig );
				bipedLeftLeg_fid = env->GetFieldID( mcc::ModelPlayer, bipedLeftLeg_field.name, bipedLeftLeg_field.sig );

			}
			else {
				auto this_clazz = env->GetObjectClass(obj);
				bipedHead_fid = env->GetFieldID( this_clazz, global::is_badlion ? "f" : "field_178720_f", global::is_badlion ? "Lbct;" : "Lnet/minecraft/client/model/ModelRenderer;" );
				bipedRightArm_fid = env->GetFieldID( this_clazz,  global::is_badlion ? "h" : "field_178723_h", global::is_badlion ? "Lbct;" : "Lnet/minecraft/client/model/ModelRenderer;");
				bipedLeftArm_fid = env->GetFieldID( this_clazz, global::is_badlion ? "i" : "field_178724_i", global::is_badlion ? "Lbct;" : "Lnet/minecraft/client/model/ModelRenderer;" );
				bipedRightLeg_fid = env->GetFieldID( this_clazz,  global::is_badlion ? "j" : "field_178721_j", global::is_badlion ? "Lbct;" : "Lnet/minecraft/client/model/ModelRenderer;" );
				bipedLeftLeg_fid = env->GetFieldID( this_clazz,  global::is_badlion ? "k" : "field_178722_k", global::is_badlion ? "Lbct;" : "Lnet/minecraft/client/model/ModelRenderer;" );
			}
			jobject bipedHead = env->GetObjectField(obj, bipedHead_fid);
			jobject bipedRightArm = env->GetObjectField(obj, bipedRightArm_fid);
			jobject bipedLeftArm = env->GetObjectField(obj, bipedLeftArm_fid);
			jobject bipedRightLeg = env->GetObjectField(obj, bipedRightLeg_fid);
			jobject bipedLeftLeg = env->GetObjectField(obj, bipedLeftLeg_fid);

			if (bipedHead && bipedRightArm && bipedLeftArm && bipedRightLeg && bipedLeftLeg)
			{
				auto ModelRenderer_cls = env->GetObjectClass(bipedHead);

				jint f_count;
				static jfieldID* fields = nullptr;
				static field_t rotateAngleX;
				static field_t rotateAngleY;
				static field_t rotateAngleZ;

				if (!fields) {
					jvmti->GetClassFields(ModelRenderer_cls, &f_count, &fields);

					char* field_name; char* field_name2; char* field_name3;
					char* signature; char* signature2; char* signature3;

					jvmti->GetFieldName(ModelRenderer_cls, fields[7], &field_name, &signature, nullptr);
					jvmti->GetFieldName(ModelRenderer_cls, fields[8], &field_name2, &signature2, nullptr);
					jvmti->GetFieldName(ModelRenderer_cls, fields[9], &field_name3, &signature3, nullptr);

					rotateAngleX.name = field_name; rotateAngleX.sig = signature;
					rotateAngleY.name = field_name2; rotateAngleY.sig = signature2;
					rotateAngleZ.name = field_name3; rotateAngleZ.sig = signature3;

				}

				auto rotateAngleX_fid = env->GetFieldID(ModelRenderer_cls, rotateAngleX.name, rotateAngleX.sig);
				auto rotateAngleY_fid = env->GetFieldID(ModelRenderer_cls, rotateAngleY.name, rotateAngleY.sig);
				auto rotateAngleZ_fid = env->GetFieldID(ModelRenderer_cls, rotateAngleZ.name, rotateAngleZ.sig);

				env->DeleteLocalRef(ModelRenderer_cls);

				std::vector<bone_t> bones
				{
					bone_t(env->GetFloatField(bipedHead, rotateAngleX_fid), env->GetFloatField(bipedHead, rotateAngleY_fid), env->GetFloatField(bipedHead, rotateAngleZ_fid) ),
					bone_t(env->GetFloatField(bipedRightArm, rotateAngleX_fid), env->GetFloatField(bipedRightArm, rotateAngleY_fid), env->GetFloatField(bipedRightArm, rotateAngleZ_fid) ),
					bone_t(env->GetFloatField(bipedLeftArm, rotateAngleX_fid), env->GetFloatField(bipedLeftArm, rotateAngleY_fid), env->GetFloatField(bipedLeftArm, rotateAngleZ_fid) ),
					bone_t(env->GetFloatField(bipedRightLeg, rotateAngleX_fid), env->GetFloatField(bipedRightLeg, rotateAngleY_fid), env->GetFloatField(bipedRightLeg, rotateAngleZ_fid) ),
					bone_t(env->GetFloatField(bipedLeftLeg, rotateAngleX_fid), env->GetFloatField(bipedLeftLeg, rotateAngleY_fid), env->GetFloatField(bipedLeftLeg, rotateAngleZ_fid) )
				};

				return bones;
			}
			std::vector<bone_t> bones;
			return bones;
		}
	}

};


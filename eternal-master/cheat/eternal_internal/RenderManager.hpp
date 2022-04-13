#include <jni.h>
#include "global.hpp"
#include "functions.hpp"

class c_rendermanager{
private:
	jobject rendermanager_obj;
	JNIEnv* env;

public:
	c_rendermanager(jobject obj)
	{
		rendermanager_obj = obj;
		env = global::jenv;
	}

	c_rendermanager(jobject obj, JNIEnv* jenv)
	{
		rendermanager_obj = obj;
		env = jenv;
	}

	jobject get_object()
	{
		return rendermanager_obj;
	}

	double renderPosX = 0.00f;
	double Get_renderPosX()
	{
		auto this_class = env->GetObjectClass(rendermanager_obj);
		//jclass CRenderManager = getObject(env, "net.minecraft.client.renderer.entity.RenderManager");
		jfieldID FIDrenderPosX = env->GetFieldID(this_class, global::is_badlion ? "o" : "field_78725_b", "D");
		//env->DeleteLocalRef(CRenderManager);
		env->DeleteLocalRef(this_class);
		return env->GetDoubleField(rendermanager_obj, FIDrenderPosX);
	}
	double renderPosY = 0.00f;
	double Get_renderPosY()
	{
		auto this_class = env->GetObjectClass(rendermanager_obj);

		jfieldID FIDrenderPosY = env->GetFieldID(this_class, global::is_badlion ? "p" : "field_78726_c", "D");

		env->DeleteLocalRef(this_class);
		return env->GetDoubleField(rendermanager_obj, FIDrenderPosY);
	}
	double renderPosZ = 0.00f;
	double Get_renderPosZ()
	{
		auto this_class = env->GetObjectClass(rendermanager_obj);

		jfieldID FIDrenderPosZ = env->GetFieldID(this_class, global::is_badlion ? "q" : "field_78723_d", "D");

		env->DeleteLocalRef(this_class);
		return env->GetDoubleField(rendermanager_obj, FIDrenderPosZ);
	}

	double Get_viewPosX()
	{
		auto this_class = env->GetObjectClass(rendermanager_obj);

		jfieldID FIDrenderPosY = env->GetFieldID(this_class, global::is_badlion ? "o" : "field_78730_l", "D");

		env->DeleteLocalRef(this_class);
		return env->GetDoubleField(rendermanager_obj, FIDrenderPosY);
	}

	double Get_viewPosY()
	{
		auto this_class = env->GetObjectClass(rendermanager_obj);
		//jclass CRenderManager = getObject(env, "net.minecraft.client.renderer.entity.RenderManager");
		jfieldID FIDrenderPosY = env->GetFieldID(this_class, global::is_badlion ? "j" : "field_78731_m", "D");
		//env->DeleteLocalRef(CRenderManager);
		env->DeleteLocalRef(this_class);
		return env->GetDoubleField(rendermanager_obj, FIDrenderPosY);
	}

	

	std::vector<field_t> _fields_;
	jint field_count;
	jfieldID* fields;

	glm::vec2 get_viewpos() { // playerView

		if (global::is_lunar) {
			static auto this_class = env->GetObjectClass(rendermanager_obj);
			if (this_class != nullptr)
			{

				if (_fields_.size() == 0) {
					if (static bool once = false; !once)
					{
						once = true;
						global::jvmti->GetClassFields(this_class, &field_count, &fields);
						for (int i = 0; i < field_count; i++)
						{
							auto field = fields[i];

							char* field_name;
							char* signature;

							global::jvmti->GetFieldName(this_class, field, &field_name, &signature, nullptr);

							jint accessFlags;
							global::jvmti->GetFieldModifiers(this_class, field, &accessFlags);

							field_t _field_{};

							if (accessFlags & 0x0008)
								_field_.is_static = true;
							else
								_field_.is_static = false;

							_field_.name = field_name;
							_field_.sig = signature;

							_fields_.push_back(_field_);

						}
					}
					else
					{
						if (_fields_.size() == 0) {
							once = false;
							//std::cout << "automapper err 1\n";
						}

					}
				}
				
				jfieldID X_fid, Y_fid, Z_fid;


				static field_t _x_ = _fields_[12];
				static field_t _y_ = _fields_[11];

				X_fid = env->GetFieldID(this_class, _x_.name, "F");
				Y_fid = env->GetFieldID(this_class, _y_.name, "F");


				return glm::vec2(env->GetFloatField(rendermanager_obj, X_fid), env->GetFloatField(rendermanager_obj, Y_fid));
			}
			else
			{
				this_class = env->GetObjectClass(rendermanager_obj);
			}

		}
		else
		{
			auto this_class = env->GetObjectClass(rendermanager_obj);
			jfieldID X_fid;
			jfieldID Y_fid;
				
			X_fid = env->GetFieldID(this_class, global::is_badlion ? "f" : "field_78732_j", "F");
			Y_fid = env->GetFieldID(this_class, global::is_badlion ? "e" : "field_78735_i", "F");

			env->DeleteLocalRef(this_class);

			return glm::vec2(env->GetFloatField(rendermanager_obj, X_fid), env->GetFloatField(rendermanager_obj, Y_fid));
		}

	}
	
	glm::vec3 get_render_pos() {

		static auto this_class = env->GetObjectClass(rendermanager_obj);
		if (this_class != nullptr)
		{
			if (static bool once = false; !once)
			{
				once = true;
				global::jvmti->GetClassFields(this_class, &field_count, &fields);
				for (int i = 0; i < field_count; i++)
				{
					auto field = fields[i];

					char* field_name;
					char* signature;

					global::jvmti->GetFieldName(this_class, field, &field_name, &signature, nullptr);

					jint accessFlags;
					global::jvmti->GetFieldModifiers(this_class, field, &accessFlags);

					field_t _field_{};

					if (accessFlags & 0x0008)
						_field_.is_static = true;
					else
						_field_.is_static = false;

					_field_.name = field_name;
					_field_.sig = signature;

					_fields_.push_back(_field_);

				}
			}
			else
			{
				if (_fields_.size() == 0) {
					once = false;
					std::cout << "automapper err 1\n";
				}
				
			}
			jfieldID X_fid, Y_fid, Z_fid;


			static field_t _x_ = _fields_[4];
			static field_t _y_ = _fields_[5];
			static field_t _z_ = _fields_[6];

			if (global::is_lunar)
			{
				X_fid = env->GetFieldID(this_class, _x_.name, "D");
				Y_fid = env->GetFieldID(this_class, _y_.name, "D");
				Z_fid = env->GetFieldID(this_class, _z_.name, "D");
			}
			else
			{
				X_fid = env->GetFieldID(this_class, global::is_badlion ? "o" : "field_78725_b", "D");
				Y_fid = env->GetFieldID(this_class, global::is_badlion ? "p" : "field_78726_c", "D");
				Z_fid = env->GetFieldID(this_class, global::is_badlion ? "q" : "field_78723_d", "D");
			}

			return glm::vec3(env->GetDoubleField(rendermanager_obj, X_fid), env->GetDoubleField(rendermanager_obj, Y_fid), env->GetDoubleField(rendermanager_obj, Z_fid));
		}
		else
		{
			this_class = env->GetObjectClass(rendermanager_obj);
		}
	}

	jobject getEntityRenderObject(std::shared_ptr<c_player> entity)
	{

		if (!rendermanager_obj || !entity)
			return nullptr;
		
		jmethodID mid = nullptr;

		if (global::is_lunar) {
			
			static auto this_clazz = mcc::RenderManager;
			
			mid = env->GetMethodID(this_clazz, "paeehpahsespesapaepaahaps", "(Lnet/minecraft/v1_8/espphaeeepaspshsppspasehe;)Lnet/minecraft/v1_8/sheeaehshsspaeeseeaehhpps;");
		}
		else {
			auto this_clazz = env->GetObjectClass(rendermanager_obj);

			if (this_clazz) {
				//	mid = env->GetMethodID(this_clazz, global::is_badlion ? "b" : "func_177087_b", global::is_badlion ? "()Lbbo;" : "()Lnet/minecraft/client/model/ModelBase;");
				mid = env->GetMethodID(this_clazz, global::is_badlion ? "a" : "func_78713_a", global::is_badlion ? "(Lpk;)Lbiv;" : "(Lnet/minecraft/entity/Entity;)Lnet/minecraft/client/renderer/entity/Render;");
				env->DeleteLocalRef(this_clazz);
			}
		}

		//this.skinMap.get(s);
		if(mid && entity->get_object())
			return env->CallObjectMethod(rendermanager_obj, mid, entity->get_object());	
	}

	
};
#include "global.hpp"

class c_itemrenderer {
private:
	jobject itemrenderer_obj;
	JNIEnv* env;

public:
	c_itemrenderer(jobject obj)
	{
		itemrenderer_obj = obj;
		env = getJNI();
	}
	c_itemrenderer(jobject obj, JNIEnv* jnienv)
	{
		itemrenderer_obj = obj;
		env = jnienv;
	}
	~c_itemrenderer()
	{
		env->DeleteLocalRef(itemrenderer_obj);
	}

	void set_equippedProgress(float val)
	{
		auto this_class = env->GetObjectClass(itemrenderer_obj);
		jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "e" : "field_78454_c", "F");
		env->DeleteLocalRef(this_class);
		
		env->SetFloatField(itemrenderer_obj, fid, val);
	}
	//renderItemAndEffectIntoGUI	func_180450_b	b	(Lnet/minecraft/item/ItemStack;II)V

	void renderItemAndEffectIntoGUI(jobject ItemStack, int x, int y )
	{
		jmethodID mid;

		if (itemrenderer_obj != nullptr) {
			if (global::is_lunar)
			{

			}
			else
			{
				///if (static bool once = false; !once) {
				auto this_class = env->GetObjectClass(itemrenderer_obj);
				//mid = env->GetMethodID(this_class, global::is_badlion ? "b" : "func_180450_b", global::is_badlion ? "(Lzx;II)V" : "(Lnet/minecraft/item/ItemStack;II)V");
				//env->DeleteLocalRef(this_class);

				//}

			}
			//if (mid)
			//	env->CallVoidMethod(itemrenderer_obj, mid, ItemStack, x, y);
		}
	}

	jobject get_object()
	{
		return itemrenderer_obj;
	}
};
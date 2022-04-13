#include "functions.hpp"
#include "IInventory.hpp"

class c_container {
private:
	jobject container_obj;
	JNIEnv* env;
public:
	c_container(jobject obj)
	{
		container_obj = obj;
		env = getJNI();//global::jenv;
	}

	std::shared_ptr<c_IInventory> getLowerChestInventory()
	{
		jclass lol = getObject(env, global::is_badlion ? "xo" : "net.minecraft.inventory.ContainerChest");
		jmethodID mid = env->GetMethodID(lol, global::is_badlion ? "e" : "func_85151_d", global::is_badlion ? "()Log;" : "()Lnet/minecraft/inventory/IInventory;");
		env->DeleteLocalRef(lol);

		return std::make_shared<c_IInventory>(env->CallObjectMethod(container_obj, mid));
	}
	bool is_chest()
	{
		jclass c = getObject(env, global::is_badlion ? "xo" : "net.minecraft.inventory.ContainerChest");
		jboolean b = env->IsInstanceOf(container_obj, c);
		env->DeleteLocalRef(c);

		return b;
	}
	void slotClick(jobject player, int slot)
	{
		jclass lol = env->GetObjectClass(container_obj);
		jmethodID mid = env->GetMethodID(lol, global::is_badlion ? "a" : "func_75144_a", global::is_badlion ? "(IIILwn;)Lzx;" : "(IIILnet/minecraft/entity/player/EntityPlayer;)Lnet/minecraft/item/ItemStack;");
		env->DeleteLocalRef(lol);

		env->CallObjectMethod(container_obj, mid, slot, 0, 4, player);
	}

	int get_windowId()
	{
		auto container_class = env->GetObjectClass(container_obj);
		jfieldID fid = env->GetFieldID(container_class, global::is_badlion ? "d" : "field_75152_c", "I");
		env->DeleteLocalRef(container_class);

		return env->GetIntField(container_obj, fid);
	}

	jobject get_object()
	{
		return container_obj;
	}

	std::shared_ptr<c_IInventory> get_IInventory()
	{
		// func_75139_a	a	(I)Lnet/minecraft/inventory/Slot;
		auto container_class = env->GetObjectClass(container_obj);
		jfieldID fid = env->GetFieldID(container_class, global::is_badlion ? "a" : "field_145900_a", global::is_badlion ? "Log;" : "Lnet/minecraft/inventory/IInventory;");
		env->DeleteLocalRef(container_class);


		return std::make_shared<c_IInventory>(env->GetObjectField(container_class, fid));
	}

	std::shared_ptr<c_itemstack> get_stack_in_slot(const int slot)
	{
		// func_75139_a	a	(I)Lnet/minecraft/inventory/Slot;
		auto container_class = env->GetObjectClass(container_obj);
		jmethodID mid = env->GetMethodID(container_class, global::is_badlion ? "a" : "func_75139_a", global::is_badlion ? "(I)Lyg;" : "(I)Lnet/minecraft/inventory/Slot;");
		env->DeleteLocalRef(container_class);

		jobject Slot = env->CallObjectMethod(container_obj, mid, slot);
		if (Slot == NULL)
		{
			return nullptr;
		}
		auto slot_class = env->GetObjectClass(Slot);

		jmethodID mid2 = env->GetMethodID(slot_class, global::is_badlion ? "d" : "func_75211_c", global::is_badlion ? "()Lzx;" : "()Lnet/minecraft/item/ItemStack;");
		jobject ItemStack = env->CallObjectMethod(Slot, mid2);
		//std::cout << "[DEBUG] Slot " << slot << " : " << env->CallObjectMethod(Slot, mid2) << std::endl;
		env->DeleteLocalRef(slot_class);

		return std::make_shared<c_itemstack>(ItemStack);
	}

};
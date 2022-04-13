#include "GuiScreen.hpp"

c_guiscreen::c_guiscreen(const jobject& obj)
{
	guiscreen_obj = obj;
	env = getJNI();
}

c_guiscreen::~c_guiscreen()
{
	env->DeleteLocalRef(guiscreen_obj);
}

bool c_guiscreen::chat_open()
{
	jclass c = global::is_lunar
		? mcc::GuiChat
		: getObject(env, global::is_badlion ? "awv" : "net.minecraft.client.gui.GuiChat");
	jboolean b = env->IsInstanceOf(guiscreen_obj, c);

	if (!global::is_lunar)
		env->DeleteLocalRef(c);

	return b;
}

bool c_guiscreen::inventory_open()
{
	jclass c = global::is_lunar
		? mcc::GuiInventory
		: getObject(env, global::is_badlion ? "azc" : "net.minecraft.client.gui.inventory.GuiInventory");
	jboolean b = env->IsInstanceOf(guiscreen_obj, c);

	if (!global::is_lunar)
		env->DeleteLocalRef(c);

	return b;
}

jobject c_guiscreen::get_object()
{
	return guiscreen_obj;
}

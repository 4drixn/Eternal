#include "main.hpp"
#include <jni.h>
#include "global.hpp"
#include "functions.hpp"

class c_gui : public singleton<c_gui>
{
private:
	JNIEnv* env;
public:

	c_gui()
	{
		env = getJNI();
	}

	void drawRect(glm::vec4 box, float thickness = 1.f, ImVec4 color = ImVec4(1, 1, 1, 1))
	{

		// + thickness
		this->drawRect(box.x - (thickness / 2), box.y, box.x + (thickness / 2), box.y, 0xFFFFFF); // top line
		this->drawRect(box.x, box.y, box.x, box.w, 0xFFFFFF); // left vertical line
		this->drawRect(box.x, box.y, box.x, box.w, 0xFFFFFF); // right vertical line
		this->drawRect(box.x, box.w, box.z, box.w, 0xFFFFFF); // bottom line

		
		this->drawRect(box.x, box.y, box.z, box.w, 0xFFFFFF);
	}

	void drawRect(int left, int top, int right, int bottom, int color)
	{
		if(global::is_lunar)
		{
			static auto cls = mcc::GuiClass;
			
			jmethodID mid = env->GetStaticMethodID(cls , "hhheehsephpeephphaeeahhsa", "(IIIII)V");

			env->CallStaticVoidMethod(cls, mid, left, top, right, bottom, color);
		}
		else {
			jclass c = getObject(env, global::is_badlion ? "avp" : "net.minecraft.client.gui.Gui");
			jmethodID mid;
			if (global::is_lunar)
			{
				mid = env->GetStaticMethodID(c, global::is_badlion ? "a" : "func_73734_a", "(IIIII)V");
			}
			else {
				mid = env->GetStaticMethodID(c, global::is_badlion ? "a" : "func_73734_a", "(IIIII)V");
			}

			if(c && mid)
				env->CallStaticVoidMethod(c, mid, left, top, right, bottom, color);
		}
		
	}

	void drawHead(jobject skin, int x, int y)
	{

	}



};
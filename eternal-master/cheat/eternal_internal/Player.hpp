#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "vec3.hpp"
#include "functions.hpp"
#include "AxisAlignedBB.hpp"
#include "ItemStack.hpp"
#include "IChatComponent.hpp"
#include "utils.hpp"
#include "Container.hpp"
#include "c_playermodel.h"
#include "NetHandlerPlayClient.hpp"
#include "IInventory.hpp"
#include "PotionEffect.hpp"

class c_player {

private:
	jobject player;
	JNIEnv* env;

public:

	c_player(jobject player_, JNIEnv* env_)
	{
		player = player_;
		env = env_;
	}
	~c_player()
	{
		env->DeleteLocalRef(player);
	}

	bool get_isdead()
	{
		static jfieldID fid;
		if (static bool once = false; !once) {
			if (global::is_lunar)
			{
				static const auto& field = mcc::impl::EntityLivingBase::fields[30];
				fid = env->GetFieldID(mcc::EntityLivingBase, field.name, field.sig);
			}
			else
			{
				static jclass this_class = env->GetObjectClass(player);
				fid = env->GetFieldID(this_class, global::is_badlion ? "aP" : "field_70729_aU", "Z");
			}
			once = true;
		}

		return env->GetBooleanField(player, fid);
		// 30
	}

	bool isAlive() // wrapper function
	{
		return !this->get_isdead() && this->get_health() > 0.0F;
	}
	
	jobject get_player_controller_lunar()
	{
		if (!global::is_lunar)
			return nullptr;

		static auto method = mapper->searchItem(mcc::impl::ave::methods, "bridge$getPlayerController");

		jmethodID mid = env->GetMethodID(mcc::ave, method.name, method.sig);

		return env->CallObjectMethod(mc, mid);
		
	}
	std::string get_str_between_two_str(const std::string& s,
		const std::string& start_delim,
		const std::string& stop_delim)
	{
		unsigned first_delim_pos = s.find(start_delim);
		unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
		unsigned last_delim_pos = s.find(stop_delim);

		return s.substr(end_pos_of_first_delim,
			last_delim_pos - end_pos_of_first_delim);
	}

	bool canBeCollidedWith()
	{
		auto this_class = env->GetObjectClass(player);
		jmethodID mid = env->GetMethodID(this_class, global::is_badlion ? "ad" : "func_70067_L", "()Z");
		env->DeleteLocalRef(this_class);

		return env->CallBooleanMethod(player, mid);
	}

	int getItemInUseDuration()
	{
		auto this_class = env->GetObjectClass(player);
		jmethodID mid = env->GetMethodID(this_class, global::is_badlion ? "bT" : "func_71057_bx", "()I");
		env->DeleteLocalRef(this_class);

		return env->CallIntMethod(player, mid);
	}

	void set_lastReportedPosY(float val)
	{
		auto this_class = env->GetObjectClass(player);
		jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "bL" : "field_175166_bJ", "D");
		env->DeleteLocalRef(this_class);

		env->SetDoubleField(player, fid, val);

	}
	float get_height()
	{
		auto this_class = env->GetObjectClass(player);
		jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "K" : "field_70131_O", "F");
		env->DeleteLocalRef(this_class);

		return env->GetFloatField(player, fid);
	}

	float get_move_speed(jobject potion_obj)
	{
		float baseSpeed = 0.2873f;
		if (this->isPotionActive(1))
		{
			int amplifier = 0;
			auto p_effect = getActivePotionEffect(potion_obj);

			if (p_effect->get_object() != nullptr)
			{
				baseSpeed *= (1.f + 0.2f * (p_effect->get_amplifier() + 1));
			}
		}

		return baseSpeed;
	}

	bool isSwingInProgress()
	{
		auto this_class = env->GetObjectClass(player);

		jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "ar" : "field_82175_bq", "Z");
		env->DeleteLocalRef(this_class);

		return env->GetBooleanField(player, fid);
	}

	void forward(float length, float y)
	{
		float yaw = this->getDirection();
		vec3 pos = this->get_position();

		this->set_pos(vec3({ (float)pos.x + (-sin(yaw) * length), (float)pos.y + y, (float)pos.z + (cos(yaw) * length) }), false);
	}

	int getSpeedEffect(jobject potion_obj)
	{
		int ret = 0;
		if (this->isPotionActive(1))
		{
			int amplifier = 0;
			auto p_effect = getActivePotionEffect(potion_obj);
			if (p_effect->get_object() != NULL)
			{
				ret = p_effect->get_amplifier() + 1;
			}
		}

		return ret;
	}

	float getJumpMotion()
	{
		vec3 oldmotion = this->get_motion_vector();
		this->jump();

		double jumpMotion = this->get_motion_vector().y;

		this->SetmotionY(oldmotion.y);
		this->SetmotionX(oldmotion.x);
		this->SetmotionZ(oldmotion.z);

		return jumpMotion;
	}

	std::shared_ptr<c_potioneffect> getActivePotionEffect(jobject potion_obj)
	{
		auto this_class = env->GetObjectClass(player);
		jmethodID mid;

		if (global::is_lunar)
			mid = env->GetMethodID(this_class, "ehpephsapseheashhhheheapa", "(Lnet/minecraft/v1_8/phepeepepheaasespaheppheh;)Lnet/minecraft/v1_8/eeaaepaaphspaesseappphesp;");
		else
			mid = env->GetMethodID(this_class, global::is_badlion ? "b" : "func_70660_b", global::is_badlion ? "(Lpe;)Lpf;" : "(Lnet/minecraft/potion/Potion;)Lnet/minecraft/potion/PotionEffect;");
		env->DeleteLocalRef(this_class);
		if (mid == nullptr)
			return nullptr;

		return std::make_shared<c_potioneffect>(env->CallObjectMethod(player, mid, potion_obj));
	}

	bool isPotionActive(int id)
	{
		auto this_class = env->GetObjectClass(player);
		jmethodID mid;

		if (global::is_lunar)
			mid = env->GetMethodID(this_class, "hehpehespashppheasaapphhh", "(I)Z");
		else
			mid = env->GetMethodID(this_class, global::is_badlion ? "k" : "func_82165_m", "(I)Z");

		if (mid == nullptr)
			return 0x0;
		
		env->DeleteLocalRef(this_class);

		return env->CallBooleanMethod(player, mid, id);
	}

	void set_inairspeed(float speed)
	{
		auto this_class = env->GetObjectClass(player);

		jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "bF" : "field_71102_ce", "F");
		env->DeleteLocalRef(this_class);

		env->SetFloatField(player, fid, speed);
	}

	bool is_hypixel_teammate(const std::shared_ptr<c_player>* player)
	{
		std::string local_unformatted_name = this->getDisplayName()->getFormattedText();
		std::string player_unformatted_name = player->get()->getDisplayName()->getFormattedText();

		std::string one = get_str_between_two_str(local_unformatted_name, "[", "]");
		std::string two = get_str_between_two_str(player_unformatted_name, "[", "]");

		if (one == two)
		{
			return true;
		}

		return false;
	}

	void set_newPos(vec3 pos)
	{
		auto this_class = env->GetObjectClass(player);
		jfieldID pos_x_fid, pos_y_fid, pos_z_fid;

		pos_x_fid = env->GetFieldID(this_class, global::is_badlion ? "bd" : "field_70709_bj", "D");
		pos_y_fid = env->GetFieldID(this_class, global::is_badlion ? "be" : "field_70710_bk", "D");
		pos_z_fid = env->GetFieldID(this_class, global::is_badlion ? "bf" : "field_110152_bk", "D");

		env->DeleteLocalRef(this_class);

		env->SetDoubleField(player, pos_x_fid, pos.x);
		env->SetDoubleField(player, pos_y_fid, pos.y);
		env->SetDoubleField(player, pos_z_fid, pos.z);
	}
	vec3 get_serverPos()
	{
		auto this_class = env->GetObjectClass(player);
		jfieldID pos_x_fid, pos_y_fid, pos_z_fid;

		pos_x_fid = env->GetFieldID(this_class, global::is_badlion ? "bW" : "field_70118_ct", "D");
		pos_y_fid = env->GetFieldID(this_class, global::is_badlion ? "bX" : "field_70117_cu", "D");
		pos_z_fid = env->GetFieldID(this_class, global::is_badlion ? "bY" : "field_70116_cv", "D");

		env->DeleteLocalRef(this_class);

		return vec3{
			(float)env->GetDoubleField(player, pos_x_fid),
			(float)env->GetDoubleField(player, pos_y_fid),
			(float)env->GetDoubleField(player, pos_z_fid)
		};
	}

	std::shared_ptr<c_container> getInventoryContainer()
	{
		auto this_class = env->GetObjectClass(player);
		jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "bj" : "field_71069_bz", global::is_badlion ? "Lxi;" : "Lnet/minecraft/inventory/Container;");
		env->DeleteLocalRef(this_class);

		return std::make_shared<c_container>(env->GetObjectField(player, fid));
	}

	std::shared_ptr<c_IInventory> getInventory()
	{
		auto this_class = env->GetObjectClass(player);
		jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "bi" : "field_71071_by", global::is_badlion ? "Lwn;" : "Lnet/minecraft/entity/player/InventoryPlayer;");
		env->DeleteLocalRef(this_class);

		return std::make_shared<c_IInventory>(env->GetObjectField(player, fid));
	}

	// Wrapper
	void shift_click(std::shared_ptr<c_container> openContainer, int slot)
	{
		this->window_click(openContainer->get_windowId(), slot, 0, 1);
	}

	// Another Wrapper
	void enequip(std::shared_ptr<c_container> openContainer, int slot)
	{
		this->window_click(openContainer->get_windowId(), slot, 0, 0);
	}
	void drop_2(std::shared_ptr<c_container> openContainer, int slot)
	{
		this->window_click(openContainer->get_windowId(), slot, 1, 4);
	}

	// haha window click and window drop are the same im retarded lool
	void window_click(int a, int b, int c, int d)
	{
		// field 11 - mc::ave - playercontroller
		auto meeeemcc = env->GetObjectClass(mc);
		jfieldID fid = env->GetFieldID(meeeemcc, global::is_badlion ? "c" : "field_71442_b", global::is_badlion ? "Lbda;" : "Lnet/minecraft/client/multiplayer/PlayerControllerMP;");

		jobject playerconntroller = global::is_lunar ? get_player_controller_lunar() : env->GetObjectField(mc, fid);
		auto this_clazz = env->GetObjectClass(playerconntroller);

		jmethodID click_ = env->GetMethodID(this_clazz, global::is_badlion ? "a" : "func_78753_a", global::is_badlion ? "(IIIILwn;)Lzx;" : "(IIIILnet/minecraft/entity/player/EntityPlayer;)Lnet/minecraft/item/ItemStack;");

		env->CallObjectMethod(playerconntroller, click_, a, b, c, d, this->get_object());
		env->DeleteLocalRef(playerconntroller);
		env->DeleteLocalRef(this_clazz);
		env->DeleteLocalRef(meeeemcc);

	}

	std::shared_ptr<c_container> get_openContainer()
	{
		auto p = env->GetObjectClass(player);
		jfieldID fid = env->GetFieldID(p, global::is_badlion ? "bk" : "field_71070_bA", global::is_badlion ? "Lxi;" : "Lnet/minecraft/inventory/Container;");
		env->DeleteLocalRef(p);

		return std::make_shared<c_container>(env->GetObjectField(player, fid));
	}

	bool isUsingItem()
	{
		auto this_class = env->GetObjectClass(player);
		jmethodID mid;

		if (global::is_lunar) {
			//auto mthod = mcc::impl::EntityPlayer::methods[156];
			mid = env->GetMethodID(this_class, "hhhhspppsepapaappsaasesap", "()Z");
		}
		else {
			mid = env->GetMethodID(this_class, global::is_badlion ? "bS" : "func_71039_bw", "()Z");
		}
		env->DeleteLocalRef(this_class);

		return env->CallBooleanMethod(player, mid);
	}

	void set_isUsingItem(bool state)
	{
		auto this_class = env->GetObjectClass(player);
		auto fid = env->GetFieldID(this_class, global::is_badlion ? "g" : "field_71074_e", "Z");
		env->DeleteLocalRef(this_class);

		env->SetBooleanField(player, fid, state);
	}

	void set_sprint_timer(int val)
	{
		auto this_class = env->GetObjectClass(player);
		jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "d" : "field_71156_d", "I");
		env->DeleteLocalRef(this_class);

		env->SetIntField(player, fid, val);
	}
	float get_moveforward()
	{
		auto this_class = env->GetObjectClass(player);
		jfieldID fid;
		if (global::is_lunar) {
			static auto field = mcc::impl::EntityLivingBase::fields[41];
			fid = env->GetFieldID(this_class, field.name, "F");
		}
		else {
			fid = env->GetFieldID(this_class, global::is_badlion ? "ba" : "field_70701_bs", "F");
		}

		env->DeleteLocalRef(this_class);

		return env->GetFloatField(player, fid);
	}
	__forceinline float getSpeed() {
		vec3 cur_motion = this->get_motion_vector();
		return (float)sqrt(cur_motion.x * cur_motion.x + cur_motion.z * cur_motion.z);

	}

	int get_ticksExisted()
	{
		auto this_class = env->GetObjectClass(player);
		jfieldID fid = NULL;

		if (global::is_lunar) {
			static auto field = mcc::impl::Entity::fields[45];
			fid = env->GetFieldID(this_class, field.name, "I");
		}
		else
			fid = env->GetFieldID(this_class, global::is_badlion ? "W" : "field_70173_aa", "I");
		env->DeleteLocalRef(this_class);

		return env->GetIntField(player, fid);
	}
	void set_noclip(bool state)
	{
		auto this_class = env->GetObjectClass(player);
		jfieldID hurttime_fid = env->GetFieldID(this_class, global::is_badlion ? "T" : "field_70145_X", "Z");
		env->DeleteLocalRef(this_class);

		env->SetBooleanField(player, hurttime_fid, state);

	}

	void set_moveforward(float val)
	{
		auto this_class = env->GetObjectClass(player);
		jfieldID fid;

		if (global::is_lunar) {
			static auto field = mcc::impl::EntityLivingBase::fields[41];
			fid = env->GetFieldID(this_class, field.name, "F");
		}
		else {
			fid = env->GetFieldID(this_class, global::is_badlion ? "ba" : "field_70701_bs", "F");
		}
		
		env->DeleteLocalRef(this_class);

		env->SetFloatField(player, fid, val);
	}

	float get_movestrafe()
	{
		auto this_class = env->GetObjectClass(player);
		jfieldID fid;

		if (global::is_lunar) {
			static auto field = mcc::impl::EntityLivingBase::fields[40];
			fid = env->GetFieldID(this_class, field.name, "F");
		}
		else
			fid = env->GetFieldID(this_class, global::is_badlion ? "aZ" : "field_70702_br", "F");

		env->DeleteLocalRef(this_class);

		return env->GetFloatField(player, fid);
	}
	void set_movestrafe(float val)
	{
		auto this_class = env->GetObjectClass(player);
		jfieldID fid;

		if (global::is_lunar) {
			static auto field = mcc::impl::EntityLivingBase::fields[40];
			fid = env->GetFieldID(this_class, field.name, "F");
		}
		else {
			fid = env->GetFieldID(this_class, global::is_badlion ? "aZ" : "field_70702_br", "F");
		}

		env->DeleteLocalRef(this_class);

		return env->SetFloatField(player, fid, val);
	}

	std::shared_ptr<c_nethandlerplayclient> get_sendQueue()
	{

		static jfieldID fid;

		if (static bool once = false; !once) {
			if (global::is_lunar) {
				static auto field = mcc::impl::EntityPlayerSP::fields[0];
				fid = env->GetFieldID(mcc::EntityPlayerSP, field.name, field.sig);
			}
			else {
				static auto this_class = env->GetObjectClass(player);
				fid = env->GetFieldID(this_class, global::is_badlion ? "a" : "field_71174_a", global::is_badlion ? "Lbcy;" : "Lnet/minecraft/client/network/NetHandlerPlayClient;");
				//	env->DeleteLocalRef(this_class);
			}
			once = true;
		}

		return std::make_shared<c_nethandlerplayclient>(env->GetObjectField(player, fid));
	}

	void onStoppedUsingItem()
	{
		auto a = env->GetObjectClass(mc);
		jfieldID fid = env->GetFieldID(a, global::is_badlion ? "c" : "field_71442_b", global::is_badlion ? "Lbda;" : "Lnet/minecraft/client/multiplayer/PlayerControllerMP;");

		env->DeleteLocalRef(a);

		jobject playerconntroller = global::is_lunar ? get_player_controller_lunar() : env->GetObjectField(mc, fid);
		auto c = env->GetObjectClass(playerconntroller);

		auto mid = env->GetMethodID(c, global::is_badlion ? "c" : "func_78766_c", global::is_badlion ? "(Lwn;)V" : "(Lnet/minecraft/entity/player/EntityPlayer;)V");
		env->DeleteLocalRef(c);

		env->CallVoidMethod(playerconntroller, mid, this->get_object());

		env->DeleteLocalRef(playerconntroller);
	}

	float get_falldistance()
	{

		static jfieldID fid = 0;

		if (static bool once = false; !once) {
			if (global::is_lunar) {
				static auto field = mcc::impl::Entity::fields[23];
				fid = env->GetFieldID(mcc::Entity, field.name, "F");
			}
			else {
				static auto this_class = env->GetObjectClass(player);
				fid = env->GetFieldID(this_class, global::is_badlion ? "O" : "field_70143_R", "F");
				//		env->DeleteLocalRef(this_class);
			}
			once = true;
		}

		return env->GetFloatField(player, fid);
	}

	void set_falldistance(float val)
	{
		auto this_class = env->GetObjectClass(player);
		jfieldID fid = 0;

		if (global::is_lunar) {
			static auto field = mcc::impl::Entity::fields[23];
			fid = env->GetFieldID(this_class, field.name, "F");
		}
		else
			fid = env->GetFieldID(this_class, global::is_badlion ? "O" : "field_70143_R", "F");

		env->DeleteLocalRef(this_class);

		env->SetFloatField(player, fid, val);
	}

	bool getJumpTicks()
	{
		auto this_class = env->GetObjectClass(player);
		auto mid = env->GetFieldID(this_class, global::is_badlion ? "bn" : "field_70773_bE", "I");
		env->DeleteLocalRef(this_class);

		return env->GetIntField(player, mid);
	}

	bool isBlocking()
	{
		auto this_class = env->GetObjectClass(player);
		auto mid = env->GetMethodID(this_class, global::is_badlion ? "bW" : "func_70632_aY", "()Z");
		env->DeleteLocalRef(this_class);

		return env->CallBooleanMethod(player, mid);
	}

	float getprevPosY()
	{
		auto this_class = env->GetObjectClass(player);
		jfieldID mid = env->GetFieldID(this_class, global::is_badlion ? "q" : "field_70167_r", "D");
		env->DeleteLocalRef(this_class);

		return env->GetDoubleField(player, mid);
	}

	float get_lastTickPosY()
	{
		auto this_class = env->GetObjectClass(player);
		jfieldID mid = env->GetFieldID(this_class, global::is_badlion ? "Q" : "field_70137_T", "D");
		env->DeleteLocalRef(this_class);

		return env->GetDoubleField(player, mid);
	}

	vec3 get_last_tick_pos()
	{
		static jfieldID pos_x_fid;
		static jfieldID pos_y_fid;
		static jfieldID pos_z_fid;


		if(global::is_lunar)
		{

			if (static bool once = false; !once) {
				static auto field_y = mcc::impl::Entity::fields[39];
				static auto field_x = mcc::impl::Entity::fields[38];
				static auto field_z = mcc::impl::Entity::fields[40];

				pos_x_fid = env->GetFieldID(mcc::Entity, field_x.name, "D");
				pos_y_fid = env->GetFieldID(mcc::Entity, field_y.name, "D");
				pos_z_fid = env->GetFieldID(mcc::Entity, field_z.name, "D");
				once = true;
			}


			return vec3({
					(float)env->GetDoubleField(player, pos_x_fid),
					(float)env->GetDoubleField(player, pos_y_fid),
					(float)env->GetDoubleField(player, pos_z_fid)
			});
		}
		else
		{
			auto this_class = env->GetObjectClass(player);

			if (static bool once = false; !once) {
				pos_x_fid = env->GetFieldID(this_class, global::is_badlion ? "P" : "field_70142_S", "D");
				pos_y_fid = env->GetFieldID(this_class, global::is_badlion ? "q" : "field_70137_T", "D");
				pos_z_fid = env->GetFieldID(this_class, global::is_badlion ? "r" : "field_70136_U", "D");
			}

			env->DeleteLocalRef(this_class);

			return vec3({
					(float)env->GetDoubleField(player, pos_x_fid),
					(float)env->GetDoubleField(player, pos_y_fid),
					(float)env->GetDoubleField(player, pos_z_fid)
				});
		}
	}
	
	float get_lastTickPosX()
	{
		auto this_class = env->GetObjectClass(player);
		jfieldID mid = env->GetFieldID(this_class, global::is_badlion ? "P" : "field_70142_S", "D");
		env->DeleteLocalRef(this_class);

		return env->GetDoubleField(player, mid);
	}
	float get_lastTickPosZ()
	{
		auto this_class = env->GetObjectClass(player);
		jfieldID mid = env->GetFieldID(this_class, global::is_badlion ? "R" : "field_70136_U", "D");
		env->DeleteLocalRef(this_class);

		return env->GetDoubleField(player, mid);
	}
	void set_pos(vec3 pos, bool fl)
	{
		auto this_class = env->GetObjectClass(player);
		jmethodID mid = 0;

		if (global::is_lunar)
			mid = env->GetMethodID(this_class, "sseeashhhsseeahehesssahph", "(DDD)V");
		else
			mid = env->GetMethodID(this_class, global::is_badlion ? "b" : "func_70107_b", "(DDD)V");

		if (!mid)
			return;
		
		if (fl)
		{
			double y = (double)(pos.y + 0.00001);
			env->CallVoidMethod(player, mid, pos.x, y, pos.z);
		}
		else
		{
			env->CallVoidMethod(player, mid, (double)pos.x, (double)pos.y, (double)pos.z);
		}

		env->DeleteLocalRef(this_class);


	}

	float getprevRenderYawOffset()
	{
		if(global::is_lunar)
		{
			auto this_class = mcc::EntityLivingBase;
			static auto field = mcc::impl::EntityLivingBase::fields[24];
			jfieldID mid = env->GetFieldID(this_class, field.name, "F");

			return env->GetFloatField(player, mid);
		}
		else {
			auto this_class = env->GetObjectClass(player);
			jfieldID mid = env->GetFieldID(this_class, global::is_badlion ? "aJ" : "field_70760_ar", "F");
			env->DeleteLocalRef(this_class);

			return env->GetFloatField(player, mid);
		}
	}

	float getrenderYawOffset()
	{
		if (global::is_lunar)
		{
			auto this_class = mcc::EntityLivingBase;
			static auto field = mcc::impl::EntityLivingBase::fields[23];
			jfieldID mid = env->GetFieldID(this_class, field.name, "F");

			return env->GetFloatField(player, mid);
		}
		else {
			auto this_class = env->GetObjectClass(player);
			jfieldID mid = env->GetFieldID(this_class, global::is_badlion ? "aL" : "field_70758_at", "F");
			env->DeleteLocalRef(this_class);

			return env->GetFloatField(player, mid);
		}
	}
	
	bool is_sneaking()
	{

		static jmethodID mid = nullptr;

		if (static bool once = false; !once) {
			if (global::is_lunar)
				mid = env->GetMethodID(mcc::Entity, "bridge$isSneaking", "()Z");
			else {
				static auto this_class = env->GetObjectClass(player);
				mid = env->GetMethodID(this_class, global::is_badlion ? "av" : "func_70093_af", "()Z");
			}
			once = true;
		}

	//	env->DeleteLocalRef(this_class);

		return env->CallBooleanMethod(player, mid);
	}

	void set_on_ground(bool state)
	{

		static jfieldID fid = nullptr;

		if (static bool once = false; !once) {
			if (global::is_lunar) {
				static auto field = mcc::impl::Entity::fields[23];
				fid = env->GetFieldID(mcc::Entity, field.name, "Z");
			}
			else {
				static auto a = env->GetObjectClass(player);
				fid = env->GetFieldID(a, global::is_badlion ? "C" : "field_70122_E", "Z");
				//		env->DeleteLocalRef(a);
			}
			once = true;
		}


		env->SetBooleanField(player, fid, state);
	}
	bool on_ground()
	{

		static jfieldID fid = nullptr;

		if (static bool once = false; !once) {
			if (global::is_lunar) {
				static auto field = mcc::impl::Entity::fields[23];
				fid = env->GetFieldID(mcc::Entity, field.name, "Z");
			}
			else {
				static auto a = env->GetObjectClass(player);
				fid = env->GetFieldID(a, global::is_badlion ? "C" : "field_70122_E", "Z");
				//		env->DeleteLocalRef(a);
			}
			once = true;
		}

		return env->GetBooleanField(player, fid);
	}

	bool is_on_same_team(jobject other)
	{
		auto this_class = env->GetObjectClass(player);
		jmethodID mid;

		if (global::is_lunar)
			mid = env->GetMethodID(this_class, "asppahpppshahssahaahspesp", "(Lnet/minecraft/v1_8/haseapheheahpesepaphhhaaa;)Z");
		else
			mid = env->GetMethodID(this_class, global::is_badlion ? "c" : "func_142014_c", global::is_badlion ? "(Lpr;)Z" : "(Lnet/minecraft/entity/EntityLivingBase;)Z");
		env->DeleteLocalRef(this_class);

		return env->CallBooleanMethod(player, mid, other);
	}

	bool is_breaking_block()
	{
		auto a = global::is_lunar ? mcc::ave : env->GetObjectClass(mc);
		jfieldID fid = env->GetFieldID(a, global::is_badlion ? "c" : "field_71442_b", global::is_badlion ? "Lbda;" : "Lnet/minecraft/client/multiplayer/PlayerControllerMP;");

		if(!global::is_lunar)
			env->DeleteLocalRef(a);

		jobject playerconntroller = global::is_lunar ? get_player_controller_lunar() : env->GetObjectField(mc, fid);
		auto c = global::is_lunar ? mcc::PlayerController : env->GetObjectClass(playerconntroller);

		jfieldID hitting;

		if (global::is_lunar) {
			static auto field = mcc::impl::PlayerController::fields[7];
			hitting = env->GetFieldID(c, field.name, "Z");
		}
		else {
			hitting = env->GetFieldID(c, global::is_badlion ? "h" : "field_78778_j", "Z");
		}

		if (!global::is_lunar)
			env->DeleteLocalRef(c);

		bool b = env->GetBooleanField(playerconntroller, hitting);

		
		env->DeleteLocalRef(playerconntroller);

		return b;
	}


	// public boolean hepspahpahpaphsapaeaahssp(ppeapapeapshppaspspspaheh , psheppphaheaheesspasahsaa psheppphaheaheesspasahsaa1, eahsaehhhhseeessppepahsas eahsaehhhhseeessppepahsas1)
	bool sendUseItem(jobject player, jobject world, jobject item_stack)
	{
		auto mc_class = env->GetObjectClass(mc);
		jfieldID fid = env->GetFieldID(mc_class, global::is_badlion ? "c" : "field_71442_b", global::is_badlion ? "Lbda;" : "Lnet/minecraft/client/multiplayer/PlayerControllerMP;");

		jobject playerconntroller = global::is_lunar ? get_player_controller_lunar() : env->GetObjectField(mc, fid);

		auto pc_class = env->GetObjectClass(playerconntroller);
		jmethodID mid;

		//aasheseshphhppshseeeapshp:(Lnet/minecraft/v1_8/hahashpseheesapehaepaesea;Lnet/minecraft/v1_8/seeeheasshepsehssapshhshe;Lnet/minecraft/v1_8/saspaesphsapppsapeesahsep;)Z
		if (global::is_lunar)
			mid = env->GetMethodID(pc_class, "seapaeepppshspasapaepheep", "(Lnet/minecraft/v1_8/paasspaeaehapahshaahespep;Lnet/minecraft/v1_8/epppapsessepheapphseapphe;Lnet/minecraft/v1_8/heppaeesahppapseeapaeseep;)Z");
		else
			mid = env->GetMethodID(pc_class, global::is_badlion ? "a" : "func_78769_a", global::is_badlion ? "(Lwn;Ladm;Lzx;)Z" : "(Lnet/minecraft/entity/player/EntityPlayer;Lnet/minecraft/world/World;Lnet/minecraft/item/ItemStack;)Z");

		bool b = env->CallBooleanMethod(playerconntroller, mid, player, world, item_stack);
		
		env->DeleteLocalRef(playerconntroller);
		env->DeleteLocalRef(mc_class);
		env->DeleteLocalRef(pc_class);

		return b;
	}



	void set_block_hit_delay(int val)
	{
		auto mc_class = env->GetObjectClass(mc);
		jfieldID fid = env->GetFieldID(mc_class, global::is_badlion ? "c" : "field_71442_b", global::is_badlion ? "Lbda;" : "Lnet/minecraft/client/multiplayer/PlayerControllerMP;");

		jobject playerconntroller = global::is_lunar ? get_player_controller_lunar() : env->GetObjectField(mc, fid);
		auto pc_class = env->GetObjectClass(playerconntroller);
		jfieldID hitting;

		if (global::is_lunar) {
			static auto field = mcc::impl::PlayerController::fields[6];
			hitting = env->GetFieldID(pc_class, field.name, field.sig);
		}
		else
			hitting = env->GetFieldID(pc_class, global::is_badlion ? "g" : "field_78781_i", "I");

		env->SetIntField(playerconntroller, hitting, val);
		env->DeleteLocalRef(playerconntroller);
		env->DeleteLocalRef(pc_class);
		env->DeleteLocalRef(mc_class);
	}

	float get_current_block_dmg()
	{
		auto mc_class = env->GetObjectClass(mc);
		jfieldID fid = env->GetFieldID(mc_class, global::is_badlion ? "c" : "field_71442_b", global::is_badlion ? "Lbda;" : "Lnet/minecraft/client/multiplayer/PlayerControllerMP;");

		jobject playerconntroller = global::is_lunar ? get_player_controller_lunar() : env->GetObjectField(mc, fid);
		auto pc_class = env->GetObjectClass(playerconntroller);
		jfieldID hitting;

		if (global::is_lunar) {
			static auto field = mcc::impl::PlayerController::fields[4];
			hitting = env->GetFieldID(pc_class, field.name, "F");
		}
		else
			hitting = env->GetFieldID(pc_class, global::is_badlion ? "e" : "field_78770_f", "F");

		float b = env->GetFloatField(playerconntroller, hitting);
		env->DeleteLocalRef(playerconntroller);
		env->DeleteLocalRef(mc_class);
		env->DeleteLocalRef(pc_class);

		return b;
	}
	void set_current_block_dmg(const float dmg)
	{

		auto mc_class = env->GetObjectClass(mc);

		jfieldID fid = env->GetFieldID(mc_class, global::is_badlion ? "c" : "field_71442_b", global::is_badlion ? "Lbda;" : "Lnet/minecraft/client/multiplayer/PlayerControllerMP;");

		jobject playerconntroller = global::is_lunar ? get_player_controller_lunar() : env->GetObjectField(mc, fid);
		auto pc_class = env->GetObjectClass(playerconntroller);

		jfieldID hitting;

		if (global::is_lunar) {
			static auto field = mcc::impl::PlayerController::fields[4];
			hitting = env->GetFieldID(pc_class, field.name, "F");
		}
		else
			hitting = env->GetFieldID(pc_class, global::is_badlion ? "e" : "field_78770_f", "F");

		env->SetFloatField(playerconntroller, hitting, dmg);
		env->DeleteLocalRef(playerconntroller);
		env->DeleteLocalRef(mc_class);
		env->DeleteLocalRef(pc_class);
	}

	std::string get_name()
	{	
		static jmethodID mid = nullptr;
		if (static bool once = false; !once) {
			if (global::is_lunar) {
				mid = env->GetMethodID(mcc::EntityPlayer, "bridge$getName", "()Ljava/lang/String;");
			}
			else {
				static auto this_class = global::is_lunar ? mcc::EntityPlayer : env->GetObjectClass(player);
				mid = env->GetMethodID(this_class, global::is_badlion ? "e_" : "func_70005_c_", "()Ljava/lang/String;");
			}
			once = true;
		}

		if (mid == nullptr)
			return "";
		
		jstring jstr = (jstring)(env->CallObjectMethod(player, mid));
		if (jstr == nullptr)
			return "";
		
		const char* strReturn = env->GetStringUTFChars(jstr, nullptr);
		env->ReleaseStringUTFChars(jstr, strReturn);
		env->DeleteLocalRef(jstr);
		//if(!global::is_lunar)
		//	env->DeleteLocalRef(this_class);

		return strReturn;
	}

	std::string get_custom_nametag()
	{
		auto this_class = env->GetObjectClass(player);
		jmethodID mid = env->GetMethodID(this_class, global::is_badlion ? "aM" : "func_95999_t", "()Ljava/lang/String;");
		jstring jstr = (jstring)(env->CallObjectMethod(player, mid));
		const char* strReturn = env->GetStringUTFChars(jstr, 0);
		env->ReleaseStringUTFChars(jstr, strReturn);
		env->DeleteLocalRef(jstr);
		env->DeleteLocalRef(this_class);

		return strReturn;
	}
	std::shared_ptr<IChatComponent> getDisplayName()
	{
		jclass uwu = env->GetObjectClass(player);
		jmethodID mid = env->GetMethodID(uwu, global::is_badlion ? "f_" : "func_145748_c_", global::is_badlion ? "()Leu;" : "()Lnet/minecraft/util/IChatComponent;");

		//			mid = env->GetMethodID(this_class, "bridge$getDisplayName", "()Ljava/lang/String;");
		env->DeleteLocalRef(uwu);
		return std::make_shared<IChatComponent>(env->CallObjectMethod(player, mid));
	}

	void clickBlock(jobject blockpos, jobject da_enum)
	{
		auto a = global::is_lunar ? mcc::ave : env->GetObjectClass(mc);

		jfieldID fid = nullptr;

		if (!global::is_lunar) {
			fid = env->GetFieldID(a, global::is_badlion ? "c" : "field_71442_b", global::is_badlion ? "Lbda;" : "Lnet/minecraft/client/multiplayer/PlayerControllerMP;");
		}

		jobject playerconntroller;
		if (global::is_lunar)
			playerconntroller = get_player_controller_lunar();
		else
			playerconntroller = env->GetObjectField(mc, fid);

		if (playerconntroller == nullptr)
			return;

		if(!global::is_lunar)
			env->DeleteLocalRef(a);

		auto b = env->GetObjectClass(playerconntroller);

		static jmethodID mid = nullptr;

		if (static bool init = false; !init) {
			if (global::is_lunar) {

				mid = env->GetMethodID(b, "hhphhhheseppapshpeppaeepp", "(Lnet/minecraft/v1_8/ppsesapeeashaaaehpeaahass;Lnet/minecraft/v1_8/sapespahhseshheshpesheshh;)V");
			}
			else {
				mid = env->GetMethodID(b, global::is_badlion ? "b" : "func_180511_b", global::is_badlion ? "(Lcj;Lcq;)Z" : "(Lnet/minecraft/util/BlockPos;Lnet/minecraft/util/EnumFacing;)Z");
			}
			init = true;
		}

		//printf("args: %p, %p, %p, %p\n", playerconntroller, mid, blockpos, da_enum);

		if(mid)
			env->CallBooleanMethod(playerconntroller, mid, blockpos, da_enum);


		env->DeleteLocalRef(playerconntroller);
	}

	vec3 get_position()
	{
		if (global::is_lunar)
		{
			static jfieldID pos_x_fid;
			static jfieldID pos_y_fid;
			static jfieldID pos_z_fid;

			static auto field_x = mcc::impl::Entity::fields[12];
			static auto field_y = mcc::impl::Entity::fields[13];
			static auto field_z = mcc::impl::Entity::fields[14];

			if (static bool once = false; !once) {
				pos_x_fid = env->GetFieldID(mcc::Entity, field_x.name, "D");
				pos_y_fid = env->GetFieldID(mcc::Entity, field_y.name, "D");
				pos_z_fid = env->GetFieldID(mcc::Entity, field_z.name, "D");
				once = true;
			}

			return vec3{
				static_cast<float>(env->GetDoubleField(player, pos_x_fid)),
				static_cast<float>(env->GetDoubleField(player, pos_y_fid)),
				static_cast<float>(env->GetDoubleField(player, pos_z_fid))
			};
		}
		else {
			static auto this_clazz = env->GetObjectClass(player);

			static jfieldID pos_x_fid;
			static jfieldID pos_y_fid;
			static jfieldID pos_z_fid;

			if (static bool once = false; !once) {
				pos_x_fid = env->GetFieldID(this_clazz, global::is_badlion ? "s" : "field_70165_t", "D");
				pos_y_fid = env->GetFieldID(this_clazz, global::is_badlion ? "t" : "field_70163_u", "D");
				pos_z_fid = env->GetFieldID(this_clazz, global::is_badlion ? "u" : "field_70161_v", "D");
				once = true;
			}

			//env->DeleteLocalRef(this_clazz);

			return vec3{
				static_cast<float>(env->GetDoubleField(player, pos_x_fid)),
				static_cast<float>(env->GetDoubleField(player, pos_y_fid)),
				static_cast<float>(env->GetDoubleField(player, pos_z_fid))
			};
		}
	}

	float get_distance_to(std::shared_ptr<c_player> other)
	{
		auto pos = get_position();
		auto entity_pos = other->get_position();
		return util::distance(pos.x, pos.y, pos.z, entity_pos.x, entity_pos.y, entity_pos.z);
	}

	jobject get_object()
	{
		return player;
	}

	float get_yaw()
	{

		if (global::is_lunar)
		{
			static auto field = mcc::impl::Entity::fields[18];
			static jfieldID fid = env->GetFieldID(mcc::Entity, field.name, "F");

			float d = env->GetFloatField(player, fid);
		//	env->DeleteLocalRef(this_clazz);

			return d;
		}
		else {
			static auto this_clazz = env->GetObjectClass(player);
			static jfieldID fid = env->GetFieldID(this_clazz, global::is_badlion ? "y" : "field_70177_z", "F");

			float d = env->GetFloatField(player, fid);
			//env->DeleteLocalRef(this_clazz);

			return d;
		}
	}

	float get_yaw2()
	{
		if (global::is_lunar)
		{
			static auto field = mcc::impl::EntityLivingBase::fields[25];
			static jfieldID fid = env->GetFieldID(mcc::EntityLivingBase, field.name, "F");

			float d = env->GetFloatField(player, fid);

			return d;
		}
		else {
			auto this_clazz = env->GetObjectClass(player);
			jfieldID fid = env->GetFieldID(this_clazz, global::is_badlion ? "aK" : "field_70759_as", "F");

			float d = env->GetFloatField(player, fid);
			env->DeleteLocalRef(this_clazz);

			return d;
		}
	}

	void set_yaw(float yaw)
	{

		if (global::is_lunar)
		{
			static auto field = mcc::impl::Entity::fields[18];
			static jfieldID fid = env->GetFieldID(mcc::Entity, field.name, "F");

			env->SetFloatField(player, fid, yaw);
		}
		else {
			static auto this_clazz = env->GetObjectClass(player);
			jfieldID fid = env->GetFieldID(this_clazz, global::is_badlion ? "y" : "field_70177_z", "F");

			env->SetFloatField(player, fid, yaw);
		//	env->DeleteLocalRef(this_clazz);
		}

	}

	//void set_new_yaw(float yaw)
	//{
	//	auto this_clazz = env->GetObjectClass(player);
	//	jfieldID fid = env->GetFieldID(this_clazz, global::is_badlion ? "bg" : "field_70712_bm", "F");

	//	env->SetFloatField(player, fid, yaw);
	//	env->DeleteLocalRef(this_clazz);
	//}

	std::shared_ptr<c_axisalignedbb> get_bounding_box()
	{

		static jfieldID boundingbox_fid;

		if (static bool once = false; !once) {
			if (global::is_lunar) {
				static auto field = mcc::impl::Entity::fields[22];
				boundingbox_fid = env->GetFieldID(mcc::Entity, field.name, field.sig);
			}
			else {
				auto player_class = env->GetObjectClass(player);
				boundingbox_fid = env->GetFieldID(player_class, global::is_badlion ? "f" : "field_70121_D", global::is_badlion ? "Laug;" : "Lnet/minecraft/util/AxisAlignedBB;");
				env->DeleteLocalRef(player_class);
			}
			once = true;
		}
		return std::make_shared<c_axisalignedbb>(env->GetObjectField(player, boundingbox_fid), env);
	}

	s_axisalignedbb get_native_non_modified_boundingbox()
	{
		const auto [x, y, z] = this->get_position();

		const float f = 0.6f / 2.f;
		const float f1 = 1.8f;

		return s_axisalignedbb({ x - f, y, z - f, x + f, y + f1, z + f });
	}
	
	bool is_invisible()
	{

		static jmethodID invisible_md = nullptr;

		if (static bool once = false; !once) {
			if (global::is_lunar) {
				invisible_md = env->GetMethodID(mcc::Entity, "bridge$isInvisible", "()Z");
			}
			else {
				static auto this_clazz = env->GetObjectClass(player);
				invisible_md = env->GetMethodID(this_clazz, global::is_badlion ? "ax" : "func_82150_aj", "()Z");
			}
		once = true;
		}

		//env->DeleteLocalRef(this_clazz);

		return env->CallBooleanMethod(player, invisible_md);
	}

	int get_hurt_time()
	{
		// EntityLivingBase - bridge$getHurtTime

		static jfieldID hurttime_fid;
		if (static bool once = false; !once) {
			if (global::is_lunar) {
				static auto field = mcc::impl::EntityLivingBase::fields[9];
				hurttime_fid = env->GetFieldID(mcc::EntityLivingBase, field.name, "I");
			}
			else {
				static auto this_clazz = env->GetObjectClass(player);
				hurttime_fid = env->GetFieldID(this_clazz, global::is_badlion ? "au" : "field_70737_aN", "I");
			}
		once = true;
		}


		//env->DeleteLocalRef(this_clazz);

		return env->GetIntField(player, hurttime_fid);
	}
	int get_hurtResistantTime()
	{
		auto this_clazz = env->GetObjectClass(player);
		jfieldID hurttime_fid = env->GetFieldID(this_clazz, global::is_badlion ? "Z" : "field_70172_ad", "I");
		env->DeleteLocalRef(this_clazz);

		return env->GetIntField(player, hurttime_fid);
	}
	int get_max_hurt_time()
	{
	
		static jfieldID get_health_mid;

		if (static bool once = false; !once) {
			if (global::is_lunar) {
				static auto field = mcc::impl::EntityLivingBase::fields[10];
				get_health_mid = env->GetFieldID(mcc::EntityLivingBase, field.name, "I");
			}
			else {
				static auto this_clazz = env->GetObjectClass(player);
				get_health_mid = env->GetFieldID(this_clazz, global::is_badlion ? "av" : "field_70738_aO", "I");
			}
			once = true;
		}


		//env->DeleteLocalRef(this_clazz);

		return env->GetIntField(player, get_health_mid);
	}

	vec3 get_motion_vector()
	{

		static jfieldID mot_x_fid;
		static jfieldID mot_y_fid;
		static jfieldID mot_z_fid;
		if (static bool once = false; !once) {
			if (global::is_lunar)
			{
				static auto motion_x = mcc::impl::Entity::fields[15];
				static auto motion_y = mcc::impl::Entity::fields[16];
				static auto motion_z = mcc::impl::Entity::fields[17];

				mot_x_fid = env->GetFieldID(mcc::Entity, motion_x.name, "D");
				mot_y_fid = env->GetFieldID(mcc::Entity, motion_y.name, "D");
				mot_z_fid = env->GetFieldID(mcc::Entity, motion_z.name, "D");
			}
			else {
				static auto this_clazz = env->GetObjectClass(player);
				mot_x_fid = env->GetFieldID(this_clazz, global::is_badlion ? "v" : "field_70159_w", "D");
				mot_y_fid = env->GetFieldID(this_clazz, global::is_badlion ? "w" : "field_70181_x", "D");
				mot_z_fid = env->GetFieldID(this_clazz, global::is_badlion ? "x" : "field_70179_y", "D");
			}
			once = true;
		}

		//env->DeleteLocalRef(this_clazz);

		return vec3{
			static_cast<float>(env->GetDoubleField(player, mot_x_fid)),
			static_cast<float>(env->GetDoubleField(player, mot_y_fid)),
			static_cast<float>(env->GetDoubleField(player, mot_z_fid))
		};
	}

	void jump()
	{
		auto this_clazz = env->GetObjectClass(player);
		jmethodID mid;

		if (global::is_lunar)
			mid = env->GetMethodID(this_clazz, "aeppeshhsphsessppsspspphe", "()V");
		else
			mid = env->GetMethodID(this_clazz, global::is_badlion ? "bF" : "func_70664_aZ", "()V");

		env->DeleteLocalRef(this_clazz);
		env->CallVoidMethod(player, mid);
	}

	void set_land_mov_fac(float val)
	{
		auto this_clazz = env->GetObjectClass(player);
		jfieldID mid = 0;

		if (global::is_lunar) {
			static auto field = mcc::impl::EntityLivingBase::fields[54];
			mid = env->GetFieldID(this_clazz, field.name, "F");
		}
		else
			env->GetFieldID(this_clazz, global::is_badlion ? "bm" : "field_70746_aG", "F");

		env->DeleteLocalRef(this_clazz);

		env->SetFloatField(player, mid, val);
	}
	void set_jump_mov_fac(float val)
	{
		auto this_clazz = env->GetObjectClass(player);
		jfieldID mid = 0;

		if (global::is_lunar) {
			static auto field = mcc::impl::EntityLivingBase::fields[27];
			mid = env->GetFieldID(this_clazz, field.name, "F");
		}
		else
			mid = env->GetFieldID(this_clazz, global::is_badlion ? "bm" : "field_70747_aH", "F");

		env->DeleteLocalRef(this_clazz);
		env->SetFloatField(player, mid, val);
	}

	// @todo: set motion x, y, z optimize
	void SetmotionX(double amount)
	{
		static auto this_clazz = env->GetObjectClass(player);
		jfieldID pitch_fid;
		if (global::is_lunar) {
			static auto motion_x = mcc::impl::Entity::fields[15];
			pitch_fid = env->GetFieldID(this_clazz, motion_x.name, "D");
		}
		else
			pitch_fid = env->GetFieldID(this_clazz, global::is_badlion ? "v" : "field_70159_w", "D");

		//env->DeleteLocalRef(this_clazz);

		env->SetDoubleField(player, pitch_fid, amount);
	}

	void SetmotionY(double amount)
	{
		static auto this_clazz = env->GetObjectClass(player);
		jfieldID pitch_fid;
		if (global::is_lunar) {
			static auto motion_y = mcc::impl::Entity::fields[16];
			pitch_fid = env->GetFieldID(this_clazz, motion_y.name, "D");
		}
		else
			pitch_fid = env->GetFieldID(this_clazz, global::is_badlion ? "w" : "field_70181_x", "D");

		//env->DeleteLocalRef(this_clazz);

		env->SetDoubleField(player, pitch_fid, amount);
	}

	void SetmotionZ(double amount)
	{
		static auto this_clazz = env->GetObjectClass(player);
		jfieldID pitch_fid;
		if (global::is_lunar) {
			static auto motion_z = mcc::impl::Entity::fields[17];
			pitch_fid = env->GetFieldID(this_clazz, motion_z.name, "D");
		}
		else
			pitch_fid = env->GetFieldID(this_clazz, global::is_badlion ? "x" : "field_70179_y", "D");

		//env->DeleteLocalRef(this_clazz);

		env->SetDoubleField(player, pitch_fid, amount);
	}

	jobject get_current_equiped_item()
	{

		jmethodID mid;

		if (global::is_lunar) {
			static auto method = mapper->searchItem(mcc::impl::EntityPlayer::methods, "bridge$getCurrentEquippedItem");
			/* updated */
			mid = env->GetMethodID(mcc::EntityPlayer, method.name, method.sig);
		}
		else {
			auto this_clazz = env->GetObjectClass(player);
			mid = env->GetMethodID(this_clazz, global::is_badlion ? "bZ" : "func_71045_bC", global::is_badlion ? "()Lzx;" : "()Lnet/minecraft/item/ItemStack;");
			env->DeleteLocalRef(this_clazz);

		}


		return env->CallObjectMethod(player, mid);
	}

	jobject get_capabilities()
	{
		auto this_clazz = env->GetObjectClass(player);
		jfieldID fid;
		
		if (global::is_lunar) {

			static auto field = mcc::impl::EntityPlayer::fields[24];
			
			fid = env->GetFieldID(this_clazz, field.name, field.sig);
		}
		else
			fid = env->GetFieldID(this_clazz, global::is_badlion ? "bA" : "field_71075_bZ", global::is_badlion ? "Lwl;" : "Lnet/minecraft/entity/player/PlayerCapabilities;");
			
		env->DeleteLocalRef(this_clazz);

		return env->GetObjectField(player, fid);
	}

	void attack_entity(jobject entity)
	{

		if(global::is_lunar)
		{

			// Effect
			/*
			{
				jclass c = mcc::EntityPlayer;
				static jmethodID mid = env->GetMethodID(c, "espesahhpshhpspesseaheeha", "(Lnet/minecraft/v1_8/espphaeeepaspshsppspasehe;)V");
				env->CallVoidMethod(player, mid, entity);
			}
			*/
			
			jobject playerconntroller = get_player_controller_lunar();

			if (playerconntroller == nullptr)
				return;


			static jmethodID mid = nullptr;

			if (static bool once = false; !once) {

				mid = env->GetMethodID(mcc::PlayerController, "ahaeeeashsphahhsappeahaph", "(Lnet/minecraft/v1_8/paasspaeaehapahshaahespep;Lnet/minecraft/v1_8/espphaeeepaspshsppspasehe;)V");

				once = true;
			}

			if (mid && playerconntroller && player && entity)
			{
				env->CallVoidMethod(playerconntroller, mid, player, entity);
			}

		}
		else
		{
			auto a = env->GetObjectClass(mc);

			jfieldID fid = nullptr;

			if (!global::is_lunar) {
				fid = env->GetFieldID(a, global::is_badlion ? "c" : "field_71442_b", global::is_badlion ? "Lbda;" : "Lnet/minecraft/client/multiplayer/PlayerControllerMP;");
			}

			if (a)
				env->DeleteLocalRef(a);
			
			jobject playerconntroller = env->GetObjectField(mc, fid);

			if (playerconntroller == nullptr)
				return;

			auto b =  env->GetObjectClass(playerconntroller);

			static jmethodID mid = nullptr;

			if (static bool once = false; !once) {

				mid = env->GetMethodID(b, global::is_badlion ? "a" : "func_78764_a", global::is_badlion ? "(Lwn;Lpk;)V" : "(Lnet/minecraft/entity/player/EntityPlayer;Lnet/minecraft/entity/Entity;)V");

				once = true;
			}

			if (mid != nullptr && playerconntroller != nullptr && player != nullptr && entity != nullptr)
			{
				env->CallVoidMethod(playerconntroller, mid, player, entity);
			}

			if (b)
				env->DeleteLocalRef(b);
		}

		

		//
		//jobject playerconntroller;
		//if (global::is_lunar)
		//	playerconntroller = get_player_controller_lunar();
		//else
		//	playerconntroller = env->GetObjectField(mc, fid);

		//if (playerconntroller == nullptr)
		//	return;

		//if(!global::is_lunar)
		//	env->DeleteLocalRef(a);

		//auto b = global::is_lunar ? mcc::PlayerController :env->GetObjectClass(playerconntroller);

		//static jmethodID mid = nullptr;

		//if (static bool once = false; !once) {
		//	if (global::is_lunar) {

		//		mid = env->GetMethodID(b, "ahaeeeashsphahhsappeahaph", "(Lnet/minecraft/v1_8/paasspaeaehapahshaahespep;Lnet/minecraft/v1_8/espphaeeepaspshsppspasehe;)V");
		//	}
		//	else {
		//		mid = env->GetMethodID(b, global::is_badlion ? "a" : "func_78764_a", global::is_badlion ? "(Lwn;Lpk;)V" : "(Lnet/minecraft/entity/player/EntityPlayer;Lnet/minecraft/entity/Entity;)V");
		//	}
		//	once = true;
		//}
		//
		//if(mid && playerconntroller && player && entity)
		//{
		//	env->CallVoidMethod(playerconntroller, mid, player, entity);		
		//}

		//if(b && !global::is_lunar)
		//	env->DeleteLocalRef(b);
		//
		//if(playerconntroller)
		//	env->DeleteLocalRef(playerconntroller);
	}

	std::shared_ptr<c_playermodel> get_main_model()
	{
		auto this_clazz = env->GetObjectClass(player);
		jmethodID mid;

		if (global::is_lunar) {
			//bridge$getMainModel
			mid = env->GetMethodID(this_clazz, "aesphppashppeepsshsphpash", "()Lnet/minecraft/v1_8/paaaeapeeeepsephesppeaphp;");
		}
		else {

		//	mid = env->GetMethodID(this_clazz, global::is_badlion ? "b" : "func_177087_b", global::is_badlion ? "()Lbbr;" : "()Lnet/minecraft/client/model/ModelPlayer;");
			mid = env->GetMethodID(this_clazz, global::is_badlion ? "b" : "func_177087_b", global::is_badlion ? "()Lbbo;" : "()Lnet/minecraft/client/model/ModelBase;");
		}

		env->DeleteLocalRef(this_clazz);


		return std::make_shared<c_playermodel>(env->CallObjectMethod(player, mid), env);

	}

	void swing_item()
	{
		auto this_clazz = env->GetObjectClass(player);
		jmethodID mid;

		if (global::is_lunar) {
			
			//static auto method = mcc::impl::EntityLivingBase::methods[53];

			//if (method.sig != "()V")
			//	return;
			
			mid = env->GetMethodID(this_clazz, "ppsaphehspsppaphhppsphpee", "()V");
		}
		else
			mid = env->GetMethodID(this_clazz, global::is_badlion ? "bw" : "func_71038_i", "()V");

		env->DeleteLocalRef(this_clazz);

		env->CallObjectMethod(player, mid);
	}

	float get_eye_height()
	{
		auto this_clazz = env->GetObjectClass(player);
		jmethodID mid;

		if (global::is_lunar)
			mid = env->GetMethodID(this_clazz, "bridge$getEyeHeight", "()F");
		else
			mid = env->GetMethodID(this_clazz, global::is_badlion ? "aS" : "func_70047_e", "()F");

		env->DeleteLocalRef(this_clazz);

		return env->CallFloatMethod(player, mid);
	}

	float get_pitch()
	{
		auto this_clazz = env->GetObjectClass(player);
		jfieldID fid = nullptr;

		if (global::is_lunar) {
			static auto field = mcc::impl::Entity::fields[19];
			fid = env->GetFieldID(this_clazz, field.name, "F");
		}
		else
			fid = env->GetFieldID(this_clazz, global::is_badlion ? "z" : "field_70125_A", "F");

		env->DeleteLocalRef(this_clazz);

		return env->GetFloatField(player, fid);
	}

	void set_pitch(float pitch)
	{
		auto this_clazz = env->GetObjectClass(player);

		jfieldID fid = nullptr;

		if (global::is_lunar) {
			static auto field = mcc::impl::Entity::fields[19];
			fid = env->GetFieldID(this_clazz, field.name, "F");
		}
		else
			fid = env->GetFieldID(this_clazz, global::is_badlion ? "z" : "field_70125_A", "F");

		env->DeleteLocalRef(this_clazz);

		env->SetFloatField(player, fid, pitch);
	}

	bool is_visible(jobject other)
	{
		auto this_clazz = env->GetObjectClass(player);
		jmethodID mid = nullptr;

		// EntityLivingBase - on bottom (Entity entityIn)
		if (global::is_lunar)
			mid = env->GetMethodID(this_clazz, "appaeapesspppppesppahahsh", "(Lnet/minecraft/v1_8/espphaeeepaspshsppspasehe;)Z");
		else
			mid = env->GetMethodID(this_clazz, global::is_badlion ? "t" : "func_70685_l", global::is_badlion ? "(Lpk;)Z" : "(Lnet/minecraft/entity/Entity;)Z");

		env->DeleteLocalRef(this_clazz);

		return env->CallBooleanMethod(player, mid, other);
	}

	float getDirection()
	{
		float player_yaw = this->get_yaw();

		if (this->get_moveforward() < 0.f)
		{
			player_yaw += 180.f;
		}

		float forward = 1.f;

		if (this->get_moveforward() < 0.f)
		{
			forward = -0.5f;
		}
		else if (this->get_moveforward() > 0.f)
		{
			forward = 0.5f;
		}

		if (this->get_movestrafe() > 0.f)
		{
			player_yaw -= 90.f * forward;
		}
		else if (this->get_movestrafe() < 0.f)
		{
			player_yaw += 90.f * forward;
		}
		float the_yaw = (player_yaw * M_PI) / 180;

		return the_yaw;
	}

	//jobject get_movementInput()
	//{
	//	auto this_clazz = env->GetObjectClass(player);
	//	jfieldID fid = env->GetFieldID(this_clazz, global::is_badlion ? "b" : "field_71158_b", global::is_badlion ? "Lbeu;" : "Lnet/minecraft/util/MovementInput;");
	//	env->DeleteLocalRef(this_clazz);

	//	return env->GetObjectField(player, fid);
	//}

	vec3 get_last_tick_position()
	{
;
		static jfieldID pos_x_fid;
		static jfieldID pos_y_fid;
		static jfieldID pos_z_fid;


		if (static bool once = false; !once) {

			static auto this_clazz = env->GetObjectClass(player);
			pos_x_fid = env->GetFieldID(this_clazz, global::is_badlion ? "P" : "field_70142_S", "D");
			pos_y_fid = env->GetFieldID(this_clazz, global::is_badlion ? "Q" : "field_70137_T", "D");
			pos_z_fid = env->GetFieldID(this_clazz, global::is_badlion ? "R" : "field_70136_U", "D");
		//	env->DeleteLocalRef(this_clazz);
			once = true;
		}


		return vec3{
			(float)env->GetDoubleField(player, pos_x_fid),
			(float)env->GetDoubleField(player, pos_y_fid),
			(float)env->GetDoubleField(player, pos_z_fid)
		};
	}


	bool is_sprinting()
	{
		auto this_clazz = env->GetObjectClass(player);
		jmethodID mid = nullptr;

		if (global::is_lunar)
			mid = env->GetMethodID(this_clazz, "aehaehpeshaespaphepaepphp", "()Z");
		else
			mid = env->GetMethodID(this_clazz, global::is_badlion ? "aw" : "func_70051_ag", "()Z");
		env->DeleteLocalRef(this_clazz);

		return env->CallBooleanMethod(player, mid);
	}

	void set_sprinting(bool state)
	{
		auto this_clazz = env->GetObjectClass(player);
		jmethodID mid = nullptr;

		if (global::is_lunar)
			mid = env->GetMethodID(this_clazz, "psehspaaasshhsseesphsaepa", "(Z)V");
		else
			mid = env->GetMethodID(this_clazz, global::is_badlion ? "d" : "func_70031_b", "(Z)V");

		env->DeleteLocalRef(this_clazz);

		env->CallBooleanMethod(player, mid, state);
	}

	//bridge$isOnLadder
	bool is_inwater()
	{
		if (global::is_lunar)
		{
			static auto method = mapper->searchItem(mcc::impl::EntityLivingBase::methods, "bridge$isInWater");
			auto this_clazz = env->GetObjectClass(player);
			auto mid = env->GetMethodID(this_clazz, method.name, method.sig);
			env->DeleteLocalRef(this_clazz);
			
			return env->CallBooleanMethod(player, mid);
		}
		else
		{
			auto this_clazz = env->GetObjectClass(player);
			jfieldID fid = env->GetFieldID(this_clazz, global::is_badlion ? "Y" : "field_70171_ac", "Z");
			env->DeleteLocalRef(this_clazz);
			return env->GetBooleanField(player, fid);
		}
	}

	bool is_collidedvertically()
	{
		auto this_clazz = env->GetObjectClass(player);
		jfieldID fid = nullptr;

		if (global::is_lunar) {
			
			static auto field = mcc::impl::Entity::fields[25];
			fid = env->GetFieldID(this_clazz, field.name, "Z");
		}
		else
			fid = env->GetFieldID(this_clazz, global::is_badlion ? "E" : "field_70124_G", "Z");

		env->DeleteLocalRef(this_clazz);
		return env->GetBooleanField(player, fid);
	}
	bool is_collidedhorizontally()
	{
		auto this_clazz = env->GetObjectClass(player);
		jfieldID fid = nullptr;

		if (global::is_lunar) {
			static auto field = mcc::impl::Entity::fields[24];
			fid = env->GetFieldID(this_clazz, field.name, "Z");
		}
		else
			fid = env->GetFieldID(this_clazz, global::is_badlion ? "D" : "field_70123_F", "Z");

		env->DeleteLocalRef(this_clazz);
		return env->GetBooleanField(player, fid);
	}

	float get_max_health()
	{

		static jmethodID mid = nullptr;

		if (static bool once = false; !once) {
			if (global::is_lunar) {

				// auuto map this shit -> entityliving base -> find "return (float)this."
				mid = env->GetMethodID(mcc::EntityLivingBase, "esaahheehsepeaehspaahpssp", "()F");
			}

			else {
				auto this_clazz = env->GetObjectClass(player);
				mid = env->GetMethodID(this_clazz, global::is_badlion ? "bn" : "func_110143_aJ", "()F");
				env->DeleteLocalRef(this_clazz);
			}
			once = true;
		}

		if (player && mid) {
			return env->CallFloatMethod(player, mid);
		}
		else
		{

			if (static bool once = false; !once)
			{
				printf("get_max_health not found!\n");
				once = true;
			}

			return 20.f;
		}
	
	}
	float get_health()
	{

		static jmethodID mid = nullptr;

		if (static bool once = false; !once) {
			if (global::is_lunar) {

				mid = env->GetMethodID(mcc::EntityLivingBase, "bridge$getHealth", "()F");
			}
			else {
				auto this_clazz = env->GetObjectClass(player);
				mid = env->GetMethodID(this_clazz, global::is_badlion ? "bn" : "func_110143_aJ", "()F");
				env->DeleteLocalRef(this_clazz);
			}
			once = true;
		}

		if(mid)
			return env->CallFloatMethod(player, mid);

		return 0.f;
	}

	//void set_pre_yaw(float yaw)
	//{
	//	auto this_class = env->GetObjectClass(player);
	//	jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "A" : "field_70126_B", "F");
	//	env->DeleteLocalRef(this_class);

	//	env->SetFloatField(player, fid, yaw);
	//}

	jobject get_currentBlock()
	{
		auto mc_class = env->GetObjectClass(mc);
		
		jfieldID fid = env->GetFieldID(mc_class, global::is_badlion ? "c" : "field_71442_b", global::is_badlion ? "Lbda;" : "Lnet/minecraft/client/multiplayer/PlayerControllerMP;");

		jobject playerconntroller = global::is_lunar ? get_player_controller_lunar() : env->GetObjectField(mc, fid);
		auto bb = env->GetObjectClass(playerconntroller);
		jfieldID hitting;

		if (global::is_lunar)
			hitting = env->GetFieldID(bb, "hspshaaaappeppaphsasppeph", "Lnet/minecraft/v1_8/ehapesphhehpehpsasppaahaa;");
		else
			hitting = env->GetFieldID(bb, global::is_badlion ? "c" : "field_178895_c", global::is_badlion ? "Lcj;" : "Lnet/minecraft/util/BlockPos;");

		jobject b = env->GetObjectField(playerconntroller, hitting);
		env->DeleteLocalRef(playerconntroller);
		env->DeleteLocalRef(bb);
		env->DeleteLocalRef(mc_class);

		return b;
	}

	// reach methods

	/**
	 * Raytrace function
	 *
	 * @param values blockReachDistance, partialTicks
	 * @return jobject MovingObjectPosition
	*/
	jobject rayTrace(double blockReachDistance, double partialTicks)
	{
		auto this_class = env->GetObjectClass(player);

		jmethodID mid = env->GetMethodID(this_class, global::is_badlion ? "a" : "func_174822_a", global::is_badlion ? "(DF)Lauh;" : "(DF)Lnet/minecraft/util/MovingObjectPosition;");


		env->DeleteLocalRef(this_class);

		return env->CallObjectMethod(player, mid, blockReachDistance, partialTicks);
	}

	/**
	* getPositionEyes function
	*
	* @param values partialTicks
	* @return returns a vector3 class
	*/
	std::shared_ptr<c_vec3> getPositionEyes(float partialTicks)
	{
		auto this_class = env->GetObjectClass(player);

		jmethodID mid = env->GetMethodID(this_class, global::is_badlion ? "e" : "func_174824_e", global::is_badlion ? "(F)Laui;" : "(F)Lnet/minecraft/util/Vec3;");

		env->DeleteLocalRef(this_class);

		return std::make_shared<c_vec3>(env->CallObjectMethod(player, mid, partialTicks));
	}

	/**
	* getLook function
	*
	* @param values partialTicks
	* @return returns a vector3 class
	*/
	std::shared_ptr<c_vec3> getLook(float partialTicks)
	{
		auto this_class = env->GetObjectClass(player);

		jmethodID mid = env->GetMethodID(this_class, global::is_badlion ? "d" : "func_70676_i", global::is_badlion ? "(F)Laui;" : "(F)Lnet/minecraft/util/Vec3;");

		env->DeleteLocalRef(this_class);

		return std::make_shared<c_vec3>(env->CallObjectMethod(player, mid, partialTicks));
	}

	float getCollisionBorderSize()
	{
		auto this_class = env->GetObjectClass(player);

		jmethodID mid = env->GetMethodID(this_class, global::is_badlion ? "ao" : "func_70111_Y", "()F");

		env->DeleteLocalRef(this_class);

		return env->CallFloatMethod(player, mid);
	}
};


#endif 
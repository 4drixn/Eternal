#pragma once
#include "Mapper.hpp"
#include "method_t.hpp"
#include "field_t.hpp"

namespace mcc {

	inline jclass GameSettings = nullptr;
	inline jclass KeyBinding = nullptr;
	
	inline jclass ave = nullptr;
	inline jclass EntityPlayerSP = nullptr;
	inline jclass PlayerController = nullptr;
	inline jclass Entity = nullptr;
	inline jclass EntityPlayer = nullptr;
	inline jclass World = nullptr;
	inline jclass WorldClient = nullptr;
	inline jclass EntityLivingBase = nullptr;
	inline jclass AxisAlignedBB = nullptr;
	inline jclass Timer = nullptr;
	inline jclass ScaledResolution = nullptr;
	inline jclass FontRenderer = nullptr;
	inline jclass BlockPos = nullptr;
	inline jclass Item = nullptr;
	inline jclass ItemStack = nullptr;
	inline jclass ItemSword = nullptr;
	inline jclass ItemAxe = nullptr;
	inline jclass ItemArmor = nullptr;
	inline jclass ItemBlock = nullptr;
	inline jclass ItemProjectile = nullptr;
	inline jclass ItemProjectile2 = nullptr;
	inline jclass ItemProjectile3 = nullptr;
	inline jclass GuiInventory = nullptr;
	inline jclass GuiChat = nullptr;
	inline jclass Block = nullptr;
	inline jclass Potion = nullptr;
	inline jclass C03PacketPlayerPosition = nullptr;
	inline jclass C04PacketPlayerPosition = nullptr;
	inline jclass C06PacketPlayerPosition = nullptr;
	inline jclass GuiClass = nullptr;
	
	inline jclass ActiveRenderInfo = nullptr;
	inline jclass RenderGlobal = nullptr;
	inline jclass RenderManager = nullptr;
	inline jclass ModelPlayer = nullptr;
	inline jclass NetHandlerPlayClient = nullptr;

	__forceinline auto Initialize() -> void
	{
		NetHandlerPlayClient = mapper->findClass("net.minecraft.v1_8.spspsahehseeseehpshheeeps");
		RenderManager = mapper->findClass("net.minecraft.v1_8.spespaespaeseseaaaasphshe");
		RenderGlobal = mapper->findClass("net.minecraft.v1_8.asspaaehpsspapsphspeshpsp"); // find thiss // textures/environment/sun.png
		ActiveRenderInfo = mapper->findClass("net.minecraft.v1_8.eaaehhessaahaaaeeashpsesp");
		ModelPlayer = mapper->findClass("net.minecraft.v1_8.paaaeapeeeepsephesppeaphp"); // public class ModelBiped extends ModelBase modelplayer extennds this shti
		
		GuiClass = mapper->findClass("net.minecraft.v1_8.peaepheeesspepsephhspheap"); 
		GameSettings = mapper->findClass("net.minecraft.v1_8.shpssesahaehhssapasehehse");
		KeyBinding =mapper->findClass(26, 10, true);
		
		PlayerController = mapper->findClass("net.minecraft.v1_8.peaasesesaesepahheahpppha");
		ave = mapper->findClass("net.minecraft.v1_8.eehhpheaeaehehehapaeshhpe");
		Entity = mapper->findClass(235, 75);
		EntityPlayerSP = mapper->findClass(86, 24, true);
		EntityPlayer = mapper->findClass("net.minecraft.v1_8.paasspaeaehapahshaahespep");
		WorldClient = mapper->findClass("net.minecraft.v1_8.shheahsepaeappahasseheaap");
		World = mapper->findClass(242, 40, true);
		EntityLivingBase = mapper->findClass(157, 57);
		AxisAlignedBB = mapper->findClass("net.minecraft.v1_8.hpspaspaasspaaapheppseaps");
		Timer =  mapper->findClass(5, 10, true);
		ScaledResolution = mapper->findClass("net.minecraft.v1_8.pssephaspapehphpappphshsa");
		FontRenderer = mapper->findClass("net.minecraft.v1_8.spppshpeshssassahaeaehsph");
		BlockPos = mapper->findClass("net.minecraft.v1_8.aesepsshepasspseahppsseep");
		
		Item = mapper->findClass(71, 12, true);
		ItemStack = mapper->findClass("net.minecraft.v1_8.heppaeesahppapseeapaeseep");
		ItemSword = mapper->findClass("net.minecraft.v1_8.peshahpaaaphepehepehaessa");
		ItemAxe = mapper->findClass("net.minecraft.v1_8.hassaehphseepphpaeasphsaa");
		ItemArmor = mapper->findClass("net.minecraft.v1_8.esppeepphhphpashpesspspha");
		ItemBlock = mapper->findClass("net.minecraft.v1_8.pphheppppphppshppspepahhe");

		ItemProjectile = mapper->findClass("net.minecraft.v1_8.pppseashaaesphssseashssep"); // Snowball
		ItemProjectile2 = mapper->findClass("net.minecraft.v1_8.hpppssahpppsseheaspeheehh"); // egg
		ItemProjectile3 = mapper->findClass("net.minecraft.v1_8.asespsphpeaapesspasahpapp"); // ender pearl

		GuiInventory = mapper->findClass("net.minecraft.v1_8.ehsapeeesspaapeppapasaehp"); // 7/9
		GuiChat = mapper->findClass("net.minecraft.v1_8.hhphespeeeeeapaeapeheasph"); // "/" in ave
		
		Block = mapper->findClass("net.minecraft.v1_8.aeahheeahpaesppshphehseep");
		Potion = mapper->findClass(29, 42, true);
		C03PacketPlayerPosition = mapper->findClass("net.minecraft.v1_8.papapeaapspephssaaheshhha");
		C04PacketPlayerPosition = mapper->findClass("net.minecraft.v1_8.papapeaapspephssaaheshhha$aaahspsehsppaphpeapsspeaa"); /// DDDZ
		C06PacketPlayerPosition = mapper->findClass("net.minecraft.v1_8.papapeaapspephssaaheshhha$pahsphpppasheaehppseaheeh"); /// most args - down
	}
}


namespace mcc {

	namespace impl {
		namespace ModelPlayer {
			inline std::vector<field_t> fields;
			__forceinline auto Initialize() -> void
			{
				if (mcc::ModelPlayer == nullptr)
				{
					std::cout << "ModelPlayer class not found!\n";
					return;
				}
				jint m_count;
				jint f_count;
				jfieldID* fields;

				jvmti->GetClassFields(mcc::ModelPlayer, &f_count, &fields);


				for (int i = 0; i < f_count; i++)
				{
					auto field = fields[i];

					char* field_name;
					char* signature;

					jvmti->GetFieldName(mcc::ModelPlayer, field, &field_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetFieldModifiers(mcc::ModelPlayer, field, &accessFlags);

					field_t _field_{};

					if (accessFlags & 0x0008)
						_field_.is_static = true;
					else
						_field_.is_static = false;

					_field_.name = field_name;
					_field_.sig = signature;

					ModelPlayer::fields.push_back(_field_);
				}

			}
		}
		
		namespace ave {
			inline std::vector<field_t> fields;
			inline std::vector<method_t> methods;

			__forceinline auto Initialize() -> void
			{
				if (mcc::ave == nullptr)
				{
					std::cout << "ave class not found!\n";
					return;
				}
				jint m_count;
				jint f_count;
				jmethodID* methods;
				jfieldID* fields;

				jvmti->GetClassMethods(mcc::ave, &m_count, &methods);
				jvmti->GetClassFields(mcc::ave, &f_count, &fields);

				for (int i = 0; i < m_count; i++)
				{
					auto method = methods[i];

					char* method_name;
					char* signature;

					jvmti->GetMethodName(method, &method_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetMethodModifiers(method, &accessFlags);

					method_t _method_{};

					if (accessFlags & 0x0008)
						_method_.is_static = true;
					else
						_method_.is_static = false;

					_method_.name = method_name;
					_method_.sig = signature;
					mcc::impl::ave::methods.push_back(_method_);
				}

				for (int i = 0; i < f_count; i++)
				{
					auto field = fields[i];

					char* field_name;
					char* signature;

					jvmti->GetFieldName(mcc::ave, field, &field_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetFieldModifiers(mcc::ave, field, &accessFlags);

					field_t _field_{};

					if (accessFlags & 0x0008)
						_field_.is_static = true;
					else
						_field_.is_static = false;

					_field_.name = field_name;
					_field_.sig = signature;

					mcc::impl::ave::fields.push_back(_field_);
				}

			}
		}

		namespace World {
			inline std::vector<field_t> fields;
			inline std::vector<method_t> methods;

			__forceinline void Initialize()
			{

				jint m_count;
				jint f_count;
				jmethodID* methods;
				jfieldID* fields;

				jvmti->GetClassMethods(mcc::World, &m_count, &methods);
				jvmti->GetClassFields(mcc::World, &f_count, &fields);

				for (int i = 0; i < m_count; i++)
				{
					auto method = methods[i];

					char* method_name;
					char* signature;

					jvmti->GetMethodName(method, &method_name, &signature, NULL);

					jint accessFlags;
					jvmti->GetMethodModifiers(method, &accessFlags);

					method_t _method_{};

					if (accessFlags & 0x0008)
						_method_.is_static = true;
					else
						_method_.is_static = false;

					_method_.name = method_name;
					_method_.sig = signature;
					mcc::impl::World::methods.push_back(_method_);
				}

				for (int i = 0; i < f_count; i++)
				{
					auto field = fields[i];

					char* field_name;
					char* signature;

					jvmti->GetFieldName(mcc::World, field, &field_name, &signature, NULL);

					jint accessFlags;
					jvmti->GetFieldModifiers(mcc::World, field, &accessFlags);

					field_t _field_{};

					if (accessFlags & 0x0008)
						_field_.is_static = true;
					else
						_field_.is_static = false;

					_field_.name = field_name;
					_field_.sig = signature;

					mcc::impl::World::fields.push_back(_field_);
				}

			}
		}
		namespace EntityPlayer {
			inline std::vector<field_t> fields;
			inline std::vector<method_t> methods;

			__forceinline void Initialize()
			{
				if (!mcc::EntityPlayer)
				{
					std::cout << "EntityPlayer not found\n";
					return;
				}
				jint m_count;
				jint f_count;
				jmethodID* methods;
				jfieldID* fields;

				jvmti->GetClassMethods(mcc::EntityPlayer, &m_count, &methods);
				jvmti->GetClassFields(mcc::EntityPlayer, &f_count, &fields);

				for (int i = 0; i < m_count; i++)
				{
					auto method = methods[i];

					char* method_name;
					char* signature;

					jvmti->GetMethodName(method, &method_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetMethodModifiers(method, &accessFlags);

					method_t _method_{};

					if (accessFlags & 0x0008)
						_method_.is_static = true;
					else
						_method_.is_static = false;

					_method_.name = method_name;
					_method_.sig = signature;
					EntityPlayer::methods.push_back(_method_);
				}

				for (int i = 0; i < f_count; i++)
				{
					auto field = fields[i];

					char* field_name;
					char* signature;

					jvmti->GetFieldName(mcc::EntityPlayer, field, &field_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetFieldModifiers(mcc::EntityPlayer, field, &accessFlags);

					field_t _field_{};

					if (accessFlags & 0x0008)
						_field_.is_static = true;
					else
						_field_.is_static = false;

					_field_.name = field_name;
					_field_.sig = signature;

					EntityPlayer::fields.push_back(_field_);
				}

			}
		}
		namespace EntityLivingBase {
			inline std::vector<field_t> fields;
			inline std::vector<method_t> methods;

			__forceinline void Initialize()
			{
				//bridge$getHurtTime
				jint m_count;
				jint f_count;
				jmethodID* methods;
				jfieldID* fields;

				jvmti->GetClassMethods(mcc::EntityLivingBase, &m_count, &methods);
				jvmti->GetClassFields(mcc::EntityLivingBase, &f_count, &fields);

				for (int i = 0; i < m_count; i++)
				{
					auto method = methods[i];

					char* method_name;
					char* signature;

					jvmti->GetMethodName(method, &method_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetMethodModifiers(method, &accessFlags);

					method_t _method_{};

					if (accessFlags & 0x0008)
						_method_.is_static = true;
					else
						_method_.is_static = false;

					_method_.name = method_name;
					_method_.sig = signature;

					EntityLivingBase::methods.push_back(_method_);
				}

				for (int i = 0; i < f_count; i++)
				{
					auto field = fields[i];

					char* field_name;
					char* signature;

					jvmti->GetFieldName(mcc::EntityLivingBase, field, &field_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetFieldModifiers(mcc::EntityLivingBase, field, &accessFlags);

					field_t _field_{};

					if (accessFlags & 0x0008)
						_field_.is_static = true;
					else
						_field_.is_static = false;

					_field_.name = field_name;
					_field_.sig = signature;

					EntityLivingBase::fields.push_back(_field_);
				}

			}
		}
	
		namespace Entity {
			inline std::vector<field_t> fields;
			inline std::vector<method_t> methods;

			__forceinline auto Initialize() -> void
			{
				if (!mcc::Entity)
				{
					std::cout << "Entity not found\n";
					return;
				}
				jint m_count;
				jint f_count;
				jmethodID* methods;
				jfieldID* fields;

				jvmti->GetClassMethods(mcc::Entity, &m_count, &methods);
				jvmti->GetClassFields(mcc::Entity, &f_count, &fields);

				for (int i = 0; i < m_count; i++)
				{
					auto method = methods[i];

					char* method_name;
					char* signature;

					jvmti->GetMethodName(method, &method_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetMethodModifiers(method, &accessFlags);

					method_t _method_{};

					if (accessFlags & 0x0008)
						_method_.is_static = true;
					else
						_method_.is_static = false;

					_method_.name = method_name;
					_method_.sig = signature;
					mcc::impl::Entity::methods.push_back(_method_);
				}

				for (int i = 0; i < f_count; i++)
				{
					auto field = fields[i];

					char* field_name;
					char* signature;

					jvmti->GetFieldName(mcc::Entity, field, &field_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetFieldModifiers(mcc::Entity, field, &accessFlags);

					field_t _field_{};

					if (accessFlags & 0x0008)
						_field_.is_static = true;
					else
						_field_.is_static = false;

					_field_.name = field_name;
					_field_.sig = signature;

					mcc::impl::Entity::fields.push_back(_field_);
				}

			}
		}

		namespace AxisAlignedBB {
			inline std::vector<field_t> fields;
			inline std::vector<method_t> methods;

			__forceinline auto Initialize() -> void
			{
				if (!mcc::AxisAlignedBB)
				{
					std::cout << "AxisAlignedBB not found\n";
					return;
				}
				jint m_count;
				jint f_count;
				jmethodID* methods;
				jfieldID* fields;

				jvmti->GetClassMethods(mcc::AxisAlignedBB, &m_count, &methods);
				jvmti->GetClassFields(mcc::AxisAlignedBB, &f_count, &fields);

				for (int i = 0; i < m_count; i++)
				{
					auto method = methods[i];

					char* method_name;
					char* signature;

					jvmti->GetMethodName(method, &method_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetMethodModifiers(method, &accessFlags);

					method_t _method_{};

					if (accessFlags & 0x0008)
						_method_.is_static = true;
					else
						_method_.is_static = false;

					_method_.name = method_name;
					_method_.sig = signature;
					mcc::impl::AxisAlignedBB::methods.push_back(_method_);
				}

				for (int i = 0; i < f_count; i++)
				{
					auto field = fields[i];

					char* field_name;
					char* signature;

					jvmti->GetFieldName(mcc::AxisAlignedBB, field, &field_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetFieldModifiers(mcc::AxisAlignedBB, field, &accessFlags);

					field_t _field_{};

					if (accessFlags & 0x0008)
						_field_.is_static = true;
					else
						_field_.is_static = false;

					_field_.name = field_name;
					_field_.sig = signature;

					mcc::impl::AxisAlignedBB::fields.push_back(_field_);
				}

			}
		}
		namespace PlayerController {
			inline std::vector<field_t> fields;
			inline std::vector<method_t> methods;

			__forceinline void Initialize()
			{
				jint m_count;
				jint f_count;
				jmethodID* methods;
				jfieldID* fields;

				jvmti->GetClassMethods(mcc::PlayerController, &m_count, &methods);
				jvmti->GetClassFields(mcc::PlayerController, &f_count, &fields);

				for (int i = 0; i < m_count; i++)
				{
					auto method = methods[i];

					char* method_name;
					char* signature;

					jvmti->GetMethodName(method, &method_name, &signature, NULL);

					jint accessFlags;
					jvmti->GetMethodModifiers(method, &accessFlags);

					method_t _method_{};

					if (accessFlags & 0x0008)
						_method_.is_static = true;
					else
						_method_.is_static = false;

					_method_.name = method_name;
					_method_.sig = signature;
					PlayerController::methods.push_back(_method_);
				}

				for (int i = 0; i < f_count; i++)
				{
					auto field = fields[i];

					char* field_name;
					char* signature;

					jvmti->GetFieldName(mcc::PlayerController, field, &field_name, &signature, NULL);

					jint accessFlags;
					jvmti->GetFieldModifiers(mcc::PlayerController, field, &accessFlags);

					field_t _field_{};

					if (accessFlags & 0x0008)
						_field_.is_static = true;
					else
						_field_.is_static = false;

					_field_.name = field_name;
					_field_.sig = signature;

					PlayerController::fields.push_back(_field_);
				}

			}
		}
		namespace EntityPlayerSP {
			inline std::vector<field_t> fields;
			inline std::vector<method_t> methods;

			__forceinline void Initialize()
			{
				if(!mcc::EntityPlayerSP)
				{
					std::cout << "entityplayerSP not found\n";
					return;
				}
				jint m_count;
				jint f_count;
				jmethodID* methods;
				jfieldID* fields;

				jvmti->GetClassMethods(mcc::EntityPlayerSP, &m_count, &methods);
				jvmti->GetClassFields(mcc::EntityPlayerSP, &f_count, &fields);

				for (int i = 0; i < m_count; i++)
				{
					auto method = methods[i];

					char* method_name;
					char* signature;

					jvmti->GetMethodName(method, &method_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetMethodModifiers(method, &accessFlags);

					method_t _method_{};

					if (accessFlags & 0x0008)
						_method_.is_static = true;
					else
						_method_.is_static = false;

					_method_.name = method_name;
					_method_.sig = signature;
					EntityPlayerSP::methods.push_back(_method_);
				}

				for (int i = 0; i < f_count; i++)
				{
					auto field = fields[i];

					char* field_name;
					char* signature;

					jvmti->GetFieldName(mcc::EntityPlayerSP, field, &field_name, &signature, NULL);

					jint accessFlags;
					jvmti->GetFieldModifiers(mcc::EntityPlayerSP, field, &accessFlags);

					field_t _field_{};

					if (accessFlags & 0x0008)
						_field_.is_static = true;
					else
						_field_.is_static = false;

					_field_.name = field_name;
					_field_.sig = signature;

					EntityPlayerSP::fields.push_back(_field_);
				}

			}
		}

		namespace Potion {
			inline std::vector<field_t> fields;

			__forceinline void Initialize()
			{
				jint f_count;
				jfieldID* fields;

				jvmti->GetClassFields(mcc::Potion, &f_count, &fields);

				for (int i = 0; i < f_count; i++)
				{
					auto field = fields[i];

					char* field_name;
					char* signature;

					jvmti->GetFieldName(mcc::Potion, field, &field_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetFieldModifiers(mcc::Potion, field, &accessFlags);

					field_t _field_{};

					if (accessFlags & 0x0008)
						_field_.is_static = true;
					else
						_field_.is_static = false;

					_field_.name = field_name;
					_field_.sig = signature;

					Potion::fields.push_back(_field_);
				}

			}
		}
		namespace Item {
			inline std::vector<field_t> fields;
			inline std::vector<method_t> methods;

			__forceinline void Initialize()
			{
				jint m_count;
				jint f_count;
				jmethodID* methods;
				jfieldID* fields;

				jvmti->GetClassMethods(mcc::Item, &m_count, &methods);
				jvmti->GetClassFields(mcc::Item, &f_count, &fields);

				for (int i = 0; i < m_count; i++)
				{
					auto method = methods[i];

					char* method_name;
					char* signature;

					jvmti->GetMethodName(method, &method_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetMethodModifiers(method, &accessFlags);

					method_t _method_{};

					if (accessFlags & 0x0008)
						_method_.is_static = true;
					else
						_method_.is_static = false;

					_method_.name = method_name;
					_method_.sig = signature;
					Item::methods.push_back(_method_);
				}

				for (int i = 0; i < f_count; i++)
				{
					auto field = fields[i];

					char* field_name;
					char* signature;

					jvmti->GetFieldName(mcc::Item, field, &field_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetFieldModifiers(mcc::Item, field, &accessFlags);

					field_t _field_{};

					if (accessFlags & 0x0008)
						_field_.is_static = true;
					else
						_field_.is_static = false;

					_field_.name = field_name;
					_field_.sig = signature;

					Item::fields.push_back(_field_);
				}

			}
		}

		namespace Timer {
			inline std::vector<field_t> fields;
			inline std::vector<method_t> methods;

			__forceinline auto Initialize() -> void
			{
				if (mcc::Timer == nullptr)
				{
					std::cout << "Timer class not found!\n";
					return;
				}
				jint m_count;
				jint f_count;
				jmethodID* methods;
				jfieldID* fields;

				jvmti->GetClassMethods(mcc::Timer, &m_count, &methods);
				jvmti->GetClassFields(mcc::Timer, &f_count, &fields);

				for (int i = 0; i < m_count; i++)
				{
					auto method = methods[i];

					char* method_name;
					char* signature;

					jvmti->GetMethodName(method, &method_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetMethodModifiers(method, &accessFlags);

					method_t _method_{};

					if (accessFlags & 0x0008)
						_method_.is_static = true;
					else
						_method_.is_static = false;

					_method_.name = method_name;
					_method_.sig = signature;
					Timer::methods.push_back(_method_);
				}

				for (int i = 0; i < f_count; i++)
				{
					auto field = fields[i];

					char* field_name;
					char* signature;

					jvmti->GetFieldName(mcc::Timer, field, &field_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetFieldModifiers(mcc::Timer, field, &accessFlags);

					field_t _field_{};

					if (accessFlags & 0x0008)
						_field_.is_static = true;
					else
						_field_.is_static = false;

					_field_.name = field_name;
					_field_.sig = signature;

					Timer::fields.push_back(_field_);
				}

			}
		}

		namespace GameSettings {
			inline std::vector<field_t> fields;

			__forceinline auto Initialize() -> void
			{
				if (mcc::GameSettings == nullptr)
				{
					std::cout << "GameSettings class not found!\n";
					return;
				}
				jint m_count;
				jint f_count;
				jfieldID* fields;

				jvmti->GetClassFields(mcc::GameSettings, &f_count, &fields);

				for (int i = 0; i < f_count; i++)
				{
					auto field = fields[i];

					char* field_name;
					char* signature;

					jvmti->GetFieldName(mcc::GameSettings, field, &field_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetFieldModifiers(mcc::GameSettings, field, &accessFlags);

					field_t _field_{};

					if (accessFlags & 0x0008)
						_field_.is_static = true;
					else
						_field_.is_static = false;

					_field_.name = field_name;
					_field_.sig = signature;

					GameSettings::fields.push_back(_field_);
				}

			}
		}
		namespace KeyBinding {
			inline std::vector<field_t> fields;

			__forceinline auto Initialize() -> void
			{
				if (mcc::KeyBinding == nullptr)
				{
					std::cout << "KeyBinding class not found!\n";
					return;
				}
				jint m_count;
				jint f_count;
				jfieldID* fields;

				jvmti->GetClassFields(mcc::KeyBinding, &f_count, &fields);

				for (int i = 0; i < f_count; i++)
				{
					auto field = fields[i];

					char* field_name;
					char* signature;

					jvmti->GetFieldName(mcc::KeyBinding, field, &field_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetFieldModifiers(mcc::KeyBinding, field, &accessFlags);

					field_t _field_{};

					if (accessFlags & 0x0008)
						_field_.is_static = true;
					else
						_field_.is_static = false;

					_field_.name = field_name;
					_field_.sig = signature;

					KeyBinding::fields.push_back(_field_);
				}

			}
		}

		namespace ItemStack {
			inline std::vector<field_t> fields;
			inline std::vector<method_t> methods;

			__forceinline auto Initialize() -> void
			{
				if (mcc::ItemStack == nullptr)
				{
					std::cout << "ItemStack class not found!\n";
					return;
				}
				jint m_count;
				jint f_count;
				jmethodID* methods;
				jfieldID* fields;

				jvmti->GetClassMethods(mcc::ItemStack, &m_count, &methods);
				jvmti->GetClassFields(mcc::ItemStack, &f_count, &fields);

				for (int i = 0; i < m_count; i++)
				{
					auto method = methods[i];

					char* method_name;
					char* signature;

					jvmti->GetMethodName(method, &method_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetMethodModifiers(method, &accessFlags);

					method_t _method_{};

					if (accessFlags & 0x0008)
						_method_.is_static = true;
					else
						_method_.is_static = false;

					_method_.name = method_name;
					_method_.sig = signature;
					mcc::impl::ItemStack::methods.push_back(_method_);
				}

				for (int i = 0; i < f_count; i++)
				{
					auto field = fields[i];

					char* field_name;
					char* signature;

					jvmti->GetFieldName(mcc::ItemStack, field, &field_name, &signature, nullptr);

					jint accessFlags;
					jvmti->GetFieldModifiers(mcc::ItemStack, field, &accessFlags);

					field_t _field_{};

					if (accessFlags & 0x0008)
						_field_.is_static = true;
					else
						_field_.is_static = false;

					_field_.name = field_name;
					_field_.sig = signature;

					mcc::impl::ItemStack::fields.push_back(_field_);
				}

			}
		}
		
	};
	
}
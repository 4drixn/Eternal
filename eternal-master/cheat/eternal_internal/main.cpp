#include "main.hpp"

#include <Windows.h>
#include <jni.h>
#include <jvmti.h>
#include <thread>
#include <functional>
#include "global.hpp"
#include "functions.hpp"
#include "Minecraft.hpp"
#include "Player.hpp"
#include "Mapper.hpp"
#include "World.hpp"
#include "c_context.hpp"
#include "clicker.hpp"

#include "ActiveRenderInfo.hpp"
#include "Velocity.hpp"
#include "SelfDestruct.hpp"
#include "ArrayList.hpp"
#include "KeyBinds.hpp"
#include "Classes.hpp"
#include "Encryption.h"
#include "FastPlace.hpp"
#include "hits.hpp"
#include "RenderManager.hpp"
#include "ScaledResolution.hpp"

#include "unload.h"

typedef jint(*hJNI_GetCreatedJavaVMs)(JavaVM** vmBuf, jsize bufLen, jsize* nVMs);
hJNI_GetCreatedJavaVMs oJNI_GetCreatedJavaVMs;


std::vector<uintptr_t> lunar_offsets {
    0xB5EDB4, 0xB633D1, 0xB64810, 0xB64811, 0xB64812, 0xB64813, 0xB64814, 0xB64815, 0xB64818, 0xB64828, 0xB64900, 0xB64901, 0xB64902,
    0xB64903, 0xB64904, 0xB64905, 0xB64908, 0xB64909, 0xB6490A, 0xB6490B, 0xB6490C, 0xB6490D, 0xB64990, 0xB64991, 0xB64992, 0xB64993,
    0xB64994, 0xB64995, 0xB649A0, 0xB649A1, 0xB649A2, 0xB649A3, 0xB649A4, 0xB649A5, 0xB649B2, 0xB649B5, 0xB649C0, 0xB649C2, 0xB649D2,
    0xB649D5, 0xB649E0, 0xB649E2, 0xB65268
};

std::vector<uintptr_t> offsets{
    0x7F9580, 0x7F9581, 0x7F9582, 0x7F9583, 0x7F9588, 0x7F9590, 0x7F95D8, 0x7F95D9, 0x7F95DA, 0x7F95DB, 0x7F95E0, 0x7F95E1, 0x7F95E2,
    0x7F95E3, 0x7F9620, 0x7F9622, 0x7F9632, 0x7F9640, 0x7F9642, 0x7F9652, 0x7F9660, 0x7F9661, 0x7F9662, 0x7F9663, 0x7F9664, 0x7F9670,
    0x7F9671, 0x7F9672, 0x7F9673, 0x7F9674, 0x7F9675, 0x7F9858
};

std::vector<uintptr_t> blc_v3_offsets{
    0x816866, 0x816859
    //0x8005C7, 0x816E98, 0x816E99, 0x816EA0, 0x816EA1, 0x816EA2, 0x816EA3, 0x8176F9, 0x8176FA, 0x817700, 0x817701, 0x817702, 0x817703,
    //0x8187C1, 0x8187C2, 0x8187C3, 0x8187C8, 0x8187CC //0x8187C8 crasher
};

std::vector<uintptr_t> blc_v2_offsets{
    0x816866, 0x816859
    //0x7FC9A0, 0x8166F0, 0x8166F9, 0x816E98, 0x816EA0, 0x816EA1, 0x816EA2, 0x816EA3, 0x8176F9, 0x8176FA, 0x8176FB, 0x817700, 0x817701,
    //0x817702, 0x817CA9, 0x8187C1, 0x8187C8, 0x8187CC,
};

jobject hitvec_;

void RelinkModuleToPEB(HMODULE hModule)
{
    std::vector<UNLINKED_MODULE>::iterator it = std::find_if(UnlinkedModules.begin(), UnlinkedModules.end(), FindModuleHandle(hModule));

    if (it == UnlinkedModules.end())
    {
        //DBGOUT(TEXT("Module Not Unlinked Yet!"));
        return;
    }

    RELINK((*it).Entry->InLoadOrderLinks, (*it).RealInLoadOrderLinks);
    RELINK((*it).Entry->InInitializationOrderLinks, (*it).RealInInitializationOrderLinks);
    RELINK((*it).Entry->InMemoryOrderLinks, (*it).RealInMemoryOrderLinks);
    UnlinkedModules.erase(it);
}


struct block_t
{
    glm::vec3 position;
    jobject block_obj;

    block_t (jobject block, glm::vec3 _pos_)
    {
        block_obj = block; position = _pos_;
    }
};

std::vector<block_t> blocks{};
void bedbreaker()
{
    auto env = getJNI();
    auto minecraft = std::make_unique<c_minecraft>(getMC(env), env);

    mc = minecraft->get_object();

    while (!selfdestruct::should_destruct)
    {

        if (minecraft->getPlayer() && minecraft->getWorld())
        {
            auto local_player = std::make_unique<c_player>(minecraft->getPlayer(), env);
            auto world = std::make_shared<c_world>(minecraft->getWorld(), env);
            int radius = 3;

            std::vector<std::tuple<int, s_axisalignedbb>> temp_list{};
            std::vector<block_t> temp_blocks{};
            for (int x = -radius; x < radius; x++)
            {
                if (selfdestruct::should_destruct)
                    goto end;

                for (int y = radius; y > -radius; y--)
                {
                    if (selfdestruct::should_destruct)
                        goto end;

                    for (int z = -radius; z < radius; z++)
                    {
                        if (selfdestruct::should_destruct)
                            goto end;

                        vec3 cur_pos = local_player->get_position();
                        int n2 = (int)cur_pos.x + x;
                        int n3 = (int)cur_pos.y + y;
                        int n4 = (int)cur_pos.z + z;

                        auto blockpos = world->get_blocknigger(vec3({ (float)n2, (float)n3, (float)n4 }));
                        if (blockpos != nullptr)
                        {
                            auto bs = world->get_blockstate(blockpos);
                            if (bs != nullptr) {
                                auto block = world->get_block(bs);
                                if (block == nullptr)
                                    continue;

                                const auto getIdFromBlock = [&]() -> int
                                {
                                    if (global::is_lunar)
                                    {
                                        auto this_class = mcc::Block;
                                        auto mid = env->GetStaticMethodID(this_class, xorstr_("papppesaaeaspshepppshpeea"), xorstr_("(Lnet/minecraft/v1_8/hhaphashesapseehppaeaeaea;)I"));
                                        return env->CallStaticIntMethod(this_class, mid, block);
                                    }
                                    else {
                                        auto this_class = env->GetObjectClass(block);
                                        jmethodID mid = env->GetStaticMethodID(this_class, global::is_badlion ? xorstr_("a") : xorstr_("func_149682_b"), global::is_badlion ? xorstr_("(Lafh;)I") : xorstr_("(Lnet/minecraft/block/Block;)I"));

                                        env->DeleteLocalRef(this_class);

                                        return env->CallStaticIntMethod(this_class, mid, block);
                                    }

                                };


                                if (block != nullptr)
                                {
                                    int id = getIdFromBlock();
                                    if(id == 26)
                                    {
                                        temp_blocks.push_back(block_t(blockpos, glm::vec3(n2, n3, n4)));
                                        goto finish;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        finish:
            n_glrender::block_positions = temp_list;
            blocks = temp_blocks;
        }


        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

end:

    ExitThread(0);

}

void search()
{
    auto env = getJNI();
    auto minecraft = std::make_unique<c_minecraft>(getMC(env), env);

    mc = minecraft->get_object();

    while (!selfdestruct::should_destruct)
    {

        //if (selfdestruct::should_destruct)
        //    ExitThread(-1);

        if (minecraft->getPlayer() && minecraft->getWorld()) 
        {
            auto local_player = std::make_unique<c_player>(minecraft->getPlayer(), env);
            auto world = std::make_unique<c_world>(minecraft->getWorld(), env);
            int n = 32; // 25

            std::vector<std::tuple<int, s_axisalignedbb>> temp_list{};
            for (int x = -n; x < n; x++)
            {
                if (selfdestruct::should_destruct)
                    goto end;
                for (int y = n; y > -n; y--)
                {
                    if (selfdestruct::should_destruct)
                        goto end;
                    for (int z = -n; z < n; z++)
                    {
                        if (selfdestruct::should_destruct)
                            goto end;

                        vec3 cur_pos = local_player->get_position();
                        int n2 = (int)cur_pos.x + x;
                        int n3 = (int)cur_pos.y + y;
                        int n4 = (int)cur_pos.z + z;

                        auto blockpos = world->get_blocknigger(vec3({ (float)n2, (float)n3, (float)n4}));
                        if (blockpos != nullptr)
                        {
                            auto bs = world->get_blockstate(blockpos);
                            if (bs != nullptr) {
                                auto block = world->get_block(bs);
                                if (block == nullptr)
                                    continue;

                                const auto getIdFromBlock = [&]() -> int
                                {
                                    if(global::is_lunar)
                                    {
                                        auto this_class = mcc::Block;
                                        auto mid = env->GetStaticMethodID(this_class, xorstr_("papppesaaeaspshepppshpeea"), xorstr_("(Lnet/minecraft/v1_8/hhaphashesapseehppaeaeaea;)I"));
                                        return env->CallStaticIntMethod(this_class, mid, block);
                                    }
                                    else {
                                        auto this_class = env->GetObjectClass(block);
                                        jmethodID mid = env->GetStaticMethodID(this_class, global::is_badlion ? xorstr_("a") : xorstr_("func_149682_b"), global::is_badlion ? xorstr_("(Lafh;)I") : xorstr_("(Lnet/minecraft/block/Block;)I"));

                                        env->DeleteLocalRef(this_class);

                                        return env->CallStaticIntMethod(this_class, mid, block);
                                    }

                                };

                                std::vector<int> block_ids
                                {
                                    56, // diamond
                                        //   21, // lapis
                                        //16, // coal
                                        //73, // redstone
                                        //   129, // emerald
                                        //   14, // gold
                                        //   15, // iron
                                        //   54, // chest
                                        //   130 // enderchest
                                        54
                                };

                                if (block != NULL)
                                {
                                    int id = getIdFromBlock();
                                    if (std::count(block_ids.begin(), block_ids.end(), id))
                                    {
                                        //printf("[dbg] %i, %i, %i\n", n2, n3, n4);
                                        temp_list.push_back(std::make_tuple(id, s_axisalignedbb({ (double)n2, (double)n3, (double)n4, (double)n2 + 1, (double)n3 + 1, (double)n4 + 1 })));
                                    }
                                }
                            }
                        }
                    }
                }
            }
            n_glrender::block_positions = temp_list;
        }


        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    }

end:

    ExitThread(0);

}

__declspec(noreturn) void ExitDll(void* ImageBase, SIZE_T SizeOfImage)
{
    //VirtualFree(ImageBase, SizeOfImage, MEM_RELEASE);
    //ExitThread(-1);
    auto* pmemset = memset;
    auto* pVirtualFree = VirtualFree;
    auto* pExitThread = ExitThread;

    pmemset(ImageBase, 0, SizeOfImage);
    pVirtualFree(ImageBase, 0, MEM_RELEASE);
    pExitThread(0);
}

void main_thread(JNIEnv* env)
{
    auto minecraft = std::make_unique<c_minecraft>(getMC(env), env);

    static std::shared_ptr<c_timer> timer2 = std::make_shared<c_timer>();

    mc = minecraft->get_object();

    while (!selfdestruct::should_destruct)
    {
        if (minecraft->getPlayer() && minecraft->getWorld()) // Ingame check
        {
            static std::shared_ptr<c_timer> timer_uwu = std::make_shared<c_timer>();

            auto local_player = std::make_unique<c_player>(minecraft->getPlayer(), env);
            auto world = std::make_unique<c_world>(minecraft->getWorld(), env);

            c_context context{ &minecraft, &local_player, &world, env };

            const auto timer_t = std::make_unique<timer>(minecraft->getTimer());

            const auto activerender = std::make_shared<c_activerenderinfo>(env);
            const auto rendermanager = std::make_shared<c_rendermanager>(minecraft->getRenderManager());

            n_glrender::PROJECTION = activerender->Get_PROJECTION();
            n_glrender::MODELVIEW = activerender->Get_MODELVIEW();
            n_glrender::ViewPort = activerender->Get_VIEWPORT();
            n_glrender::view_pos = rendermanager->get_viewpos();

            keybinds::onUpdate(&context);

            flight::flight(&context);
            speed::speed_(&context);
            killaura::aura(&context);

            aimassist::aim(&context);

            //autotool::autotool(&context); // needs to get fixed
            Criticals::crits(&context);
            //nofall::nofall(&context);
            cheststealer::stealer(&context);
            //autoarmor::autoarmor_(&context); // has to be in a thread
            //m_timer::timer_(&context);
            //sprint::sprint;

            antibot::antibot(&context);
            reach::reach(&context);
            velocity::velocity(&context);
            arraylist::draw(&context);

            clicker::clicker(&context);
            misc::fix_hit_delay(&context);
            misc::fastplace::fast_place(&context);
            fastmine::fastmine(&context);
            MCF::onUpdate(&context);
            //misc::sumoborder::onUpdate(&context);
            //faststop::faststop(&context);

            bridgeassist::bridge(&context);

        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }   

    c_main::get().unhook();

    // printf(xorstr_("[DEBUG] Selfdestructing... [%p]\n"), global::instance);

    HMODULE dababy = GetModuleHandleA(xorstr_("jvm.dll"));

    // maybe you will have to update these offsets idk lmk if they crash
    if (global::is_lunar)
    {
        for (const auto& offset : lunar_offsets)
        {
            uintptr_t address = reinterpret_cast<uintptr_t>(dababy);
            address += offset;

            *reinterpret_cast<DWORD*>(address) = 0;
        }
    }
    else
    {
        if(global::is_badlion)
        {
            //if(global::V_3)
            //{
            //          for (const auto& offset : blc_v3_offsets)
            //          {
            //              uintptr_t address = reinterpret_cast<uintptr_t>(dababy);
            //              address += offset;

            //              *reinterpret_cast<DWORD*>(address) = 0;
            //          }
            //}
            //      else
            //      {
            //          for (const auto& offset : blc_v2_offsets)
            //          {
            //              uintptr_t address = reinterpret_cast<uintptr_t>(dababy);
            //              address += offset;

            //              *reinterpret_cast<DWORD*>(address) = 0;
            //          }
            //      }
        }
        else {
            for (auto offset : offsets)
            {
                uintptr_t address = reinterpret_cast<uintptr_t>(dababy);
                address += offset;

                *reinterpret_cast<DWORD*>(address) = 0;
            }
        }

    }


    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    FreeConsole();
    ShowWindow(GetConsoleWindow(), SW_HIDE);


    global::jvm->DetachCurrentThread();
    global::jvm = nullptr;

    //  global::jvm->DestroyJavaVM();
    //  Beep(300, 300);


    global::jvmHandle = nullptr;

    //  free(global::instance);
    RelinkModuleToPEB(global::instance);


    FreeLibraryAndExitThread(global::instance, ERROR_SUCCESS);
    // FreeLibraryAndExitThread(global::instance, -1);

}

namespace initialize {

    void __stdcall init()
    {
        jvmtiCapabilities capabilities;

        global::jvmHandle = GetModuleHandleA(xorstr_("jvm.dll"));
        global::func_JNI_GetCreatedJavaVMs = GetProcAddress(global::jvmHandle, xorstr_("JNI_GetCreatedJavaVMs"));

        oJNI_GetCreatedJavaVMs = (hJNI_GetCreatedJavaVMs)global::func_JNI_GetCreatedJavaVMs;
        jint returnOF = oJNI_GetCreatedJavaVMs(&global::jvm, 1, nullptr);

        jint returnOF2 = global::jvm->AttachCurrentThread((void**)&global::jenv, nullptr);

        int getEnvStat = global::jvm->GetEnv((void**)&global::jvmti, JVMTI_VERSION_1_2);

        (void)memset(&capabilities, 0, sizeof(jvmtiCapabilities));

        capabilities.can_retransform_classes = 1;
        capabilities.can_generate_exception_events = 1;
        capabilities.can_access_local_variables = 1;
        capabilities.can_get_constant_pool = 1;
        capabilities.can_get_bytecodes = 1;
        capabilities.can_generate_all_class_hook_events = 1;

        global::jvmti->AddCapabilities(&capabilities);
        global::jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_EXCEPTION, (jthread)nullptr);


        HANDLE thread1 = nullptr;
        HANDLE thread2 = nullptr;
        if (global::jenv != nullptr)
        {
            jvmtiEventCallbacks callbacks = { nullptr };
            global::jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));

            if (global::is_lunar)
            {
                mapper = std::make_shared<Mapper>();

                mcc::Initialize();

                mcc::impl::ave::Initialize();
                mcc::impl::World::Initialize();
                mcc::impl::Entity::Initialize();
                mcc::impl::EntityPlayer::Initialize();
                mcc::impl::EntityPlayerSP::Initialize();
                mcc::impl::EntityLivingBase::Initialize();
                mcc::impl::AxisAlignedBB::Initialize();
                mcc::impl::PlayerController::Initialize();
                mcc::impl::Potion::Initialize();
                mcc::impl::Item::Initialize();
                mcc::impl::Timer::Initialize();
                mcc::impl::GameSettings::Initialize();
                mcc::impl::KeyBinding::Initialize();
                mcc::impl::ItemStack::Initialize();
                mcc::impl::ModelPlayer::Initialize();
            }


            c_main::get().hook();

            if (!global::is_lunar)
            {
                postPreInit(global::jenv);
            }

            //CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)search, NULL, 0, nullptr));
            //CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)bedbreaker, NULL, 0, nullptr));

            // put in different files pleaseee

            main_thread(global::jenv);

            //thread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)autoarmor::autoarmor_, NULL, 0, nullptr);


            if(!global::is_lunar)
                cleanup(global::jenv);
        }
        //TerminateThread(thread1, 1);

        if (global::jenv->ExceptionCheck())
        {
            global::jenv->ExceptionDescribe();
        }


        //FreeLibraryAndExitThread(global::instance, 0);
    }
}


//int n = 12;
//for (int i = -n; i < n; ++i)
//{
//    for (int j = n; j > -n; --j)
//    {
//        for (int k = -n; k < n; ++k)
//        {
//            vec3 cur_pos = local_player->get_position();
//            int n2 = (int)cur_pos.x + i;
//            int n3 = (int)cur_pos.y + j;
//            int n4 = (int)cur_pos.z + k;
//
//            auto blockpos = world->get_blocknigger(vec3({ (float)cur_pos.x, (float)cur_pos.y - 1, (float)cur_pos.z }));
//            if (blockpos != NULL)
//            {
//                auto bs = world->get_blockstate(blockpos);
//                auto block = world->get_block(bs);
//                if (block != NULL)
//                {
//                    if (world->is_bed(block))
//                    {
//                        local_player->swing_item();
//                        //world->break_block(blockpos, block, bs);
//                        local_player->get_sendQueue()->addToSendQueue(packets::C07PacketPlayerDigging::init(blockpos, "START_DESTROY_BLOCK"));
//                        //local_player->get_sendQueue()->addToSendQueue(packets::C07PacketPlayerDigging::init(blockpos, "STOP_DESTROY_BLOCK"));
//                        printf("Bed found at: %d, %d, %d\n", n2, n3, n4);
//
//                    }
//                }
//            }
//        }
//    }
//}

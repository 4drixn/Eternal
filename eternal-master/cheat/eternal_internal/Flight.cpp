#include "Flight.hpp"
#include "Packet.hpp"
#include "Potion.hpp"
#include "Timer.h"
#include "timer.hpp"
#include "config.hpp"
#include "global.hpp"
#include "FakeLag.hpp"

int kkr_stage = 0;

int stage = 0;
int ticks = 0;
float y = 0;

void flight::flight(c_context* ctx)
{
    static std::shared_ptr<c_timer> timer_ = std::make_shared<c_timer>();

    if (!timer_->has_passed(50))
        return;

    timer_->reset();

    auto local_player = ctx->local_player->get();
    auto timer_t = std::make_shared<timer>(ctx->minecraft->get()->getTimer());

    if (config::flight::selected_mode == 0) // Hypixel
    {
        if (flight::m_enabled)
        {
            static auto potion = std::make_shared<c_potion>();
            static jobject pot = potion->get_speed_potion();

            float speedf = 0.29f + local_player->get_move_speed(pot) * 0.06f;

            if (local_player->get_moveforward() > 0)
                stage++;

            //if (local_player->on_ground())
            //{
            //    local_player->jump();
            //}
            //else
            {
                if (stage > 2)
                    local_player->SetmotionY(0.f);

                vec3 pos = local_player->get_position();
                if (stage > 2)
                {
                    local_player->set_pos(vec3({ pos.x, pos.y - 0.003f, pos.z }), false);

                    ticks++;
                    float offset = 3.25E-4f;

                    switch (ticks)
                    {
                    case 1:
                        y *= -0.949999988079071f;
                        break;
                    case 2:
                    case 3:
                    case 4:
                        y += 3.25E-4f;
                        break;
                    case 5:
                        y += 5.0E-4f;
                        ticks = 0;
                        break;
                    }


                    float the_yaw = local_player->getDirection();
                    float x_ = -sin(the_yaw) * speedf;
                    float z_ = cos(the_yaw) * speedf;
                    local_player->SetmotionX(x_);
                    local_player->SetmotionZ(z_);

                    local_player->get_sendQueue()->addToSendQueue(packets::C04PacketPlayerPosition::init(pos.x, pos.y + y, pos.z, false));

                    local_player->set_pos(vec3({ pos.x, pos.y - 0.003f, pos.z }), false);
                }
            }

        }
        else {
            ticks = 0;
            y = 0;
            stage = 0;
        }
    }
    else if (config::flight::selected_mode == 1) // Vanilla
    {
        const auto& env = ctx->env;

        if (flight::m_enabled)
        {
          //  fakelag::should_do_it = true;
            jobject capabilities = local_player->get_capabilities();
            static jclass this_clazz = env->GetObjectClass(capabilities);

            jfieldID setFlySpeed_mid = 0;
            jfieldID isFlying_fid = 0;
            
            if (global::is_lunar) {
                setFlySpeed_mid = env->GetFieldID(this_clazz, "eapspahheaasaeeashsasshpp", "F");
            }
            else
                setFlySpeed_mid = env->GetFieldID(this_clazz, global::is_badlion ? "f" : "field_75096_f", "F");

            if (global::is_lunar) {
                isFlying_fid = env->GetFieldID(this_clazz, "eahaesshaeapehehpasphshpe", "Z");
            }
            else
                isFlying_fid = env->GetFieldID(this_clazz, global::is_badlion ? "b" : "field_75100_b", "Z");

            //jfieldID allowFlying_fid = env->GetFieldID(this_clazz, global::is_badlion ? "c" : "field_75101_c", "Z");

            env->SetFloatField(capabilities, setFlySpeed_mid, (flight::m_speed / 10) );
            env->SetBooleanField(capabilities, isFlying_fid, true);
            //env->SetBooleanField(capabilities, allowFlying_fid, true);    

            //env->DeleteLocalRef(capabilities);
            //env->DeleteLocalRef(this_clazz);
        }
        else {
          //  fakelag::should_do_it = false;
            jobject capabilities = local_player->get_capabilities();
            static jclass this_clazz = env->GetObjectClass(capabilities);

            //jfieldID allowFlying_fid = env->GetFieldID(this_clazz, global::is_badlion ? "c" : "field_75101_c", "Z");
            jfieldID isFlying_fid;

            if (global::is_lunar) {
                isFlying_fid = env->GetFieldID(this_clazz, "eahaesshaeapehehpasphshpe", "Z");
            }
            else
                isFlying_fid = env->GetFieldID(this_clazz, global::is_badlion ? "b" : "field_75100_b", "Z");


            env->SetBooleanField(capabilities, isFlying_fid, false);
            //env->SetBooleanField(capabilities, allowFlying_fid, false);

            env->DeleteLocalRef(capabilities);
           // env->DeleteLocalRef(this_clazz);
        }
    }


}






//vec3 cur_pos = local_player->get_position();
//local_player->SetmotionY(0);
//local_player->set_lastReportedPosY(0);

////if (local_player->get_ticksExisted() % 3)
////{
////    cur_pos = local_player->get_position();

////    float d = cur_pos.y - 1.0E-10;
////    //local_player->set_pos(vec3({ cur_pos.x, d, cur_pos.z }), false);

////   // local_player->get_sendQueue()->addToSendQueue(packets::C03PacketPlayer::init(true));
////    local_player->get_sendQueue()->addToSendQueue(packets::C04PacketPlayerPosition::init(cur_pos.x, d, cur_pos.z, true));

////}

//static auto potion = std::make_shared<c_potion>();
//static jobject pot = potion->get_speed_potion();

//float speedf = 0.29f + local_player->get_move_speed(pot) * 0.06f;

//if (local_player->get_moveforward() > 0)
//{
//    speedf += 2 / 18;
//}
//else {
//    speedf = 0;
//}


//

//local_player->set_jump_mov_fac(0);
//local_player->set_on_ground(false);
//cur_pos = local_player->get_position();

//switch (kkr_stage)
//{
//case 0:
//    local_player->set_pos(vec3({ cur_pos.x,  cur_pos.y + (float)1.9999E-8, cur_pos.z }), false);
//    kkr_stage++;
//    break;
//case 1:
//    local_player->set_pos(vec3({ cur_pos.x, cur_pos.y + (float)1E-8, cur_pos.z }), false);
//    kkr_stage++;
//    break;
//case 2:
//    local_player->set_pos(vec3({ cur_pos.x, cur_pos.y + (float)1.99999E-8, cur_pos.z }), false);
//    kkr_stage = 0;  
//    break;
//default:
//    kkr_stage = 0;
//    break;

//}



















// FUNNY HOUSING FLIGHT


//if (flight::m_enabled)
//{
//    static auto potion = std::make_shared<c_potion>();
//    static jobject pot = potion->get_speed_potion();
//
//    float speedf = 1.29f + local_player->get_move_speed(pot) * 0.06f;
//
//    float motionY = 0;
//
//    if (local_player->get_moveforward() > 0)
//        stage++;
//    if (GetAsyncKeyState(VK_SPACE))
//        motionY = 2;
//    if (GetAsyncKeyState(VK_CONTROL))
//        motionY = -2;
//    //if (local_player->on_ground())
//    //{
//    //    local_player->jump();
//    //}
//    //else
//    {
//        if (stage > 2)
//            local_player->SetmotionY(motionY);
//
//        vec3 pos = local_player->get_position();
//        if (stage > 2)
//        {
//            //local_player->set_pos(vec3({ pos.x, pos.y - 0.003f, pos.z }), false);
//
//            ticks++;
//            float offset = 3.25E-4f;
//
//            switch (ticks)
//            {
//            case 1:
//                y *= -0.949999988079071f;
//                break;
//            case 2:
//            case 3:
//            case 4:
//                y += 3.25E-4f;
//                break;
//            case 5:
//                y += 5.0E-4f;
//                ticks = 0;
//                break;
//            }
//
//
//            float the_yaw = local_player->getDirection();
//            float x_ = -sin(the_yaw) * speedf;
//            float z_ = cos(the_yaw) * speedf;
//            local_player->SetmotionX(x_);
//            local_player->SetmotionZ(z_);
//
//            local_player->get_sendQueue()->addToSendQueue(packets::C04PacketPlayerPosition::init(pos.x, pos.y + y, pos.z, false));
//
//
//            // local_player->set_pos(vec3({ pos.x, pos.y - 0.003f, pos.z }), false);
//        }
//    }
//
//}
//else {
//    ticks = 0;
//    y = 0;
//    stage = 0;
//}

    //if (flight::m_enabled)
    //{
    //    // Verus

    //    local_player->set_sprinting(true);
    //    timer_t->set_timer_speed(1.0009499f);
    //    local_player->set_on_ground(true);
    //    local_player->SetmotionY(0.f);
    //}
    //else {
    //    timer_t->set_timer_speed(1.f);
    //}




//public class pheepsehhhasaespaaehspses implements llIllllIIlIIlIIIIIIIIIlII {
//    public boolean sppspapaashhhpeeeapehapss;
//
//    public boolean aseshpahpeaspephesahsaehs;
//
//    public boolean ahpeapahhahhasphpephspese;
//
//    public boolean aahapahpphehpeshapspapeas;
//
//    public boolean ephassappepeeeehsehpapppp = true;
//
//    public float epspessaheaheeshspepeesss = 0.05F;
//
//    public float ehhsehssasaeshephpehpesha = 0.1F;

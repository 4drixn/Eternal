#pragma once

#include "Player.hpp"
#include "World.hpp"
#include "Minecraft.hpp"


struct c_context{
	std::unique_ptr<c_minecraft>* minecraft;
	std::unique_ptr<c_player>* local_player;
	std::unique_ptr<c_world>* world;
	JNIEnv* env;
};

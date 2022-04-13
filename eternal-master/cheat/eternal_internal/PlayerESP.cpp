#include "PlayerESP.hpp"
#include "player_utils.hpp"
#include "vec4.hpp"
#include "vec2.hpp"
#include "ActiveRenderInfo.hpp"
#include "Libs/imgui/imgui.h"
#include "c_glrender.hpp"
#include "c_gltext.hpp"
#include "RenderManager.hpp"


void playeresp::esp(c_context* ctx)
{
	auto minecraft = ctx->minecraft->get();
	auto timer_t = std::make_unique<timer>(ctx->minecraft->get()->getTimer());
	auto rendermanager = std::make_unique<c_rendermanager>(minecraft->getRenderManager());
	auto world = ctx->world->get();


	// put esp here
}
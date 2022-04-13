#include "ArrayList.hpp"
#include <jni.h>
#include <vector>
#include <mutex>
#include "Modules.hpp"
#include "main.hpp"
#include "Gui.hpp"
#include "ScaledResolution.hpp"

#include "Flight.hpp"
#include "m_timer.hpp"
#include "AutoArmor.hpp"
#include "AntiBot.hpp"
#include "FastMine.hpp"
#include "FastStop.hpp"
#include "FastPlace.hpp"
#include "NoFall.hpp"
#include "AutoTool.hpp"
#include "Color.h"
#include "utils.hpp"

#include "ActiveRenderInfo.hpp"
#include "c_gltext.hpp"
#include "c_gui.hpp"
#include "Encryption.h"
#include "RenderGlobal.hpp"
#include "RenderManager.hpp"

float flSpeed = 0.00003f;
float flRainbow = 0;
Color colColor(0, 0, 0, 255);

bool vertical = false;
bool horizontal = true;
bool _fade_ = true;


void drawText(ImFont* font2 = thin_font, float dis = 0.f, ImVec2 pos = ImVec2(0, 0), const std::string& text = "", bool center = false)
{
	auto DrawList = ImGui::GetOverlayDrawList();

	ImGui::PushFont(font2);
	if (center)
	{
		pos.x -= ImGui::CalcTextSize(text.c_str()).x / 2;
	}

	DrawList->AddText(font2, dis, ImVec2(pos.x, pos.y), IM_COL32_WHITE, text.c_str());

	ImGui::PopFont();
}


void drawTextOutlined(float dis, ImVec2 pos, const std::string& text, bool center = false)
{
	const auto& DrawList = ImGui::GetBackgroundDrawList();
	ImGui::PushFont(thin_font);
	if(center)
		pos.x -= ImGui::CalcTextSize(text.c_str()).x / 2;

	DrawList->AddText(thin_font, dis, ImVec2(pos.x + 1, pos.y + 1), IM_COL32_BLACK, text.c_str());

	DrawList->AddText(thin_font, dis, ImVec2(pos.x - 1, pos.y - 1), IM_COL32_BLACK, text.c_str());

	DrawList->AddText(thin_font, dis, ImVec2(pos.x + 1, pos.y - 1), IM_COL32_BLACK, text.c_str());

	DrawList->AddText(thin_font, dis, ImVec2(pos.x - 1, pos.y + 1), IM_COL32_BLACK, text.c_str());

	DrawList->AddText(thin_font, dis, ImVec2(pos.x + 1, pos.y), IM_COL32_BLACK, text.c_str());

	DrawList->AddText(thin_font, dis, ImVec2(pos.x - 1, pos.y), IM_COL32_BLACK, text.c_str());

	DrawList->AddText(thin_font, dis, ImVec2(pos.x, pos.y - 1), IM_COL32_BLACK, text.c_str());

	DrawList->AddText(thin_font, dis, ImVec2(pos.x, pos.y + 1), IM_COL32_BLACK, text.c_str());

	DrawList->AddText(thin_font, dis, ImVec2(pos.x, pos.y), IM_COL32_WHITE, text.c_str());

	ImGui::PopFont();
}

__forceinline std::string fade(int offset)
{
	float index = 100;

	auto time = std::chrono::system_clock::now();
	auto since_epoch = time.time_since_epoch();
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch);

	double rainbowState = ceil((millis.count() + offset) / 20);
	double lol = std::fmod(rainbowState, 360);

	float loly = std::fmod(millis.count(), 2000); // idk?

	float brightness = abs(std::fmod((loly / 1000.0F + (float)index / (float)offset * 2.0F), 2.0f) - 1.0F);

	brightness = 0.5F + 0.5F * brightness;

	Color colRainbow = colColor.FromHSB(1, 1.f, 1.f);

	colRainbow = colColor.FromHSB(0.98888888888, 0.8f, std::fmod(brightness, 2.f));

	return utils::colours::RGB2MC(colRainbow.r, colRainbow.g, colRainbow.b);
}

__forceinline std::string rainbow(int offset)
{
	auto time = std::chrono::system_clock::now();
	auto since_epoch = time.time_since_epoch();
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch);

	double rainbowState = ceil((millis.count() + offset) / 20);
	double lol = std::fmod(rainbowState, 360);

	Color colRainbow = colColor.FromHSB(1, 1.f, 1.f);

	colRainbow = colColor.FromHSB((float)(lol / 360.0f), 0.8f, 0.8);

	return utils::colours::RGB2MC(colRainbow.r, colRainbow.g, colRainbow.b);
}



std::vector<std::tuple<std::string, bool*>> ModuleList;
std::vector<std::tuple<std::string, bool*>> EnabledModules;

bool sortbysec(const std::tuple<std::string, bool*>& a,
	const std::tuple<std::string, bool*>& b)
{
	return (std::get<0>(a).size() > std::get<0>(b).size());
}

// TODO: 

void initlist()
{
	ModuleList.push_back(std::make_tuple<>(xorstr_("Auto Clicker"), &clicker::m_enabled));
	ModuleList.push_back(std::make_tuple<>(xorstr_("Aim Assist"), &aimassist::m_enabled));
	ModuleList.push_back(std::make_tuple<>(xorstr_("KillAura"), &killaura::m_enabled));
	ModuleList.push_back(std::make_tuple<>(xorstr_("Criticals"), &Criticals::m_enabled));
	ModuleList.push_back(std::make_tuple<>(xorstr_("Velocity"), &velocity::m_enabled));
	ModuleList.push_back(std::make_tuple<>(xorstr_("Reach"), &reach::m_enabled));
	ModuleList.push_back(std::make_tuple<>(xorstr_("AntiBot"), &antibot::m_enabled));

	ModuleList.push_back(std::make_tuple<>(xorstr_("Flight"), &flight::m_enabled));
	ModuleList.push_back(std::make_tuple<>(xorstr_("Speed"), &speed::m_enabled));
	ModuleList.push_back(std::make_tuple<>(xorstr_("AutoTool"), &autotool::m_enabled));

	ModuleList.push_back(std::make_tuple<>(xorstr_("AutoArmor"), &autoarmor::m_enabled));
	//ModuleList.push_back(std::make_tuple<>("FakeLag", (&fakelag::outbound::m_enabled || &fakelag::inbound::m_enabled)));

	ModuleList.push_back(std::make_tuple<>(xorstr_("Sprint"), &sprint::m_enabled));
	ModuleList.push_back(std::make_tuple<>(xorstr_("NoFall"), &nofall::m_enabled));
	ModuleList.push_back(std::make_tuple<>(xorstr_("FastMine"), &fastmine::m_enabled));
	ModuleList.push_back(std::make_tuple<>(xorstr_("FastStop"), &faststop::m_enabled));
	ModuleList.push_back(std::make_tuple<>(xorstr_("FastPlace"), &misc::fastplace::m_enabled));
	ModuleList.push_back(std::make_tuple<>(xorstr_("ChestStealer"), &cheststealer::m_enabled));
	ModuleList.push_back(std::make_tuple<>(xorstr_("BridgeAssist"), &bridgeassist::m_enabled));
	ModuleList.push_back(std::make_tuple<>(xorstr_("PlayerESP"), &playeresp::m_enabled));
	//ModuleList.push_back(std::make_tuple<>("ArrayList", &arraylist::m_enabled));
	ModuleList.push_back(std::make_tuple<>(xorstr_("Timer"), &m_timer::enabled));

	sort(ModuleList.begin(), ModuleList.end(), sortbysec);
}

int offset = 50;
std::string allowed_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

void arraylist::onRender()
{
	static JNIEnv* env = getJNI();
	if (env != nullptr && GetForegroundWindow() == FindWindowA("LWJGL", nullptr))
	{
		const auto minecraft = std::make_unique<c_minecraft>(getMC(env), env);
		const auto font_renderer = std::make_shared<c_fontrenderer>(minecraft->getFontRenderer(), env);

		const auto ScaledResolution = std::make_shared<c_scaledresolution>(minecraft->get_scaled_resolution());

		const auto local_player = std::make_unique<c_player>(minecraft->getPlayer(), env);

		const auto world = std::make_unique<c_world>(minecraft->getWorld(), env);

		if (local_player->get_object() == nullptr || world->get_object() == nullptr || minecraft->getCurrentScreen() != nullptr)
			return;

		auto rendermanager = std::make_shared<c_rendermanager>(minecraft->getRenderManager(), env);

		const auto gui = std::make_unique<c_gui>();
		float offsetty = 7.5;
		float closest = FLT_MAX;
		// was working on this drawhead for a nice draw targethud
		//	gui->drawHead(NetworkPlayerHandler->getPlayerInfo("Cptegon"), 30, 30);

		flRainbow += flSpeed;
		auto timer_t = std::make_unique<timer>(minecraft->getTimer());

		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		glDepthMask(true);

		glLineWidth(1.0f);

		const float renderPartialTicks = timer_t->get_renderPartialTicks();
		const auto local_pos = local_player->get_position();
		const auto last_tick_pos = local_player->get_last_tick_pos();

		static std::map<std::string, float> map;

		//auto render_pos = n_glrender::render_pos;//glm::vec3(last_tick_pos.x + (local_pos.x - last_tick_pos.x) * renderPartialTicks, last_tick_pos.y + (local_pos.y - last_tick_pos.y) * renderPartialTicks, last_tick_pos.z + (local_pos.z - last_tick_pos.z) * renderPartialTicks);// n_glrender::render_pos;//
		auto render_pos = glm::vec3(last_tick_pos.x + (local_pos.x - last_tick_pos.x) * renderPartialTicks, last_tick_pos.y + (local_pos.y - last_tick_pos.y) * renderPartialTicks, last_tick_pos.z + (local_pos.z - last_tick_pos.z) * renderPartialTicks);// n_glrender::render_pos;//glm::vec3(last_tick_pos.x + (local_pos.x - last_tick_pos.x) * renderPartialTicks, last_tick_pos.y + (local_pos.y - last_tick_pos.y) * renderPartialTicks, last_tick_pos.z + (local_pos.z - last_tick_pos.z) * renderPartialTicks);// n_glrender::render_pos;//

		ImGuiStyle& style = ImGui::GetStyle();

		ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
		ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f } );
		ImGui::PushStyleColor( ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f } );

		ImGui::Begin( ( "##rrr" ), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs );
		{
			ImGui::SetWindowPos( ImVec2( 0, 0 ), ImGuiCond_Always );
			ImGui::SetWindowSize( ImVec2( ImGui::GetIO( ).DisplaySize.x, ImGui::GetIO( ).DisplaySize.y ), ImGuiCond_Always );
			{
				if (config::visuals::esp::esp_mode == 0 && playeresp::m_enabled)
				{
					try {
						for (const auto& player : world->get_players())
						{

							if (player == nullptr)
								continue;

							if (player->get_object() == nullptr)
								continue;

							if (env->IsSameObject(local_player->get_object(), player->get_object()))
								continue;

							if (!player->isAlive())
								continue;
							/* Ignore Bots */
							//std::string name = player->get_name();
							//if (allowed_chars.find(name[0]) == std::string::npos)
							//	continue;

							//std::string name = player->get_name();
							//if (player->is_invisible() && allowed_chars.find(name[0]) == std::string::npos)
							//	continue;

							float dis = local_player->get_distance_to(player);

							auto player_pos = player->get_position();
							auto last_tick_player_pos = player->get_last_tick_pos();

							glm::vec4 w2sbb = glm::vec4(FLT_MAX, FLT_MAX, -1.f, -1.f);


							auto bb = player->get_native_non_modified_boundingbox();
							bb.minX += -render_pos.x + -player_pos.x + last_tick_player_pos.x + (player_pos.x - last_tick_player_pos.x) * renderPartialTicks;
							bb.minY += -render_pos.y + -player_pos.y + last_tick_player_pos.y + (player_pos.y - last_tick_player_pos.y) * renderPartialTicks;
							bb.minZ += -render_pos.z + -player_pos.z + last_tick_player_pos.z + (player_pos.z - last_tick_player_pos.z) * renderPartialTicks;
							bb.maxX += -render_pos.x + -player_pos.x + last_tick_player_pos.x + (player_pos.x - last_tick_player_pos.x) * renderPartialTicks;
							bb.maxY += -render_pos.y + -player_pos.y + last_tick_player_pos.y + (player_pos.y - last_tick_player_pos.y) * renderPartialTicks;
							bb.maxZ += -render_pos.z + -player_pos.z + last_tick_player_pos.z + (player_pos.z - last_tick_player_pos.z) * renderPartialTicks;

							bb.minX -= 0.1f;
							bb.minY	-= 0.1f;
							bb.minZ	-= 0.1f;
							bb.maxX	+= 0.1f;
							bb.maxY	+= 0.1f;
							bb.maxZ	+= 0.1f;

							std::vector<std::vector<double>> boxVertices
							{
								{bb.minX, bb.minY, bb.minZ},
								{bb.minX, bb.maxY, bb.minZ},
								{bb.maxX, bb.maxY, bb.minZ},
								{bb.maxX, bb.minY, bb.minZ},
								{bb.minX, bb.minY, bb.maxZ},
								{bb.minX, bb.maxY, bb.maxZ},
								{bb.maxX, bb.maxY, bb.maxZ},
								{bb.maxX, bb.minY, bb.maxZ},
							};


							try {
								for (int i = 0; i < 8; i++)
								{
									if (glm::vec2 screenPos; utils::WorldToScreen::WorldToScreen(glm::vec3((float)boxVertices[i][0], (float)boxVertices[i][1], (float)boxVertices[i][2]), screenPos, n_glrender::MODELVIEW, n_glrender::PROJECTION, n_glrender::ViewPort))
									{
										w2sbb.x = fmin(screenPos.x, w2sbb.x);
										w2sbb.y = fmin(screenPos.y, w2sbb.y);
										w2sbb.z = fmax(screenPos.x, w2sbb.z);
										w2sbb.w = fmax(screenPos.y, w2sbb.w);
									}

								}
							}
							catch (std::exception exception) { }

							if (w2sbb.x > 0 || w2sbb.y > 0 || w2sbb.z <= n_glrender::ViewPort[2] || w2sbb.w <= n_glrender::ViewPort[3])
							{



								auto boxSize = glm::vec2(fabs(w2sbb.z - w2sbb.x), fabs(w2sbb.w - w2sbb.y));

								glEnable(GL_BLEND);
								if (config::visuals::esp::boxes && config::visuals::esp::filled) {
									glColor4f(0, 0, 0, 0.40);
									gui->drawRect(w2sbb.x + 1, w2sbb.y + 1, w2sbb.z - 1, w2sbb.w - 1, std::stoi(utils::colours::RGBA2MC(22, 22, 22, 20), nullptr, 0));
									//ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(w2sbb.x + 1, w2sbb.y + 1), ImVec2(w2sbb.z - 1, w2sbb.w - 1), ImGui::GetColorU32(ImVec4(22. / 255., 22. / 255., 22. / 255., 20. / 255.)), 0, 0);
								}

								glColor4f(1, 1, 1, 1.f);

								if (config::visuals::esp::boxes)
								{
									if (bool cornered = false; cornered)
									{

										//c_glrender::get().drawCorneredBox(w2sbb.x, w2sbb.y, boxSize.x, boxSize.y, 1.f);

										const auto draw_cornered_esp = [&]()
										{
											float X = w2sbb.x;
											float Y = w2sbb.y;
											float W = w2sbb.z;
											float H = w2sbb.w;

											float lineW = (boxSize.x / 5);
											float lineH = (boxSize.y / 6);
											float lineT = 1;

											//inline

											ImGui::GetCurrentWindow()->DrawList->AddLine( ImVec2( X, Y ), ImVec2( X, Y + lineH ), ImGui::GetColorU32( ImVec4( 0, 0, 0, 1 ) ), 3.f );
											ImGui::GetCurrentWindow()->DrawList->AddLine( ImVec2( X, Y ), ImVec2( X + lineW, Y ), ImGui::GetColorU32( ImVec4( 0, 0, 0, 1 ) ), 3.f );

											ImGui::GetCurrentWindow()->DrawList->AddLine( ImVec2( W, Y ), ImVec2( W, Y + lineH ), ImGui::GetColorU32( ImVec4( 0, 0, 0, 1 ) ), 3.f );
											ImGui::GetCurrentWindow()->DrawList->AddLine( ImVec2( W - lineW, Y ), ImVec2( W, Y ), ImGui::GetColorU32( ImVec4( 0, 0, 0, 1 ) ), 3.f );

											ImGui::GetCurrentWindow()->DrawList->AddLine( ImVec2( X, H - lineH ), ImVec2( X, H ), ImGui::GetColorU32( ImVec4( 0, 0, 0, 1 ) ), 3.f );
											ImGui::GetCurrentWindow()->DrawList->AddLine( ImVec2( X, H ), ImVec2( X + lineW, H ), ImGui::GetColorU32( ImVec4( 0, 0, 0, 1 ) ), 3.f );

											ImGui::GetCurrentWindow()->DrawList->AddLine( ImVec2( W, H - lineH ), ImVec2( W, H ), ImGui::GetColorU32( ImVec4( 0, 0, 0, 1 ) ), 3.f );
											ImGui::GetCurrentWindow()->DrawList->AddLine( ImVec2( W - lineW, H ), ImVec2( W + 1, H ), ImGui::GetColorU32( ImVec4( 0, 0, 0, 1 ) ), 3.f );

											ImGui::GetCurrentWindow()->DrawList->AddLine( ImVec2( X, Y ), ImVec2( X, Y + lineH ), ImGui::GetColorU32( ImVec4( 1, 1, 1, 1 ) ), 1.f );
											ImGui::GetCurrentWindow()->DrawList->AddLine( ImVec2( X, Y ), ImVec2( X + lineW, Y ), ImGui::GetColorU32( ImVec4( 1, 1, 1, 1 ) ), 1.f );

											ImGui::GetCurrentWindow()->DrawList->AddLine( ImVec2( W, Y ), ImVec2( W, Y + lineH ), ImGui::GetColorU32( ImVec4( 1, 1, 1, 1 ) ), 1.f );
											ImGui::GetCurrentWindow()->DrawList->AddLine( ImVec2( W - lineW, Y ), ImVec2( W, Y ), ImGui::GetColorU32( ImVec4( 1, 1, 1, 1 ) ), 1.f );

											ImGui::GetCurrentWindow()->DrawList->AddLine( ImVec2( X, H - lineH ), ImVec2( X, H ), ImGui::GetColorU32( ImVec4( 1, 1, 1, 1 ) ), 1.f );
											ImGui::GetCurrentWindow()->DrawList->AddLine( ImVec2( X, H ), ImVec2( X + lineW, H ), ImGui::GetColorU32( ImVec4( 1, 1, 1, 1 ) ), 1.f );

											ImGui::GetCurrentWindow()->DrawList->AddLine( ImVec2( W, H - lineH ), ImVec2( W, H ), ImGui::GetColorU32( ImVec4( 1, 1, 1, 1 ) ), 1.f );
											ImGui::GetCurrentWindow()->DrawList->AddLine( ImVec2( W - lineW, H ), ImVec2( W + 1, H ), ImGui::GetColorU32( ImVec4( 1, 1, 1, 1 ) ), 1.f );


											//c_glrender::get().line(3, X, Y, X, Y + lineH, 0, 0, 0, 255);
											//c_glrender::get().line(3, X, Y, X + lineW, Y, 0, 0, 0, 255);

											//c_glrender::get().line(3, W, Y, W, Y + lineH, 0, 0, 0, 255);
											//c_glrender::get().line(3, W - lineW, Y, W, Y, 0, 0, 0, 255);

											//c_glrender::get().line(3, X, H - lineH - 1, X, H, 0, 0, 0, 255);
											//c_glrender::get().line(3, X, H, X + lineW, H, 0, 0, 0, 255);

											//c_glrender::get().line(3, W, H - lineH, W, H, 0, 0, 0, 255);
											//c_glrender::get().line(3, W - lineW, H, W + 1, H, 0, 0, 0, 255);


											//c_glrender::get().line(1, X, Y, X, Y + lineH, 255, 255, 255, 255);
											//c_glrender::get().line(1, X, Y, X + lineW, Y, 255, 255, 255, 255);

											//c_glrender::get().line(1, W, Y, W, Y + lineH, 255, 255, 255, 255);
											//c_glrender::get().line(1, W - lineW, Y, W, Y, 255, 255, 255, 255);

											//c_glrender::get().line(1, X, H - lineH, X, H, 255, 255, 255, 255);
											//c_glrender::get().line(1, X, H, X + lineW, H, 255, 255, 255, 255);

											//c_glrender::get().line(1, W, H - lineH, W, H, 255, 255, 255, 255);
											//c_glrender::get().line(1, W - lineW, H, W, H, 255, 255, 255, 255);




											//ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X - lineT, Y - lineT), ImVec2(X + lineW, Y - lineT), IM_COL32_BLACK); //top left
											//ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X - lineT, Y - lineT), ImVec2(X - lineT, Y + lineH), IM_COL32_BLACK);
											//ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X - lineT, Y + H - lineH), ImVec2(X - lineT, Y + H + lineT), IM_COL32_BLACK); //bot left
											//ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X - lineT, Y + H + lineT), ImVec2(X + lineW, Y + H + lineT), IM_COL32_BLACK);
											//ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W - lineW, Y - lineT), ImVec2(X + W + lineT, Y - lineT), IM_COL32_BLACK); // top right
											//ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W + lineT, Y - lineT), ImVec2(X + W + lineT, Y + lineH), IM_COL32_BLACK);
											//ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W + lineT, Y + H - lineH), ImVec2(X + W + lineT, Y + H + lineT), IM_COL32_BLACK); // bot right
											//ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W - lineW, Y + H + lineT), ImVec2(X + W + lineT, Y + H + lineT), IM_COL32_BLACK);
										};
										draw_cornered_esp();

									}
									else {

										// imgui drawing works fine but the quality is kinda shit i dont like it

										ImGui::GetCurrentWindow()->DrawList->AddRect( ImVec2( w2sbb.x, w2sbb.y ), ImVec2( w2sbb.z, w2sbb.w ), ImGui::GetColorU32( ImVec4( 0, 0, 0, 1 ) ), 0, 0, 3.f );
										ImGui::GetCurrentWindow()->DrawList->AddRect( ImVec2( w2sbb.x, w2sbb.y ), ImVec2( w2sbb.z, w2sbb.w ), ImGui::GetColorU32( ImVec4( 1, 1, 1, 1 ) ), 0, 0, 1.f );

										// Opengl does not work with fast render on but looks better imo
										
										//glColor4f(0, 0, 0, 1);

										//glLineWidth( 3.f );

										//glBegin(GL_LINE_LOOP);
										//glLineWidth( 3.f );
										//glVertex2f(w2sbb.x, w2sbb.y);
										//glLineWidth( 3.f );
										//glVertex2f(w2sbb.x, w2sbb.w);
										//glLineWidth( 3.f );
										//glVertex2f(w2sbb.z, w2sbb.w);
										//glLineWidth( 3.f );
										//glVertex2f(w2sbb.z, w2sbb.y);
										//glEnd();

										//glColor4f(1, 1, 1, 1);

										//glLineWidth( 1.f );

										//glBegin( GL_LINE_LOOP );
										//glVertex2f( w2sbb.x, w2sbb.y );
										//glVertex2f( w2sbb.x, w2sbb.w );
										//glVertex2f( w2sbb.z, w2sbb.w );
										//glVertex2f( w2sbb.z, w2sbb.y );
										//glEnd( );

									}
								}// Draw boxes




								char health_txt[10];
								sprintf(health_txt, "%.0f", player->get_health());

								const float maxHealth = player->get_max_health();
								const float health = player->get_health();


								std::string player_name = player->get_name();

								if (config::visuals::esp::hp_bar) {
									// HP bar
									if (bool hp = true; hp) {
										c_glrender::get().line(2.f * fmin(1.f, 5. / dis), w2sbb.x - 4.0f, static_cast<GLfloat>(static_cast<int>(w2sbb.y - 1.0f)), w2sbb.x - 4.0f, static_cast<GLfloat>(static_cast<int>(w2sbb.w + 1.0f)), 0, 0, 0, 255);
										c_glrender::get().line(2.f * fmin(1.f, 5.f / dis), w2sbb.x - 4.0f, static_cast<GLfloat>(static_cast<int>(w2sbb.w + 1)), w2sbb.x - 4.0f, static_cast<GLfloat>(static_cast<int>(w2sbb.w - (boxSize.y / maxHealth * health))) - 1.f, (maxHealth - health) / maxHealth * 255, health / maxHealth * 255, 0, 255);
										//c_glrender::get().line(2.f, w2sbb.x - 5.0f, static_cast<GLfloat>(static_cast<int>(w2sbb.y - 2.0f)), w2sbb.x - 5.0f, static_cast<GLfloat>(static_cast<int>(w2sbb.w + 2.0f)), 0, 0, 0, 255
									}


									// retarded servers that go above 100 hp..
									//int hp = std::min(20.f, health);

									// calculate hp bar color.
									//int r = std::min((510 * (20 - hp)) / 20, 255);
									//int g = std::min((510 * hp) / 20, 255);

									// outline
									//c_glrender::get().line(2.5, w2sbb.x - 5.0f, w2sbb.y - 2.0f, w2sbb.x, w2sbb.w + 2.0f, 0, 0, 0, 255);


									// inside
									//c_glrender::get().line(2.5, w2sbb.x - 4.0f, w2sbb.y - 1.0f, w2sbb.x - 4.0f, w2sbb.w + 1.0f, 0, 0, 0, 70);

									// hp fill
									//c_glrender::get().line(3, w2sbb.x - 4.0f, w2sbb.y - 1.0f, w2sbb.x - 4.0f, w2sbb.w + 1.0f, r, g, 0, 255); // coloured
									//c_glrender::get().line(2.5, w2sbb.x - 4.0f, w2sbb.y - 1.0f, w2sbb.x - 4.0f, w2sbb.w + 1.0f, 243, 243, 243, 255);



									//c_glrender::get().line(3, w2sbb.x - 4.0f, w2sbb.y - 1.0f, w2sbb.x - 4.0f, w2sbb.w + 1.0f, 0, 0, 0, 255);

								}

								//	gui->drawRect(w2sbb.x - 4.0f, w2sbb.y - 1.0f, w2sbb.x - 2.f, w2sbb.w + 1.f, ImColor(243, 243, 243, 255));
								wglMakeCurrent(hooks::gl_context.m_hdc_devicectx, hooks::gl_context.m_glrenderctx);

								//if (config::visuals::esp::hp_bar) {
								//	// HP bar
								//	//if (bool hp = true; hp) {
								//	//	c_glrender::get().line(2.f * fmin(1.f, 5. / dis), w2sbb.x - 4.0f, static_cast<GLfloat>(static_cast<int>(w2sbb.y - 1.0f)), w2sbb.x - 4.0f, static_cast<GLfloat>(static_cast<int>(w2sbb.w + 1.0f)), 0, 0, 0, 255);
								//	//	c_glrender::get().line(2.f * fmin(1.f, 5.f / dis), w2sbb.x - 4.0f, static_cast<GLfloat>(static_cast<int>(w2sbb.w + 1)), w2sbb.x - 4.0f, it_map->second, (maxHealth - health) / maxHealth * 255, health / maxHealth * 255, 0, 255);
								//	//	//c_glrender::get().line(2.f, w2sbb.x - 5.0f, static_cast<GLfloat>(static_cast<int>(w2sbb.y - 2.0f)), w2sbb.x - 5.0f, static_cast<GLfloat>(static_cast<int>(w2sbb.w + 2.0f)), 0, 0, 0, 255
								//	//}


								//	// retarded servers that go above 100 hp..
								//	int hp = std::min(20.f, health);

								//	// calculate hp bar color.
								//	int r = std::min((510 * (20 - hp)) / 20, 255);
								//	int g = std::min((510 * hp) / 20, 255);

								//	//c_glrender::get().line(2.5, w2sbb.x - 5.0f, w2sbb.y - 2.0f, w2sbb.x, w2sbb.w + 2.0f, 0, 0, 0, 255);

								//	// inside
								//	//ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(w2sbb.x - 4.0f, w2sbb.y - 1.0f), ImVec2(w2sbb.x - 2.f, w2sbb.w + 1.f), ImColor(0, 0, 0, 70));

								//	// hp fill
								//	//ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(w2sbb.x - 4.0f, w2sbb.y - 1.0f), ImVec2(w2sbb.x - 2.f, (float)((static_cast<int>(w2sbb.y + (boxSize.y / maxHealth * health))) + 1.f)), ImColor(243, 243, 243, 255));
								//	//
								//	//
								//	//// outline
								//	//ImGui::GetForegroundDrawList()->AddRect(ImVec2(w2sbb.x - 5.0f, w2sbb.y - 2.0f), ImVec2(w2sbb.x - 1.f, w2sbb.w + 2.f), ImColor(0, 0, 0, 255));
								//}


								////HP text
								if (config::visuals::esp::hp_bar) {

									if (dis < 10 && health != 20.f)
										drawTextOutlined(10 / (dis * 3) + 10, ImVec2(w2sbb.x - 6 * strlen(health_txt), w2sbb.w - (boxSize.y / maxHealth * health)), health_txt);
								}

								////Player
								if (config::visuals::esp::names) {
									drawTextOutlined(18.0f * fmax(0.6f, 1.5f / dis), ImVec2((w2sbb.x + (boxSize.x / 2.00f)), w2sbb.y - (2 / 2.00f) - 15.f), player_name, true);
								}

								// among us
								//ImGui::GetBackgroundDrawList()->AddImage((void*)amogus_texture, ImVec2(w2sbb.x, w2sbb.y), ImVec2(w2sbb.z, w2sbb.w));
								
								//ImGui::GetBackgroundDrawList()->AddImage((void*)amogus_texture, ImVec2(w2sbb.x, w2sbb.y), ImVec2(w2sbb.z, w2sbb.w));
								ImGui::GetBackgroundDrawList()->AddImage((void*)amogus_texture, ImVec2(w2sbb.x + 1, w2sbb.y + 1), ImVec2(w2sbb.z - 1, w2sbb.w - 1));

								//Block Distance
								if( static bool draw_distance = true; draw_distance )
								{
									char text[50] = "";
									sprintf(text,"[%.1f Blocks]", dis);

									drawTextOutlined(14.0f * fmax(0.8f, 2.f / dis), ImVec2((w2sbb.x + (boxSize.x / 2.00f)), w2sbb.w + 5.f), text, true);
								}

								// decent
								//	drawTextOutlined(15.0f * fmax(0.6f, 2.f / dis), ImVec2((w2sbb.x + (boxSize.x / 2.00f)) , w2sbb.y - (2 / 2.00f) - 15.f), player->get_name(), true);
								wglMakeCurrent(hooks::gl_context.m_hdc_devicectx, hooks::gl_context.m_oglrenderctx);

							}

						}


					}
					catch (std::exception err) { std::cout << "fix: " << err.what() << std::endl; } // remove this later
				}


				if ((config::visuals::esp::esp_mode == 1 && playeresp::m_enabled) || playeresp::skeleton)
				{
					if (!n_glrender::PROJECTION.empty() && !n_glrender::MODELVIEW.empty()) {
						glPushMatrix();
						glMatrixMode(GL_PROJECTION);
						glLoadMatrixf(n_glrender::PROJECTION.data());
						glMatrixMode(GL_MODELVIEW);
						glLoadMatrixf(n_glrender::MODELVIEW.data());

						glPushMatrix();
						glEnable(GL_LINE_SMOOTH);
						glDisable(GL_DEPTH_TEST);
						glDisable(GL_TEXTURE_2D);
						glDepthMask(false);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						glEnable(GL_BLEND);
						glLineWidth(1.f);

						auto render_global = std::make_shared<c_renderglobal>(minecraft->get_renderglobal());
						for (const auto& player : world->get_players())
						{
							if (player == nullptr)
								continue;

							if (player->get_object() == nullptr)
								continue;

							if (env->IsSameObject(local_player->get_object(), player->get_object()))
								continue;

							if (!player->isAlive())
								continue;

							// anti bot
							
							//std::string name = player->get_name();
							//if (player->is_invisible() && allowed_chars.find(name[0]) == std::string::npos)
							//	continue;

							float dis = local_player->get_distance_to(player);


							auto player_pos = player->get_position();
							auto last_tick_player_pos = player->get_last_tick_pos();

							auto axisAlignedBB = std::make_shared<c_axisalignedbb>(world->new_bounding(player->get_native_non_modified_boundingbox()), env);
							if (axisAlignedBB->get_object() == nullptr)
								continue;

							if (config::visuals::esp::esp_mode == 1 && playeresp::m_enabled)
							{
								axisAlignedBB = axisAlignedBB->offset(
									-render_pos.x + -player_pos.x + last_tick_player_pos.x + (player_pos.x - last_tick_player_pos.x) * renderPartialTicks,
									-render_pos.y + -player_pos.y + last_tick_player_pos.y + (player_pos.y - last_tick_player_pos.y) * renderPartialTicks,
									-render_pos.z + -player_pos.z + last_tick_player_pos.z + (player_pos.z - last_tick_player_pos.z) * renderPartialTicks
								);
								// - (player->is_sneaking() ? 0.4f : 0.f), idk its up to u if u want to have this
								if (config::visuals::esp::expand)
									axisAlignedBB = axisAlignedBB->expand(0.2, 0.2, 0.2, 0.2, 0.2, 0.2);

								//ImVec4(0, 1.f, 0, 0.1f);
								ImVec4 glCol = ImVec4(config::visuals::esp::fill_color[0], config::visuals::esp::fill_color[1], config::visuals::esp::fill_color[2], config::visuals::esp::fill_color[3]);

								// color based on hurttime 
								
								//if (auto hurt_time = player->get_hurt_time(); hurt_time > 0)
								//	glCol = ImVec4(1, 0.f, 0.f, 0.1f);

								glColor4f(glCol.x, glCol.y, glCol.z, glCol.w);

								if (axisAlignedBB->get_object() != nullptr) {

									if (config::visuals::esp::filled)
										c_glrender::get().drawFilledBoundingBox(axisAlignedBB->get_native_boundingbox());

									if (config::visuals::esp::outline) {
										glLineWidth(config::visuals::esp::outline_width);
										render_global->drawSelectionBoundingBox(axisAlignedBB->get_object(), glCol.x * 55.f, glCol.y * 255.f, glCol.z * 255.f, 200);
										glLineWidth(1.f);
									}
								}
							}


							//glLineWidth(1.5f);
							//glColor4f(1, 1, 1, 0.5f);
							//auto pos = glm::vec3(player_pos.x, player_pos.y + 1.6f, player_pos.z) - render_pos;
							//{
							//	glBegin(GL_LINES);
							//	{
							//		glVertex3d(0.0, 0.0 + local_player->get_eye_height(), 0.0);
							//		glVertex3d(pos.x, pos.y, pos.z);
							//	}
							//	glEnd();
							//}

							// Nametags
							//	{
							//		float d = last_tick_player_pos.x + (player_pos.x - last_tick_player_pos.x) * renderPartialTicks - render_pos.x;
							//		float d2 = last_tick_player_pos.y + (player_pos.y - last_tick_player_pos.y) * renderPartialTicks - render_pos.y;
							//		float d3 = last_tick_player_pos.z + (player_pos.z - last_tick_player_pos.z) * renderPartialTicks - render_pos.z;


							//		float f = dis / 4.0F;
							//		if (f < 1.6F) {
							//			f = 1.6F;

							//		}
							//		float f2 = f / 30.0F;
							//		f2 = (float)((double)f2 * 0.3f);

							//		glPushMatrix();
							//		glEnable(GL_TEXTURE_2D);
							//		glTranslatef(d, d2 + 1.8f, d3);
							//		glNormal3f(0.0f, 1.0f, 0.0f);

							//		glRotatef(-n_glrender::view_pos.y, 0.0F, 1.0f, 0.0f);
							//		glRotatef(n_glrender::view_pos.x, 1.0f, 0.0f, 0.0f);
							//		glScalef(-f2, -f2, f2);


							//		std::string text = player->get_name();
							//		float string_width = font_renderer->get_string_width(text) / 2;

							//		gui->drawRect(-font_renderer->get_string_width(text) / 2 - 2, -13, string_width + 2, -1, INT_MIN);
							//		font_renderer->drawString(text.c_str(), -string_width, -11, 0xffffff, true);

							//		glLineWidth(1.0F);

							//		// armor
							//		{
							//			auto held_item = player->get_current_equiped_item();
							//			if(held_item != nullptr)
							//				ItemRenderer->renderItemAndEffectIntoGUI(held_item, -16, 0);
							//		}

							//		glDisable(GL_TEXTURE_2D);
							//		glNormal3f(1.0f, 1.0f, 1.0f);
							//		glPopMatrix();


							//	//glLineWidth(2.5f);
							//	//glColor4f(1, 1, 1, 0.5f);
							//	//auto pos = glm::vec3(player_pos.x, player_pos.y + 1.6f, player_pos.z) - render_pos;
							//	//{
							//	//	glBegin(GL_LINES);
							//	//	{
							//	//		glVertex3d(0.0, 0.0 + local_player->get_eye_height(), 0.0);
							//	//		glVertex3d(pos.x, pos.y, pos.z);
							//	//	}
							//	//	glEnd();
							//	//}

							//	//glLineWidth(1.f);
							//}

							// draw tracers


							// glDisable(GL_TEXTURE_2D);
							// glDisable(GL_DEPTH_TEST);
							// glDepthMask(false);


						}

						if (playeresp::skeleton)
						{
							for (const auto& player : world->get_players())
							{
								if (player == nullptr)
									continue;

								if (player->get_object() == nullptr)
									continue;

								if (env->IsSameObject(local_player->get_object(), player->get_object()))
									continue;

								if (!player->isAlive())
									continue;


								auto player_pos = player->get_position();
								auto last_tick_player_pos = player->get_last_tick_pos();

								float dis = local_player->get_distance_to(player);

								jobject renderPlayer = rendermanager->getEntityRenderObject(player);

								if (renderPlayer == nullptr)
									continue;
								auto render_player = std::make_shared<c_player>(renderPlayer, env);

								std::vector<bone_t> player_bones = render_player->get_main_model()->get_bones();

								glm::vec3 vec(last_tick_player_pos.x + (player_pos.x - last_tick_player_pos.x) * renderPartialTicks - render_pos.x, last_tick_player_pos.y + (player_pos.y - last_tick_player_pos.y) * renderPartialTicks - render_pos.y, last_tick_player_pos.z + (player_pos.z - last_tick_player_pos.z) * renderPartialTicks - render_pos.z);

								glPushMatrix();
								glEnable(2848);

								{	// smooth
									glEnable(GL_LINE_SMOOTH);
									glEnable(GL_POLYGON_SMOOTH);
									glEnable(GL_POINT_SMOOTH);
									glEnable(GL_BLEND);
									glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
									glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
									glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
									glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
								}

								glLineWidth(fmin(3, 2 / dis + 1));
								glColor4f(0.f, 191.f / 255.f, 1.f, 1.f);

								glTranslated(vec.x, vec.y, vec.z);

								float prevRenderYawOffset = player->getprevRenderYawOffset();
								float renderYawOffset = player->getrenderYawOffset();
								float xOff = prevRenderYawOffset + (renderYawOffset - prevRenderYawOffset) * renderPartialTicks;
								bool is_sneaking = player->is_sneaking();

								glRotatef(-xOff, 0.0f, 1.0f, 0.0f);
								glTranslatef(0.0f, 0.0f, is_sneaking ? -0.235f : 0.0f);

								float legHeight = is_sneaking ? 0.6F : 0.75F;

								glPushMatrix();
								glTranslated(-0.125, legHeight, 0.0);

								if (player_bones[3].x != 0.0F)
									glRotatef(player_bones[3].x * 57.29578F, 1.0F, 0.0F, 0.0F);
								if (player_bones[3].y != 0.0F)
									glRotatef(player_bones[3].y * 57.29578F, 0.0F, 1.0F, 0.0F);
								if (player_bones[3].z != 0.0F)
									glRotatef(player_bones[3].z * 57.29578F, 0.0F, 0.0F, 1.0F);

								glBegin(3);
								glVertex3d(0.0, 0.0, 0.0);
								glVertex3d(0.0, -legHeight, 0.0);
								glEnd();
								glPopMatrix();

								glPushMatrix();
								glTranslated(0.125, legHeight, 0.0);
								if (player_bones[4].x != 0.0F)
									glRotatef(player_bones[4].x * 57.29578F, 1.0F, 0.0F, 0.0F);
								if (player_bones[4].y != 0.0F)
									glRotatef(player_bones[4].y * 57.29578F, 0.0F, 1.0F, 0.0F);
								if (player_bones[4].z != 0.0F)
									glRotatef(player_bones[4].z * 57.29578F, 0.0F, 0.0F, 1.0F);
								glBegin(3);
								glVertex3d(0.0, 0.0, 0.0);
								glVertex3d(0.0, -legHeight, 0.0);
								glEnd();
								glPopMatrix();
								glTranslated(0.0, 0.0, is_sneaking ? 0.25 : 0.0);
								glPushMatrix();
								glTranslated(0.0, is_sneaking ? -0.05 : 0.0, is_sneaking ? -0.01725 : 0.0);
								glPushMatrix();
								glTranslated(-0.375, legHeight + 0.55, 0.0);
								if (player_bones[1].x != 0.0F)
									glRotatef(player_bones[1].x * 57.29578F, 1.0F, 0.0F, 0.0F);
								if (player_bones[1].y != 0.0F)
									glRotatef(player_bones[1].y * 57.29578F, 0.0F, 1.0F, 0.0F);
								if (player_bones[1].z != 0.0F)
									glRotatef(-player_bones[1].z * 57.29578F, 0.0F, 0.0F, 1.0F);
								glBegin(3);
								glVertex3d(0.0, 0.0, 0.0);
								glVertex3d(0.0, -0.5, 0.0);
								glEnd();
								glPopMatrix();
								glPushMatrix();
								glTranslated(0.375, legHeight + 0.55, 0.0);
								if (player_bones[2].x != 0.0F)
									glRotatef(player_bones[2].x * 57.29578F, 1.0F, 0.0F, 0.0F);
								if (player_bones[2].y != 0.0F)
									glRotatef(player_bones[2].y * 57.29578F, 0.0F, 1.0F, 0.0F);
								if (player_bones[2].z != 0.0F)
									glRotatef(-player_bones[2].z * 57.29578F, 0.0F, 0.0F, 1.0F);
								glBegin(3);
								glVertex3d(0.0, 0.0, 0.0);
								glVertex3d(0.0, -0.5, 0.0);
								glEnd();
								glPopMatrix();

								glPushMatrix();
								glTranslated(0.375f, legHeight + 0.55, 0.0);
								if (player_bones[2].x != 0.0F) {
									glRotatef(player_bones[2].x * 57.29578F, 1.0F, 0.0F, 0.0F);
								}

								if (player_bones[2].y != 0.0F) {
									glRotatef(player_bones[2].y * 57.29578F, 0.0F, 1.0F, 0.0F);
								}

								if (player_bones[2].z != 0.0F) {
									glRotatef(-player_bones[2].z * 57.29578F, 0.0F, 0.0F, 1.0F);
								}

								glBegin(3);
								glVertex3d(0.0, 0.0, 0.0);
								glVertex3d(0.0, -0.5, 0.0);
								glEnd();
								glPopMatrix();
								glRotatef(xOff - player->get_yaw2(), 0.0F, 1.0F, 0.0F);
								glPushMatrix();
								glTranslated(0.0, legHeight + 0.55, 0.0);
								if (player_bones[0].x != 0.0F) {
									glRotatef(player_bones[0].x * 57.29578F, 1.0F, 0.0F, 0.0F);
								}

								glBegin(3);
								glVertex3d(0.0f, 0.0f, 0.0f);
								glVertex3d(0.f, 0.3f, 0.f);
								glEnd();
								glPopMatrix();
								glPopMatrix();
								glRotatef(is_sneaking ? 25.0F : 0.0F, 1.0F, 0.0F, 0.0F);
								glTranslated(0.f, is_sneaking ? -0.16175 : 0.f, is_sneaking ? -0.48025 : 0.f);
								glPushMatrix();
								glTranslated(0.f, legHeight, 0.f);
								glBegin(3);
								glVertex3d(-0.125, 0.f, 0.f);
								glVertex3d(0.125, 0.f, 0.f);
								glEnd();
								glPopMatrix();
								glPushMatrix();
								glTranslated(0.f, legHeight, 0.f);
								glBegin(3);
								glVertex3d(0.f, 0.f, 0.f);
								glVertex3d(0.f, 0.55, 0.f);
								glEnd();
								glPopMatrix();
								glPushMatrix();
								glTranslated(0.f, legHeight + 0.55, 0.f);
								glBegin(3);
								glVertex3d(-0.375, 0.f, 0.f);
								glVertex3d(0.375, 0.f, 0.f);
								glEnd();
								glPopMatrix();

								{   // End smooth
									glDisable(GL_LINE_SMOOTH);
									glDisable(GL_POLYGON_SMOOTH);
									glEnable(GL_POINT_SMOOTH);
								}
								glLineWidth(1.f);

								glPopMatrix();

							}
						}

						// Search

						{
							for (const auto& block_data : n_glrender::block_positions)
							{
								auto block = std::get<1>(block_data);
								int block_id = std::get<0>(block_data);

								ImVec4 color(0, 0.7, 0.75, 1.f);
								float fill_alpha = 0.3f;

								/*
								56, // diamond
								21, // lapis
								16, // coal
								73, // redstone
								129, // emerald
								14, // gold
								15, // iron
								54, // chest
								130 // enderchest
								*/


								if( block_id == 56 ) // diamond
									color = ImVec4( 0.19, 0.80, 0.82, 1.f );
								else if( block_id == 21 )
									color = ImVec4( 0.09, 0.18, 0.67, 1.f );
								else if( block_id == 16 )
									color = ImVec4( 0.26, 0.26, 0.26, 1.f );
								else if( block_id == 73 )
									color = ImVec4( 0.95, 0.11, 0.14, 1.f );
								else if( block_id == 129 )
									color = ImVec4( 0.16, 0.77, 0.32, 1.f );
								else if( block_id == 14 )
									color = ImVec4( 0.97, 0.93, 0.37, 1.f );
								else if( block_id == 15 )
									color = ImVec4( 0.54, 0.44, 0.37, 1.f );
								else if( block_id == 54 )
									color = ImVec4( 0.56, 0.42, 0.15, 1.f );
								else if( block_id == 130 )
									color = ImVec4( 0.07, 0.11, 0.11, 1.f );


								glLineWidth(1.f);
								auto axis = std::make_shared<c_axisalignedbb>(init_axisalignedbb(env, block.minX, block.minY, block.minZ, block.maxX, block.maxY, block.maxZ), env);
								axis = axis->offset(-render_pos.x, -render_pos.y, -render_pos.z);

								glColor4f(color.x, color.y, color.z, fill_alpha);
								c_glrender::get().drawFilledBoundingBox(axis->get_native_boundingbox());

								glColor4f(color.x, color.y, color.z, color.w);
								render_global->drawSelectionBoundingBox(axis->get_object(), color.x * 255, color.y * 255, color.z * 255, color.w * 255);
							}
						}


						glDisable(GL_BLEND);
						glDepthMask(true);
						glEnable(GL_TEXTURE_2D);
						glEnable(GL_DEPTH_TEST);
						glDisable(GL_LINE_SMOOTH);
						glPopMatrix();

						glPopMatrix();

					}
				}
			}


		}

		ImGui::End( );
		ImGui::PopStyleColor( );
		ImGui::PopStyleVar( 2 );

		c_glrender::get().setup_ortho();

		glPushMatrix();
		glScalef(2.f, 2.f, 2.f);

		std::string* eternal_beta = new std::string(xorstr_("Eternal BETA"));
		font_renderer->drawString(eternal_beta->c_str(), 3, 3, 0xFFFFFF, true); //0x262424
		delete eternal_beta;

		glPopMatrix();

		offset = math::interpolate(offset, 0, 0.0000060);
		int y = 3 - offset; // 2


		if (arraylist::m_enabled)
		{
			bool fade_out = false;
			glPushMatrix();

			glScalef(ScaledResolution->get_scaled_resolution(), ScaledResolution->get_scaled_resolution(), ScaledResolution->get_scaled_resolution()); 

			sort(ModuleList.begin(), ModuleList.end(), [font_renderer](const std::tuple<std::string, bool*>& a,
				const std::tuple<std::string, bool*>& b) -> bool
				{
					int one = font_renderer->get_string_width(std::get<0>(a));
					int two = font_renderer->get_string_width(std::get<0>(b));
					return two < one;
				});

			static std::map<std::string, float> animationY;
			static std::map<std::string, float> animationX;
			static std::map<std::string, float> animationA;
			int v = 0;


			for (int i = 0; i < ModuleList.size(); ++i)
			{
				std::tuple Module = ModuleList[i];
				std::string ModuleName = std::get<0>(Module);
				std::string* ModuleName2 = new std::string(ModuleName);

				if (*std::get<1>(Module) == false)
				{
					bool should_skip = false;

					auto it_anim2 = animationY.find(ModuleName2->c_str());

					if (it_anim2 != animationY.end())
					{
						if (it_anim2->second != -500)
						{
							it_anim2->second = ImLerp(it_anim2->second, -500.f, 0.0060f * 3.f);
						}
						else {
							should_skip = true;
						}
					}

					auto it_anim3 = animationX.find(ModuleName2->c_str());
					if (it_anim3 != animationX.end())
					{
						float target = (float)((ModuleName.length() * 12) + 24) * -1;
						if (it_anim3->second != target)
						{
							it_anim3->second = ImLerp(it_anim3->second, target, 0.0060f * 3.f);
						}
						else {
							should_skip = true;
						}
					}

					//auto it_anim4 = animationA.find(ModuleName);
					//if (it_anim4 != animationA.end())
					//{
					//	if (it_anim4->second != 1)
					//	{
					//		fade_out = true;
					//		it_anim4->second = ImLerp(it_anim4->second, 1.f, 0.0060f);
					//	}
					//	else {
					//		should_skip = true;
					//	}
					//}

					if (should_skip)
						continue;
				}

				auto it_anim = animationY.find(ModuleName2->c_str());
				if (it_anim == animationY.end())
				{
					animationY.insert({ ModuleName2->c_str(), 0});
					it_anim = animationY.find(ModuleName2->c_str());
				}

				auto it_animX = animationX.find(ModuleName2->c_str());
				if (it_animX == animationX.end())
				{
					animationX.insert({ ModuleName2->c_str(), -400.f  - (v * 40)}); // -400
					it_animX = animationX.find(ModuleName2->c_str());
				}

				auto it_animA = animationA.find(ModuleName2->c_str());
				if (it_animA == animationA.end())
				{
					animationA.insert({ ModuleName2->c_str(), 1});
					it_animA = animationA.find(ModuleName2->c_str());
				}

				float target_val = (v * 10) + 2 - offset;
				it_anim->second = ImLerp(it_anim->second, target_val, 0.0075);


				float time = 0.0075 * 3;
				it_animX->second = ImLerp(it_animX->second, 2.f, time);
				if (_fade_)
				{
					if (!fade_out)
					{
						if (it_animA->second != 255)
							it_animA->second++;

					}

				}


				// Background
				gui->drawRect(ScaledResolution->get_scaled_width() - font_renderer->get_string_width(ModuleName) - it_animX->second - 2, target_val - 1, ScaledResolution->get_scaled_width(), target_val + 10 - 1, 0x4c000000);

				////Bar
				//gui->drawRect(ScaledResolution->get_scaled_width() - 2.f, target_val - 1, ScaledResolution->get_scaled_width() + 1, target_val + 10 , 0x3FA4C6FF);

				if (_fade_)
				{
					if (vertical)
						font_renderer->drawString(ModuleName2->c_str(), ((ScaledResolution->get_scaled_width()) - font_renderer->get_string_width(ModuleName) - 2) + offset, it_anim->second, std::stoi(fade(i * 150), nullptr, 0), true); // 0xff1C4BE0 //std::stoi(rainbow(i * 100), nullptr,  0)
					else
					{
						float target = (float)((ModuleName.length() * 12) + 24) * -1;

						//gui->drawRect(ScaledResolution->get_scaled_width() - font_renderer->get_string_width(ModuleName) - it_animX->second -2, target_val - 1, ScaledResolution->get_scaled_width(), target_val + y - 1, 0xFFFFFF);

						if (it_animX->second != target || (fade_out && it_animA->second != 1))
							font_renderer->drawString(ModuleName2->c_str(), ((ScaledResolution->get_scaled_width()) - font_renderer->get_string_width(ModuleName) - it_animX->second), target_val, std::stoi(fade(i * 150), nullptr, 0), true);
					}
				}
				else {

					if (vertical)
						font_renderer->drawString(ModuleName2->c_str(), ((ScaledResolution->get_scaled_width()) - font_renderer->get_string_width(ModuleName) - 2) + offset, it_anim->second, 0xFF5733, true); // 0xff1C4BE0 //std::stoi(rainbow(i * 100), nullptr,  0)
					else
					{
						float target = (float)((ModuleName.length() * 12) + 24) * -1;

						if (it_animX->second != target) {
							float xPos = ((ScaledResolution->get_scaled_width()) - font_renderer->get_string_width(ModuleName) - it_animX->second);

							font_renderer->drawString(ModuleName2->c_str(), xPos, target_val, std::stoi(rainbow(i * 100), nullptr, 0), true);
						}
					}

				}

				y += 10;

				if (*std::get<1>(Module))
					++v;
				//std::shared_ptr<std::string> my_string = std::make_shared<std::string>(std::string("My string"));

				ModuleName = "";
				delete ModuleName2;
			}


			//gui->drawRect(ScaledResolution->get_scaled_width() - 2.f, 2 - offset, ScaledResolution->get_scaled_width() + 1, y + 1.f, 0x3FA4C6FF);
			glPopMatrix();

		}
		c_glrender::get().restore_gl();

	}  
}

static std::once_flag once_lol;


void arraylist::draw(c_context* ctx)
{
	std::call_once(once_lol, [&]() {
		hooks::onRender(&arraylist::onRender);
		initlist();
		});

	if (!arraylist::m_enabled)
		return;
}




/// nametags
/// 


//for (const auto& entity : world->get_players()) {
//
//	vec3 pos = entity->get_position();
//	float pT = timer_t->get_renderPartialTicks();
//
//	glPushMatrix();
//	glEnable(GL_BLEND);
//	glDisable(GL_DEPTH_TEST);
//	// Disable Lightning
//	// Enable Blend
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	double x = entity->get_lastTickPosX() + (pos.x - entity->get_lastTickPosX()) * pT - rendermanager->Get_renderPosX();
//	double y = entity->get_lastTickPosY() + (pos.y - entity->get_lastTickPosY()) * pT - rendermanager->Get_renderPosY() + 1.2f;
//	double z = entity->get_lastTickPosZ() + (pos.z - entity->get_lastTickPosZ()) * pT - rendermanager->Get_renderPosZ();
//
//	float d = local_player->get_distance_to(entity);
//	float a = std::min(std::max(1.2f * (d * 0.15f), 1.25f), 6.f);
//
//	glTranslatef(x, y + entity->get_height() + 0.5f - (entity->get_height() / 2), z);
//	glNormal3f(0, 1, 0);
//	glRotatef(rendermanager->Get_viewPosY(), 0, 1, 0);
//	glRotatef(rendermanager->Get_viewPosX(), 1, 0, 0);
//
//	glScalef(-a, -a, -a);
//	float string_width = font_renderer->get_string_width(entity->get_name()) / 2;
//
//	glEnable(GL_TEXTURE_2D);
//
//	font_renderer->drawString(entity->get_name(), -string_width, 0, -1, true);
//
//
//	glEnable(GL_DEPTH_TEST);
//	// Disable Blend
//	glDisable(GL_BLEND);
//	glColor4f(1, 1, 1, 1);
//	glNormal3f(1, 1, 1);
//	glPopMatrix();
//}
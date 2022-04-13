#include "main.hpp"
#include "c_glrender.hpp"
#include "c_gltext.hpp"
#include <mutex>
#include <iostream>
#include "functions.hpp"
#include "Player.hpp"
#include "World.hpp"
#include "c_context.hpp"
#include "Modules.hpp"

#include "SwapLayerBuffers.hpp"

#include "Libs/imgui/imgui_internal.h"
#include "Libs/imgui/imgui.h"
#include "Libs/imgui/imgui_impl_win32.h"
#include "Libs/imgui/imgui_impl_opengl2.h"

#include "FontRenderer.hpp"
#include "Font.hpp"
#include "Icons.hpp"
#include "IconHelper.hpp"
#include "Gui.hpp"
#include "config.hpp"
#include "ArrayList.hpp"
#include "Chams.hpp"
#include "c_notification.hpp"
#include "Encryption.h"

#include "utils.hpp"
#include "FakeLag.hpp"
#include "FastPlace.hpp"
#include "Icons.h"
#include "NoFall.hpp"



#define STB_IMAGE_IMPLEMENTATION

ImVec2 Array_Size = ImVec2(75, 150);
ImVec2 Array_Pos = ImVec2(300, 300);
float x = 0.001;
float child_alpha = 0.6f;
bool fade_in = true;
static int menu_movement_x = 0;
static int menu_movement_y = 0;

DWORD st;


GLuint visuals_texture = 0;
GLuint menu_texture = 0;
GLuint setting_texture = 0;
GLuint movement_texture = 0;
GLuint combat_texture = 0;
GLuint user_texture = 0;

bool LoadTexture(unsigned char data[], int len, GLuint* out_texture, int* out_width, int* out_height)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;

	unsigned char* image_data = stbi_load_from_memory(data, len, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}

auto timer_ = []() {
	static double start_time = GetTickCount();
	double current_time = GetTickCount() - start_time;

	if (current_time >= 500) {
		start_time = GetTickCount();
		return true;
	}
	return false;
};

float val = 2.f;
void fade_outline(ImDrawList* drawlist, ImVec2 pos, ImVec2 size, ImVec4 color)
{
	if (!drawlist)
		drawlist = ImGui::GetBackgroundDrawList();

	while (true)
	{
		color.w *= alpha;

		if (color.w <= 0.019f)
			break;

		ImVec2 bet = ImVec2(pos.x + size.x, pos.y + size.y);
		drawlist->AddRect(pos, bet, ImGui::GetColorU32(color));

		color.w -= color.w / 4;
		pos = ImVec2(pos.x - 1.f, pos.y - 1.f); //-= ImVec2(1.f, 1.f);

		//if (val < 10.f)
		//	val += 0.02;
		//else
		//	val -= 0.02;

		size = ImVec2(size.x + val, size.y + val); // += ImVec2(2.f, 2.f);
		
	}
}

float bgAlpha = 1;
type_wglSwapLayerBuffers owglSwapLayerBuffers;
WNDPROC g_oriWndProc = NULL;

void hooks::onRender(void(*func)())
{
	hooks::callbacks.push_back(func);
}

bool update = false;

BOOL hooks::wgl_swap_buffers(HDC hdc, UINT uint) {

	HDC m_curhdc = hdc;
	if (!c_gltext::get().m_fontbuilt || m_curhdc != c_gltext::get().m_fonthdc) {
		c_gltext::get().build_fonts();

		return owglSwapLayerBuffers(hdc, uint);
	}

	if(selfdestruct::should_destruct)
		return owglSwapLayerBuffers(hdc, uint);

	HWND mc_window = FindWindowA("LWJGL", nullptr);

	c_glrender::get().setup_ortho();

	unsigned char color[3]{ 255, 0, 0 };
	unsigned char colors[4]{ 255, 0, 0 };

	//c_glrender::get().drawCrosshair();


	if (c_main::get().b_open)
	{
		if (bgAlpha < 130)
			bgAlpha += 1;

		c_glrender::get().box(800, 300, 2500, 1, 1, 1, (GLubyte)bgAlpha);

	}
	else {
		bgAlpha = 1;
	}

	if (!global::jenv) {

		return owglSwapLayerBuffers(hdc, uint);
	}

	std::call_once(flag, [&]() {
		c_main::get().m_hwnd = mc_window;
		hooks::gl_context = utils::create_gl_context();
		auto context = ImGui::CreateContext();
		ImGui::SetCurrentContext(context);
		
		set_colors();

		//unsigned int flags = ImGuiFreeType::NoHinting;
		//ImGuiFreeType::BuildFontAtlas(ImGui::GetIO().Fonts, flags);
		ImGui_ImplWin32_Init(mc_window);
		ImGui_ImplOpenGL2_Init();

		g_oriWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(mc_window, GWLP_WNDPROC, LONG_PTR(hooks::wndproc)));

		c_glrender::get().restore_gl();

		update = true;


		return owglSwapLayerBuffers(hdc, uint);
	});


	if (mc_window != c_main::get().m_hwnd)
	{
		std::cout << "rebuilding\n";
		ImGui_ImplOpenGL2_Shutdown();
		ImGui_ImplWin32_Shutdown();

		c_main::get().m_hwnd = mc_window;
		hwnd_changed = true;
		
		//g_oriWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(mc_window, GWLP_WNDPROC, LONG_PTR(hooks::wndproc)));
		SetWindowLongPtr(mc_window, GWLP_WNDPROC, LONG_PTR(hooks::wndproc));
		hooks::gl_context = utils::create_gl_context();

		auto context = ImGui::CreateContext();
		ImGui::SetCurrentContext(context);
		set_colors();
		ImGui_ImplWin32_Init(mc_window);
		ImGui_ImplOpenGL2_Init();

		RECT res;
		GetClientRect(mc_window, &res);

		int width = res.right - res.left;
		int height = res.bottom - res.top;
		ImGui::SetNextWindowPos(ImVec2(width / 2 - menuSize[0] / 2, height / 2 - menuSize[1] / 2));
		GUI_ScreenPos = ImVec2(width / 2 - menuSize[0] / 2, height / 2 - menuSize[1] / 2);

		global::window_pos_changed = true;


		if (timer_())
		{
			global::window_pos_changed = false;
		}

		update = true;

	//	wglMakeCurrent(gl_context.m_hdc_devicectx, gl_context.m_oglrenderctx);

		return owglSwapLayerBuffers(hdc, uint);
	}


	wglMakeCurrent(hooks::gl_context.m_hdc_devicectx, hooks::gl_context.m_glrenderctx);

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	auto s = ImVec2{}, p = ImVec2{}, gs = ImVec2{ 680, 440 };

	if(update)
	{
		{
			int my_image_width2 = 0;
			int my_image_height2 = 0;
			LoadTexture(icons::visual, 903, &visuals_texture, &my_image_width2, &my_image_height2);

			int my_image_width = 0;
			int my_image_height = 0;
			LoadTexture(icons::menu, 336, &menu_texture, &my_image_width, &my_image_height);
			
			LoadTexture(icons::settings, 1013, &setting_texture, &my_image_width, &my_image_height);
			LoadTexture(icons::move, 828, &movement_texture, &my_image_width, &my_image_height);
			LoadTexture(icons::combat, 778, &combat_texture, &my_image_width, &my_image_height);
			LoadTexture(icons::user, 696, &user_texture, &my_image_width, &my_image_height);
			
			LoadTexture(icons::amogus, 7568, &amogus_texture, &my_image_width, &my_image_height);
		
		}
		update = false;
	}
	
	//if (hooks::callbacks.size() > 0)
	//{
	//	for (auto&& fn : hooks::callbacks)
	//		fn();
	//}

	wglMakeCurrent(hooks::gl_context.m_hdc_devicectx, hooks::gl_context.m_oglrenderctx);
	
	auto& Style = ImGui::GetStyle();

	Style.AntiAliasedLines = false;
	Style.AntiAliasedFill = false;

	if (hooks::callbacks.size() > 0)
	{
		for (auto&& fn : hooks::callbacks)
			fn();
	}

	wglMakeCurrent(hooks::gl_context.m_hdc_devicectx, hooks::gl_context.m_glrenderctx);

	if (notification::notifications.size() > 0)
	{
		if (auto& _notification_ = notification::notifications[notification::notifications.size() - 1]; !_notification_.closing)
			_notification_.run();
		else
		{
			_notification_.close_withanim();

			if (st == NULL)
				st = GetTickCount();

			if (math::calc_timer(st, 500))
			{
				notification::notifications.clear();
				st = NULL;
			}
		}		
	}

	
	ImGui::GetIO().IniFilename = NULL;

	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = 1.f;

	if (c_main::get().b_open || fade_out)
	{

		ImGuiIO& io = ImGui::GetIO();
		if (fade_out)
			io.MouseDrawCursor = false;
		else
			io.MouseDrawCursor = true;

		if (global::window_pos_changed && !initialized)
		{

			if (timer_())
			{
				GUI_ScreenPos = ImGui::GetWindowPos();
				global::window_pos_changed = false;
			}
		}

		
		if (!initialized )
		{
			initialized = true;
			hwnd_changed = false;

			RECT res;
			GetWindowRect(mc_window, &res);

			int width = res.right - res.left;
			int height = res.bottom - res.top;
			ImGui::SetNextWindowPos(ImVec2(width / 2 - menuSize[0] / 2, height / 2 - menuSize[1] / 2));
			global::window_pos_changed = false;
		}

		if (fade_out)
		{
			if (alpha > 0)
			{
				alpha = math::clamp(alpha + frequency * io.DeltaTime * -1, 0.f, 1.f);
			}
			else {
				c_main::get().b_open = false;
				fade_out = false;
			}
		}

		if (alpha <= 1.f && fade_out == false)
		{
			alpha = math::clamp(alpha + frequency * io.DeltaTime, 0.f, 1.f);//clip(alpha + frequency * io.DeltaTime / 1.5f, 0.f, 1.f);;
		}
		
		//ImGui::SetNextWindowBgAlpha(alpha);
		ImGui::SetNextWindowSize(ImVec2(gs));

		ImGui::StyleColorsDark();
		ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.13, 0.12, 0.13, 1.f);
		//ImGui::GetStyle().Colors[ImGuiCol_ChildBg] = ImVec4(0.1, 0.1, 0.1, 1.f);
		ImGui::GetStyle().AntiAliasedFill = true;
		ImGui::GetStyle().AntiAliasedLines = true;
		
		{
			ImGui::GetStyle().Alpha = alpha;
			ImGui::Begin("##GUI", &c_main::get().b_open, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
			{
				#pragma region fade_animation
								if (fade_in)
								{
									child_alpha = 0.1f;

									fade_in = false;
								}
								else if (!fade_in && child_alpha <= 0.95)
								{
									child_alpha = ImLerp(child_alpha, 1.f, ImGui::GetIO().DeltaTime * 4.f);
								}
								else
									child_alpha = 1.f;

								child_alpha *= alpha;
				#pragma endregion fade_animation


				if (GUI_ScreenPos.x != ImGui::GetWindowPos().x)
								{
									global::window_pos_changed = true;

									if (timer_())
									{
										GUI_ScreenPos = ImGui::GetWindowPos();
										global::window_pos_changed = false;
									}

								}

				ImGui::BeginChild("_top left_", ImVec2(140, 38), false);
				{
					ImGui::SetCursorPos(ImVec2(10, 10));
					ImGui::PushFont(big_font);
					float textSize = ImGui::CalcTextSize(xorstr_("E T E R N A L")).x;
					ImGui::SetCursorPosX((140 / 2 - textSize + 38.5));
					ImGui::Text(xorstr_("E T E R N A L"));
					ImGui::PopFont();

					ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.16, 0.16, 0.16, 1.f));
					ImGui::SetCursorPos(ImVec2(-4, 37));
					ImGui::BeginChild("top left down bar", ImVec2(140, 1), false);
					{

					}
					ImGui::EndChild();

					ImGui::SetCursorPos(ImVec2(139, 0));
					ImGui::BeginChild("top left right bar", ImVec2(1, 38), false);
					{

					}
					ImGui::EndChild();

					ImGui::PopStyleColor();
				}
				ImGui::EndChild();

				{
					//style.Colors[ImGuiCol_WindowBg] = ImVec4(0.07058823529, 0.06666666666, 0.07058823529, 1.f);
					s = ImVec2(ImGui::GetWindowSize().x - ImGui::GetStyle().WindowPadding.x * 2, ImGui::GetWindowSize().y - ImGui::GetStyle().WindowPadding.y * 2); p = ImVec2(ImGui::GetWindowPos().x + ImGui::GetStyle().WindowPadding.x, ImGui::GetWindowPos().y + ImGui::GetStyle().WindowPadding.y); auto draw = ImGui::GetWindowDrawList();

					draw->AddRectFilled(ImVec2(p.x + 140.f, p.y), ImVec2(p.x + s.x, p.y + 38.5f), ImColor(15, 17, 19, (int)(alpha * 255)), 4.f, ImDrawCornerFlags_TopRight);
					draw->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 140.f, p.y + 38.5f), ImColor(15, 17, 19, (int)(alpha * 200)), 4.f, ImDrawCornerFlags_TopLeft);

					draw->AddRectFilled(ImVec2(p.x + 140.f, p.y + s.y / 11.5), ImVec2(p.x + s.x, p.y + s.y), ImGui::GetColorU32(ImVec4(0.13, 0.12, 0.13, alpha)), 5, ImDrawCornerFlags_BotRight);

					draw->AddRectFilled(ImVec2(p.x, p.y + s.y / 11.5), ImVec2(p.x + 140.f, p.y + s.y), ImColor(15, 17, 19, (int)(alpha * 200)), 5, ImDrawCornerFlags_BotLeft);
				}


				ImGui::SetCursorPos(ImVec2(8, 42));
				ImGui::BeginChild("main left###gang", ImVec2(140, s.y), false, ImGuiWindowFlags_NoScrollbar);
				{
					ImGui::SetCursorPos(ImVec2(7, 20));

					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.72, 0.72, 0.73, 1));
					ImGui::PushFont(thin_font);
					ImGui::Text("FEATURES");
					ImGui::PopFont();
					ImGui::PopStyleColor();
					ImGui::Spacing();

					ImGui::PushFont(main_font);
					ImGui::SetCursorPosX(7);

					if (TabButton("Combat", ImVec2(125, 23), 1, (void*)combat_texture, ImVec2(30, 30)))
					{
						selected_id[0] = 1;
						selected_id[1] = 0;
						fade_in = true;
					}


					ImGui::Spacing();
					ImGui::SetCursorPosX(7);
					//if (TabButton("Movement", ImVec2(125, 23), 2, "2"))
					if (TabButton("Movement", ImVec2(125, 23), 2, (void*)movement_texture, ImVec2(30, 30)))
					{
						selected_id[0] = 2;
						selected_id[1] = 0;
					}

					ImGui::Spacing();
					ImGui::SetCursorPosX(7);
					//if (TabButton("Player", ImVec2(125, 23), 3, "4"))
					if (TabButton(xorstr_("Player"), ImVec2(125, 23), 3, (void*)user_texture, ImVec2(30, 30)))
					{
						selected_id[0] = 3;
						selected_id[1] = 0;
					}


					ImGui::Spacing();
					ImGui::SetCursorPosX(7);
					//if (TabButton("Visuals", ImVec2(125, 23), 6, "6"))


					if (TabButton(xorstr_("Visuals"), ImVec2(125, 23), 4, (void*)visuals_texture, ImVec2(30, 30)))
					{
						selected_id[0] = 4;
						selected_id[1] = 0;
					}


					ImGui::Spacing();
					ImGui::SetCursorPosX(7);
					//if (TabButton("Misc", ImVec2(125, 23), 5, "3"))


					if (TabButton(xorstr_("Misc"), ImVec2(125, 23), 5, (void*)menu_texture, ImVec2(30, 30)))
					{
						selected_id[0] = 5;
						selected_id[1] = 0;
					}

					ImGui::Spacing();
					ImGui::SetCursorPosX(7);
					//if (TabButton("Settings", ImVec2(125, 23), 4, "5"))
					if (TabButton(xorstr_("Settings"), ImVec2(125, 23), 6, (void*)setting_texture, ImVec2(30, 30)))
					{
						selected_id[0] = 6;
						selected_id[1] = 0;
					}
					ImGui::PopFont();
					ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.16, 0.16, 0.16, 1.f));
					ImGui::SetCursorPos(ImVec2(139, 0));
					ImGui::BeginChild(xorstr_("top left right bar"), ImVec2(1, 390), false);
					{

					}
					ImGui::EndChild();

					ImGui::PopStyleColor();

				}
				ImGui::EndChild();


				ImGui::SetCursorPos(ImVec2(20, 8));
				ImGui::BeginChild(xorstr_("top right"), ImVec2(s.x - 13, 42), false);
				{
					ImGui::SetCursorPosY(-1);
					{	//tabs
						ImGui::PushFont(font);
						ImGui::SetCursorPosX(150);
						ImGui::BeginGroup();
						switch (selected_id[0])
						{
							case 1:
							{
								std::string* clicker = new std::string(xorstr_("Clicker"));
								std::string* aimassist = new std::string(xorstr_("AimAssist"));
								std::string* reach = new std::string(xorstr_("Reach"));
								std::string* velocity = new std::string(xorstr_("Velocity"));
								std::string* killaura = new std::string(xorstr_("KillAura"));
								std::string* criticals = new std::string(xorstr_("Criticals"));
								if (ImGui::tab(xorstr_("Clicker"), selected_id[1] == 0)) { fade_in = true;  selected_id[1] = 0; slider_animation.clear(); state.clear(); size.clear(); size2.clear(); } ImGui::SameLine();
								if (ImGui::tab(xorstr_("AimAssist"), selected_id[1] == 1)) { fade_in = true; selected_id[1] = 1; slider_animation.clear(); state.clear(); size.clear(); size2.clear();} ImGui::SameLine();
								if (ImGui::tab(xorstr_("Reach"),selected_id[1] == 2)) { fade_in = true; selected_id[1] = 2; slider_animation.clear(); state.clear(); size.clear(); size2.clear();} ImGui::SameLine();
								if (ImGui::tab(xorstr_("Velocity"), selected_id[1] == 3)) { fade_in = true; selected_id[1] = 3; slider_animation.clear(); state.clear(); size.clear(); size2.clear();} ImGui::SameLine();
								if (ImGui::tab(xorstr_("KillAura"), selected_id[1] == 4)) { fade_in = true; selected_id[1] = 4; slider_animation.clear(); state.clear(); size.clear(); size2.clear();} ImGui::SameLine();
								if (ImGui::tab(xorstr_("Criticals"), selected_id[1] == 5)) { fade_in = true; selected_id[1] = 5; slider_animation.clear(); state.clear(); size.clear(); size2.clear();}

								break;
							}

							case 2:
							{
								if (ImGui::tab(xorstr_("Sprint"), selected_id[1] == 0))selected_id[1] = 0; ImGui::SameLine();
								if (ImGui::tab(xorstr_("Speed"), selected_id[1] == 1))selected_id[1] = 1; ImGui::SameLine();
								if (ImGui::tab(xorstr_("FastStop"), selected_id[1] == 2))selected_id[1] = 2; ImGui::SameLine();
								if (ImGui::tab(xorstr_("Flight"), selected_id[1] == 3))selected_id[1] = 3; ImGui::SameLine();
								if (ImGui::tab(xorstr_("BridgeAssist"), selected_id[1] == 4))selected_id[1] = 4; ImGui::SameLine();
								break;
							}
							case 3:
							{
								if (ImGui::tab(xorstr_("Antibot"), selected_id[1] == 0))selected_id[1] = 0; ImGui::SameLine();
								if (ImGui::tab(xorstr_("AutoTool"), selected_id[1] == 1))selected_id[1] = 1; ImGui::SameLine();
								if (ImGui::tab(xorstr_("ChestStealer"), selected_id[1] == 2))selected_id[1] = 2; ImGui::SameLine();
								if (ImGui::tab(xorstr_("FastMine"), selected_id[1] == 3))selected_id[1] = 3; ImGui::SameLine();
								if (ImGui::tab(xorstr_("Nofall"), selected_id[1] == 4))selected_id[1] = 4; ImGui::SameLine();
								break;
							}
							case 4:
							{
								if (ImGui::tab(xorstr_("PlayerESP"), selected_id[1] == 0))selected_id[1] = 0; ImGui::SameLine();
								if (ImGui::tab(xorstr_("Nametags"), selected_id[1] == 1))selected_id[1] = 1; ImGui::SameLine();
								if (ImGui::tab(xorstr_("BlockESP"), selected_id[1] == 2))selected_id[1] = 2; ImGui::SameLine();
								if (ImGui::tab(xorstr_("Skeleton"), selected_id[1] == 3))selected_id[1] = 3; ImGui::SameLine();
								if (ImGui::tab(xorstr_("Chams"), selected_id[1] == 4))selected_id[1] = 4; ImGui::SameLine();
								break;
							}
							case 5:
							{
								if (ImGui::tab(xorstr_("FastPlace"), selected_id[1] == 0))selected_id[1] = 0; ImGui::SameLine();
								if (ImGui::tab(xorstr_("Timer"), selected_id[1] == 1))selected_id[1] = 1; ImGui::SameLine();
							}
							break;

						}

						ImGui::EndGroup();
						ImGui::PopFont();
					}
					ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.16, 0.16, 0.16, 1.f));
					ImGui::SetCursorPos(ImVec2(0, 37));
					ImGui::BeginChild(xorstr_("bar 4"), ImVec2(0, 1), false);
					{

					}
					ImGui::EndChild();

					ImGui::PopStyleColor();
				}
				ImGui::EndChild();

				ImGui::SetCursorPosX(148);
				ImGui::SetCursorPosY(46);
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.f);
				ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 3.f);


				ImGui::GetStyle().Alpha = child_alpha;


				ImGui::BeginChild("dwad", ImVec2(0, 0), false);
				{

					ImGui::SetCursorPosX(5);
					ImGui::SetCursorPosY(5);

					ImGui::PushItemWidth(200.f);

					ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.04, 0.03, 0.04, alpha));
					ImGui::GetStyle().ChildBorderSize = 2.f;

					ImGui::GetStyle().Alpha = child_alpha;
					switch (selected_id[0])
					{
					case 1:
					{
						switch (selected_id[1])
						{
						case 0:
						{
							{
								ImGui::PushFont(font);
								{
									ImGui::SetCursorPosY(20);
									ImGui::BeginGroup();
									ImGui::SetCursorPosX(25);
									ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
									ImGui::BeginChild("##kk", ImVec2(230, 344), false);
									{

										ImGui::SetCursorPosY(10);
										ImGui::SetCursorPosX(4);
										ImGui::Checkbox("Enabled", &clicker::m_enabled);

										ImGui::Spacing();

										ImGui::SetCursorPosX(10);
										ImGui::Text("CPS:");
										ImGui::SetCursorPosX(5);
										ImGui::PushItemWidth(210);

										ImGui::SetCursorPosX(10);
										ImGui::SliderInt("", &clicker::cps, 3, 20, "%d");

										ImGui::Spacing();

										ImGui::SetCursorPosX(10);
										ImGui::Text("Conditions:");
										ImGui::SetCursorPosX(5);
										static std::string label = "";
										ImGui::SetCursorPosX(10);
										MultiSelCombo(label, ImVec2(100, 11), config::clicker::conditions, config::clicker::selected, 4, "##wdoaw");

										ImGui::PopItemWidth();

									}
									ImGui::EndChild();

									ImGui::PopStyleColor();
									ImGui::EndGroup();

									ImGui::SetCursorPosY(20);
									ImGui::BeginGroup();
									ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 230 - 25);
									ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
									ImGui::BeginChild("##kkk", ImVec2(230, 344), false);
									{
										static bool ok = true;

										ImGui::SetCursorPosY(10);
										ImGui::SetCursorPosX(10);

										ImGui::Text("Blockhit Mode");

										ImGui::Spacing();
										ImGui::PushItemWidth(210);

										static std::string label = "";
										ImGui::SetCursorPosX(10);
										MultiSelCombo(label, ImVec2(100, 11), config::clicker::conditions, config::clicker::selected, 4, "##qweqw");

										ImGui::PopItemWidth();

									}
									ImGui::EndChild();

									ImGui::PopStyleColor();
									ImGui::EndGroup();
								}
								ImGui::PopFont();
							}
							break;
						case 1:

						{
							ImGui::PushFont(font);
							{

								ImGui::SetCursorPosY(20);
								ImGui::SetCursorPosX(25);
								ImGui::BeginGroup();
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
								ImGui::BeginChild("##kk", ImVec2(230, 344), false);
								{

									ImGui::SetCursorPosY(10);
									ImGui::SetCursorPosX(4);
									ImGui::Checkbox("Enabled###ok", &aimassist::m_enabled);

									ImGui::Spacing();

									ImGui::SetCursorPosX(10);
									ImGui::Text("Aim Speed:");
									ImGui::SetCursorPosX(10);
									ImGui::PushItemWidth(210);

									ImGui::SliderFloat("###koqwdkolpq", &aimassist::m_speed, 0.1, 10.0, "%.1f");
									ImGui::SetCursorPosX(10);
									ImGui::Text("Aim FOV:");
									ImGui::SetCursorPosX(10);
									ImGui::SliderFloat("###iowd", &aimassist::m_fov, 1., 360., "%.1f");
									ImGui::SetCursorPosX(10);
									ImGui::Text("Aim Distance:");
									ImGui::SetCursorPosX(10);
									ImGui::SliderFloat("###iowwadawd", &aimassist::m_dis, 0.1, 6, "%.1f");

									ImGui::Spacing();

									ImGui::SetCursorPosX(10);
									ImGui::Text("Target Mode:");
									ImGui::SetCursorPosX(10);
									ImGui::Combo("###targetsaim", &config::aimassist::selected_item, config::targets, 3);


									ImGui::Spacing();

									ImGui::SetCursorPosX(10);
									ImGui::Text("Conditions:");
									ImGui::SetCursorPosX(10);
									static std::string label = "";
									MultiSelCombo(label, ImVec2(100, 11), config::aimassist::conditions, config::aimassist::selected, 7, "##hwaduiw");

									ImGui::PopItemWidth();

								}
								ImGui::EndChild();
								ImGui::EndGroup();


							}
							ImGui::PopFont();
						}
						break;
						case 2:
						{
							ImGui::PushFont(font);
							{

								ImGui::SetCursorPosY(20);
								ImGui::SetCursorPosX(25);
								ImGui::BeginGroup();
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
								ImGui::BeginChild("##kk", ImVec2(230, 344), false);
								{

									ImGui::SetCursorPosY(10);
									ImGui::SetCursorPosX(4);

									ImGui::Checkbox("Enabled###reachbool", &reach::m_enabled);

									ImGui::Spacing();

									ImGui::SetCursorPosX(10);
									ImGui::Text("Reach:");
									ImGui::SetCursorPosX(10);
									ImGui::PushItemWidth(210);
									ImGui::SliderFloat("####rikoweoik", &reach::m_reach, 3.f, 5.f, "%.2f");

									ImGui::SetCursorPosX(10);
									ImGui::Text("Hitbox Extend:");
									ImGui::SetCursorPosX(10);
									ImGui::SliderFloat("####klrgkl", &reach::hitbox_size, 0.f, 2.f, "%.2f");

									ImGui::Spacing();

									ImGui::SetCursorPosX(10);
									ImGui::Text("Conditions:");
									ImGui::SetCursorPosX(10);
									static std::string label = "";
									MultiSelCombo(label, ImVec2(100, 11), config::reach::conditions, config::reach::selected, 3, "##awdkokowa");

									ImGui::PopItemWidth();

								}
								ImGui::EndChild();
								ImGui::EndGroup();


							}
							ImGui::PopFont();
						}
						break;
						case 3:
						{
							ImGui::PushFont(font);
							{

								ImGui::SetCursorPosY(20);
								ImGui::SetCursorPosX(25);
								ImGui::BeginGroup();
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
								ImGui::BeginChild("##kk", ImVec2(230, 344), false);
								{

									ImGui::SetCursorPosY(10);
									ImGui::SetCursorPosX(4);

									ImGui::Checkbox("Enabled###reachbool", &velocity::m_enabled);

									ImGui::Spacing();

									ImGui::SetCursorPosX(10);
									ImGui::Text("Horizontal:");
									ImGui::SetCursorPosX(10);
									ImGui::PushItemWidth(210);
									ImGui::SliderFloat("####adawd", &velocity::velocity_h, 0.f, 100.f, "%.1f%%");

									ImGui::SetCursorPosX(10);
									ImGui::Text("Vertical:");
									ImGui::SetCursorPosX(10);
									ImGui::SliderFloat("####klrgkl", &velocity::velocity_v, 0.f, 100.f, "%.1f%%");

									ImGui::SetCursorPosX(10);
									ImGui::Text("Delay:");
									ImGui::SetCursorPosX(10);
									ImGui::SliderInt("####fesf", &velocity::delay_ticks, 0, 6, "%d");

									ImGui::Spacing();

									ImGui::SetCursorPosX(10);
									ImGui::Text("Mode:");
									ImGui::SetCursorPosX(10);
									ImGui::Combo("###combooo", &config::velocity::selected_item, config::velocity::mode, 5);

									ImGui::PopItemWidth();


								}
								ImGui::EndChild();
								ImGui::EndGroup();


							}
							ImGui::PopFont();
						}
						break;
						case 4:
						{
							ImGui::PushFont(font);
							{

								ImGui::SetCursorPosY(20);
								ImGui::SetCursorPosX(25);
								ImGui::BeginGroup();
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
								ImGui::BeginChild("##kk", ImVec2(230, 344), false);
								{

									ImGui::SetCursorPosY(10);
									ImGui::SetCursorPosX(4);

									ImGui::Checkbox("Enabled###ok", &killaura::m_enabled);

									ImGui::Spacing();

									ImGui::SetCursorPosX(10);
									ImGui::Text("APS:");
									ImGui::SetCursorPosX(10);
									ImGui::PushItemWidth(210);
									ImGui::SliderFloat("###koqwdkolpq", &killaura::m_cps, 1.f, 20.f, "%.1f");
									ImGui::SetCursorPosX(10);
									ImGui::Text("Aura FOV:");
									ImGui::SetCursorPosX(10);
									ImGui::SliderFloat("###iowd", &killaura::m_fov, 1., 360., "%.1f");
									ImGui::SetCursorPosX(10);
									ImGui::Text("Aura Distance:");
									ImGui::SetCursorPosX(10);
									ImGui::SliderFloat("###iowwadawd", &killaura::m_dist, 0.1, 6, "%.1f");

									ImGui::Spacing();

									ImGui::SetCursorPosX(10);
									ImGui::Text("Mode:");
									ImGui::SetCursorPosX(10);
									ImGui::Combo("###mode", &config::killaura::selected_mode, config::KA_modes, 2);

									ImGui::SetCursorPosX(10);
									ImGui::Text("Target Mode:");
									ImGui::SetCursorPosX(10);
									ImGui::Combo("###kmdwakomawkmodko", &config::killaura::selected_item, config::targets, 3);

									ImGui::SetCursorPosX(10);
									ImGui::Text("Blockhit Mode:");
									ImGui::SetCursorPosX(10);
									ImGui::Combo("###kko", &config::killaura::blockhit_selected, config::autoblock_modes, 2);

									// BlockHit

								}
								ImGui::EndChild();
								ImGui::EndGroup();


							}
							ImGui::PopFont();
						}
						break;
						case 5:
						{
							ImGui::PushFont(font);
							{

								ImGui::SetCursorPosY(20);
								ImGui::SetCursorPosX(25);
								ImGui::BeginGroup();
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
								ImGui::BeginChild("##kk", ImVec2(230, 344), false);
								{

									ImGui::SetCursorPosY(10);
									ImGui::SetCursorPosX(4);

									ImGui::Checkbox("Enabled###lmaod", &Criticals::m_enabled);

									ImGui::Spacing();

									ImGui::SetCursorPosX(10);
									ImGui::Text("Mode:");

									ImGui::PushItemWidth(210);
									ImGui::SetCursorPosX(10);
									ImGui::Combo("###klsdfkol", &config::crits::selected_item, config::crit_modes, 1);

									ImGui::PopItemWidth();

									// BlockHit

								}
								ImGui::EndChild();
								ImGui::EndGroup();


							}
							ImGui::PopFont();
						}
						break;
						}
						}

					}
					break;
					case 2:
					{
						switch (selected_id[1])
						{
						case 0:
						{
							ImGui::PushFont(font);
							{

								ImGui::SetCursorPosY(20);
								ImGui::SetCursorPosX(25);
								ImGui::BeginGroup();
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
								ImGui::BeginChild("##kk", ImVec2(230, 344), false);
								{

									ImGui::SetCursorPosY(10);
									ImGui::SetCursorPosX(4);

									ImGui::Checkbox("Enabled###SprintBool", &sprint::m_enabled);

									ImGui::Spacing();

									// BlockHit

								}
								ImGui::EndChild();
								ImGui::EndGroup();


							}
							ImGui::PopFont();
						}
						break;
						case 1:
						{
							ImGui::PushFont(font);
							{

								ImGui::SetCursorPosY(20);
								ImGui::SetCursorPosX(25);
								ImGui::BeginGroup();
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
								ImGui::BeginChild("##kk", ImVec2(230, 344), false);
								{

									ImGui::SetCursorPosY(10);
									ImGui::SetCursorPosX(4);

									ImGui::Checkbox("Enabled###SpeedBool", &speed::m_enabled);

									if (config::speed::selected_mode == 4) // Custom
									{
										ImGui::PushItemWidth(210);
										ImGui::Spacing();

										ImGui::SetCursorPosX(10);
										ImGui::Text("Speed:");
										ImGui::SetCursorPosX(10);
										ImGui::SliderFloat("###bhopspeed", &speed::m_speed, 1, 10, "%.1f");

										ImGui::SetCursorPosX(10);
										ImGui::Text("Y Motion:");
										ImGui::SetCursorPosX(10);
										ImGui::SliderFloat("###ymotion", &speed::y_motion, 0, 1, "%.2f");

										ImGui::SetCursorPosX(10);
										ImGui::Text("OnGround offset:");
										ImGui::SetCursorPosX(10);
										ImGui::SliderFloat("###offsetyes", &speed::m_offset, 0, 1, "%.2f");

										ImGui::PopItemWidth();
									}

									ImGui::Spacing();

									ImGui::SetCursorPosX(10);
									ImGui::Text("Mode:");

									ImGui::PushItemWidth(210);
									ImGui::SetCursorPosX(10);


									ImGui::Combo("###SpeedMode", &config::speed::selected_mode, config::speed_modes, 5);

								}
								ImGui::EndChild();
								ImGui::EndGroup();


							}
							ImGui::PopFont();
						}
						break;
						case 2:
						{
							ImGui::PushFont(font);
							{

								ImGui::SetCursorPosY(20);
								ImGui::SetCursorPosX(25);
								ImGui::BeginGroup();
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
								ImGui::BeginChild("##kk", ImVec2(230, 344), false);
								{

									ImGui::SetCursorPosY(10);
									ImGui::SetCursorPosX(4);

									ImGui::Checkbox("Enabled###FastStopBool", &faststop::m_enabled);

									ImGui::Spacing();

								}
								ImGui::EndChild();
								ImGui::EndGroup();


							}
							ImGui::PopFont();
						}
						break;
						case 3:
						{
							ImGui::PushFont(font);
							{

								ImGui::SetCursorPosY(20);
								ImGui::SetCursorPosX(25);
								ImGui::BeginGroup();
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
								ImGui::BeginChild("##kk", ImVec2(230, 344), false);
								{

									ImGui::SetCursorPosY(10);
									ImGui::SetCursorPosX(4);

									ImGui::Checkbox("Enabled###FlightBool", &flight::m_enabled);

									if (config::flight::selected_mode == 1) // Custom
									{
										ImGui::PushItemWidth(210);
										ImGui::Spacing();

										ImGui::SetCursorPosX(10);
										ImGui::Text("Speed:");
										ImGui::SetCursorPosX(10);
										ImGui::SliderFloat("###flyspeed", &flight::m_speed, 1, 10, "%.1f");

										ImGui::PopItemWidth();
									}

									ImGui::Spacing();

									ImGui::SetCursorPosX(10);
									ImGui::Text("Mode:");

									ImGui::PushItemWidth(210);
									ImGui::SetCursorPosX(10);


									ImGui::Combo("###FlightModee", &config::flight::selected_mode, config::flight_modes, 3);

									ImGui::Spacing();

								}
								ImGui::EndChild();
								ImGui::EndGroup();

							}
							ImGui::PopFont();
						}
						break;
						case 4:
						{
							ImGui::PushFont(font);
							{

								ImGui::SetCursorPosY(20);
								ImGui::SetCursorPosX(25);
								ImGui::BeginGroup();
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
								ImGui::BeginChild("##kk", ImVec2(230, 344), false);
								{


									ImGui::SetCursorPosY(10);
									ImGui::SetCursorPosX(4);

									ImGui::Checkbox(xorstr_("Enabled###qweqwe"), &bridgeassist::m_enabled);

									ImGui::Spacing();

									ImGui::SetCursorPosX(10);
									ImGui::Text(xorstr_("Delay:"));
									ImGui::PushItemWidth(210);
									ImGui::SetCursorPosX(10);
									ImGui::SliderInt(xorstr_("###delaay"), &bridgeassist::m_delay, 0, 250, "%d");

									ImGui::Spacing();

									ImGui::SetCursorPosX(10);
									ImGui::Checkbox(xorstr_("Block only"), &bridgeassist::block_only);
									ImGui::SetCursorPosX(10);
									ImGui::Checkbox(xorstr_("Require sneak(ctrl)"), &bridgeassist::crouch_first);
									// add the conditions

								}
								ImGui::EndChild();
								ImGui::EndGroup();

							}
							ImGui::PopFont();
						}
						break;
						}
					}
					break;
					case 3:
					{
						switch (selected_id[1])
						{
						case 0:
						{
							ImGui::PushFont(font);
							{

								ImGui::SetCursorPosY(20);
								ImGui::SetCursorPosX(25);
								ImGui::BeginGroup();
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
								ImGui::BeginChild("##kk", ImVec2(230, 344), false);
								{

									ImGui::SetCursorPosY(10);
									ImGui::SetCursorPosX(4);

									ImGui::Checkbox("Enabled###antibotbool", &antibot::m_enabled);

								}
								ImGui::EndChild();
								ImGui::EndGroup();


							}
							ImGui::PopFont();
						}
						break;
						case 1:
						{
							ImGui::PushFont(font);
							{

								ImGui::SetCursorPosY(20);
								ImGui::SetCursorPosX(25);
								ImGui::BeginGroup();
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
								ImGui::BeginChild("##kk", ImVec2(230, 344), false);
								{

									ImGui::SetCursorPosY(10);
									ImGui::SetCursorPosX(4);

									ImGui::Checkbox("Enabled###AutoToolbool", &autotool::m_enabled);

								}
								ImGui::EndChild();
								ImGui::EndGroup();


							}
							ImGui::PopFont();
						}
						break;
						case 2:
						{
							ImGui::PushFont(font);
							{

								ImGui::SetCursorPosY(20);
								ImGui::SetCursorPosX(25);
								ImGui::BeginGroup();
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
								ImGui::BeginChild("##kk", ImVec2(230, 344), false);
								{

									ImGui::SetCursorPosY(10);
									ImGui::SetCursorPosX(4);

									ImGui::Checkbox("Enabled###Cheststealerbool", &cheststealer::m_enabled);

									ImGui::Spacing();
									ImGui::SetCursorPosX(5);
									ImGui::Text("Delay:");
									ImGui::PushItemWidth(150);
									ImGui::SetCursorPosX(5);
									ImGui::SliderInt("###delaaay", &cheststealer::m_delay, 100, 500, "%d");

								}
								ImGui::EndChild();
								ImGui::EndGroup();


							}
							ImGui::PopFont();
						}
						break;
						case 3:
						{
							ImGui::PushFont(font);
							{

								ImGui::SetCursorPosY(20);
								ImGui::SetCursorPosX(25);
								ImGui::BeginGroup();
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
								ImGui::BeginChild("##kk", ImVec2(230, 344), false);
								{

									ImGui::SetCursorPosY(10);
									ImGui::SetCursorPosX(4);

									ImGui::Checkbox("Enabled###FastMinebool", &fastmine::m_enabled);

									ImGui::Spacing();
									ImGui::SetCursorPosX(10);
									ImGui::Text("Break block at:");
									ImGui::PushItemWidth(210);
									ImGui::SetCursorPosX(10);
									ImGui::SliderInt("###val", &fastmine::val, 0, 100, "%d%%");

								}
								ImGui::EndChild();
								ImGui::EndGroup();


							}
							ImGui::PopFont();
						}
						break;
						case 4:
						{
							ImGui::PushFont(font);
							{

								ImGui::SetCursorPosY(20);
								ImGui::SetCursorPosX(25);
								ImGui::BeginGroup();
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
								ImGui::BeginChild("##kk", ImVec2(230, 344), false);
								{

									ImGui::SetCursorPosY(10);
									ImGui::SetCursorPosX(4);

									ImGui::Checkbox("Enabled###nofallbool", &nofall::m_enabled);

									ImGui::Spacing();

								}
								ImGui::EndChild();
								ImGui::EndGroup();


							}
							ImGui::PopFont();
						}
						break;
						}
					}
					break;
					case 4:
					{
						switch (selected_id[1])
						{
						case 0:
						{
							ImGui::PushFont(font);
							{

								ImGui::SetCursorPosY(20);
								ImGui::SetCursorPosX(25);
								ImGui::BeginGroup();
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
								ImGui::BeginChild("##kk", ImVec2(230, 344), false);
								{

									ImGui::SetCursorPosY(10);
									ImGui::SetCursorPosX(4);

									ImGui::Checkbox(xorstr_("Enabled###playeresp"), &playeresp::m_enabled);
									ImGui::Spacing();

									ImGui::SetCursorPosX(10);
									ImGui::Text("Mode:");

									ImGui::PushItemWidth(210);
									ImGui::SetCursorPosX(10);
									ImGui::Combo("###2d3d", &config::visuals::esp::esp_mode, config::esp_modes, 2);

									if (config::visuals::esp::esp_mode == 0) // 2d
									{
										ImGui::Spacing();

										// Cornered or Normal or none
										ImGui::SetCursorPosX(10);
										ImGui::Checkbox(xorstr_("Boxes"), &config::visuals::esp::boxes); // and then remove this 

										ImGui::SetCursorPosX(10);
										ImGui::Checkbox(xorstr_("Fill###ssasqaws"), &config::visuals::esp::filled);
										ImGui::SetCursorPosX(10);
										ImGui::Checkbox(xorstr_("HP bar###swas"), &config::visuals::esp::hp_bar);
										ImGui::SetCursorPosX(10);
										ImGui::Checkbox(xorstr_("Names###sdwd"), &config::visuals::esp::names);

									}
									else if (config::visuals::esp::esp_mode == 1) // 3d
									{
										ImGui::Spacing();


										if (config::visuals::esp::outline) {
											ImGui::SetCursorPosX(10);
											ImGui::Text("Outline Width:");
											ImGui::SetCursorPosX(10);
											ImGui::SliderFloat("###okkk", &config::visuals::esp::outline_width, 0.1f, 5.f, "%.2f");
										}
										//if (config::visuals::esp::expand) {
										//	ImGui::Spacing();
										//	ImGui::SetCursorPosX(10);
										//	ImGui::Text("Expand size:");
										//	ImGui::SetCursorPosX(10);
										//	ImGui::SliderFloat("###okkk", &config::visuals::esp::expand_size, 0.1f, 5.f, "%.2f");
										//}


										ImGui::Spacing();
										ImGui::SetCursorPosX(10);
										ImGui::Checkbox(xorstr_("Filled###qweqwe"), &config::visuals::esp::filled);
										ImGui::SetCursorPosX(10);
										ImGui::Checkbox(xorstr_("Expand###wqeqwe"), &config::visuals::esp::expand);
										ImGui::SetCursorPosX(10);
										ImGui::Checkbox(xorstr_("Outlined"), &config::visuals::esp::outline);
									}
									ImGui::PopItemWidth();

								}
								ImGui::EndChild();
								ImGui::EndGroup();


								ImGui::SetCursorPosY(20);
								ImGui::BeginGroup();
								ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 230 - 25);
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
								ImGui::BeginChild("##kkk", ImVec2(230, 344 / 2), false);
								{

									ImGui::SetCursorPosY(10);
									ImGui::SetCursorPosX(10);

									ImGui::Text("Box Color");

									ImGui::SameLine(230 - 10 - ImGui::GetFrameHeight());

									ImGui::ColorEdit4("##aweawe", config::visuals::esp::box_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_RGB);

									ImGui::Spacing();

									ImGui::SetCursorPosX(10);
									ImGui::Text("Fill Color");
									ImGui::SameLine(230 - 10 - ImGui::GetFrameHeight());
									ImGui::ColorEdit4("##dad", config::visuals::esp::fill_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_RGB);

								}
								ImGui::EndChild();

								ImGui::PopStyleColor();
								ImGui::EndGroup();


							}
							ImGui::PopFont();
						}
						break;
							case 3:
							{
								ImGui::PushFont(font);
								{

									ImGui::SetCursorPosY(20);
									ImGui::SetCursorPosX(25);
									ImGui::BeginGroup();
									ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
									ImGui::BeginChild("##kek", ImVec2(230, 344), false);
									{

										ImGui::SetCursorPosY(10);
										ImGui::SetCursorPosX(4);

										ImGui::Checkbox("Enabled###skeeltoton", &playeresp::skeleton);
										ImGui::Spacing();

										ImGui::PushItemWidth(210);

										ImGui::PopItemWidth();

									}
									ImGui::EndChild();
									ImGui::EndGroup();


								}
								ImGui::PopFont();
							}
							break;
						case 4:
						{
							ImGui::PushFont(font);
							{

								ImGui::SetCursorPosY(20);
								ImGui::SetCursorPosX(25);
								ImGui::BeginGroup();
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
								ImGui::BeginChild("##kek", ImVec2(230, 344), false);
								{

									ImGui::SetCursorPosY(10);
									ImGui::SetCursorPosX(4);

									ImGui::Checkbox("Enabled###Chamsss", &chams::m_enabled);
									ImGui::Spacing();
									ImGui::SetCursorPosX(10);
									ImGui::Checkbox("Fill Color###1", &chams::fill_chams);
									ImGui::SetCursorPosX(10);
									ImGui::Checkbox("Wire Chams###2", &chams::wireframe);


									ImGui::Spacing();

									ImGui::SetCursorPosX(10);
									ImGui::Text("Fill Color");
									ImGui::SameLine(230 - 10 - ImGui::GetFrameHeight());
									
									ImGui::ColorEdit3("##aweawe", chams::color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_RGB);

									ImGui::PushItemWidth(210);

									ImGui::PopItemWidth();

								}
								ImGui::EndChild();
								ImGui::EndGroup();


							}
							ImGui::PopFont();
						}
						break;
						}
					}
					break;
						case 5:
						{
							switch (selected_id[1])
							{
							case 0:
							{
								ImGui::PushFont(font);
								{

									ImGui::SetCursorPosY(20);
									ImGui::SetCursorPosX(25);
									ImGui::BeginGroup();
									ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09, 0.08, 0.09, 1.f));
									ImGui::BeginChild("##k2k", ImVec2(230, 344), false);
									{

										ImGui::SetCursorPosY(10);
										ImGui::SetCursorPosX(4);

										ImGui::Checkbox("Enabled###fastplacee", &misc::fastplace::m_enabled);

										ImGui::Spacing();

										ImGui::SetCursorPosX(10);
										ImGui::Text("Ticks:");
										ImGui::PushItemWidth(210);
										ImGui::SetCursorPosX(10);

										ImGui::SliderInt("###wdad", &misc::fastplace::ticks, 0, 5);

										ImGui::PopItemWidth();


										// BlockHit

									}
									ImGui::EndChild();
									ImGui::EndGroup();


								}
								ImGui::PopFont();
							}
							break;
							}
							break;
						}

					}
				}
				ImGui::GetStyle().Alpha = 1.f;


				ImGui::PopStyleColor();
				ImGui::GetStyle().ChildBorderSize = 0.f;

				ImGui::PopItemWidth();

				ImGui::EndChild();

				ImGui::PopStyleVar(2);

			}
			ImGui::End();
		}
	}
	
	ImGui::Render();

	wglMakeCurrent(hooks::gl_context.m_hdc_devicectx, hooks::gl_context.m_glrenderctx);
	
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	wglMakeCurrent(hooks::gl_context.m_hdc_devicectx, hooks::gl_context.m_oglrenderctx);

	c_glrender::get().restore_gl();

	return owglSwapLayerBuffers(hdc, uint);
		
}


LRESULT CALLBACK hooks::wndproc(HWND window_handle, UINT user_msg, WPARAM wparam, LPARAM lparam) {

	if (initialized)
	{
		ImGuiIO& io = ImGui::GetIO();

		if (user_msg == WM_KEYDOWN)
		{
			if (wparam == VK_INSERT)
			{
				if (c_main::get().b_open)
				{
					fade_out = true;
					c_main::get().b_open = false;
				}
				else
					c_main::get().b_open = true;
				
		
			}
		}
		if (c_main::get().b_open && !fade_out)
		{
			switch (user_msg) {
			case WM_LBUTTONDOWN:
				io.MouseDown[0] = true;
				ImGui_ImplWin32_WndProcHandler(window_handle, user_msg, wparam, lparam);
				return true;
				break;
			case WM_LBUTTONUP:
				io.MouseDown[0] = false;
				break;
			case WM_RBUTTONDOWN:
				io.MouseDown[1] = true;
				break;
			case WM_RBUTTONUP:
				io.MouseDown[1] = false;
				break;
			case WM_MBUTTONDOWN:
				io.MouseDown[2] = true;
				break;
			case WM_MBUTTONUP:
				io.MouseDown[2] = false;
				break;
			case WM_XBUTTONDOWN:
				if ((GET_KEYSTATE_WPARAM(wparam) & MK_XBUTTON1) == MK_XBUTTON1)
					io.MouseDown[3] = true;
				else if ((GET_KEYSTATE_WPARAM(wparam) & MK_XBUTTON2) == MK_XBUTTON2)
					io.MouseDown[4] = true;
				break;
			case WM_XBUTTONUP:
				if ((GET_KEYSTATE_WPARAM(wparam) & MK_XBUTTON1) == MK_XBUTTON1)
					io.MouseDown[3] = false;
				else if ((GET_KEYSTATE_WPARAM(wparam) & MK_XBUTTON2) == MK_XBUTTON2)
					io.MouseDown[4] = false;
				break;
			case WM_MOUSEWHEEL:
				io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wparam) > 0 ? +1.0f : -1.0f;
				break;
			case WM_MOUSEMOVE:

				io.MousePos.x = (signed short)(lparam);
				io.MousePos.y = (signed short)(lparam >> 16);
				ImGui_ImplWin32_WndProcHandler(window_handle, user_msg, wparam, lparam);
				return true;
				break;
			case WM_KEYDOWN:
				if (wparam < 256)
					io.KeysDown[wparam] = 1;
				break;
			case WM_KEYUP:
				if (wparam < 256)
					io.KeysDown[wparam] = 0;
				break;
			case WM_CHAR:
				if (wparam > 0 && wparam < 0x10000)
					io.AddInputCharacter((unsigned short)wparam);
				return true;
				break;

			}
		}
	}

	return CallWindowProcA(g_oriWndProc, window_handle, user_msg, wparam, lparam);
}

#pragma once
#include <tuple>

#include <map>
#include "Maths.hpp"

#include "Color.h"
#include "Font.hpp"

#include "Libs/imgui/imgui.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "Libs/imgui/imgui_internal.h"
#include "Libs/imgui/misc/freetype/imgui_freetype.h"


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//fn_wgl_swap_buffers original_wgl_swap_buffers;

inline GLuint amogus_texture;

inline std::once_flag flag;
inline ImVec2 GUI_ScreenPos;
inline ImFont* main_font;
inline ImFont* medium_font;
inline ImFont* thin_font;
inline ImFont* thin_font2;
inline ImFont* icon_font;
inline ImFont* big_font;
inline ImFont* font;

inline bool fade_out = false;
inline float alpha = 0.;
inline int selected_id[] = { 1, 0 };

inline bool initialized = false;
inline bool hwnd_changed = false;

inline int menuSize[2]{ 540, 360 };

inline static constexpr auto frequency = 1 / 0.75f;


__forceinline float easeIn(float t, float b, float c, float d) {
	float s = 1.70158f;
	float postFix = t /= d;
	return c * (postFix)*t * ((s + 1) * t - s) + b;
}

__forceinline float clip(float n, float lower, float upper)
{
	n = (n > lower) * n + !(n > lower) * lower;
	return (n < upper) * n + !(n < upper) * upper;
}

__forceinline void subMenu(std::string label, int id)
{

	if (selected_id[1] == id)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
	}
	else 
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.23, 0.23, 0.23, 1));
	}
	if(ImGui::TextEx(label.c_str()))
		selected_id[1] = id;

	ImGui::PopStyleColor();
	
}

__forceinline void set_colors()
{
	ImColor mainColor = ImColor(43, 135, 244);
	ImColor bodyColor = ImColor(int(24), int(24), int(24), 255);
	ImColor fontColor = ImColor(int(255), int(255), int(255), 255);

	ImGuiStyle& style = ImGui::GetStyle();

	//style.WindowPadding = ImVec2(0, 0);
	//style.WindowMinSize = ImVec2(10, 10);
	//style.WindowRounding = 5.0f;
	//style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	//style.ChildRounding = 0.0f;

	//style.FramePadding = ImVec2(2, 2);
	//style.FrameRounding = 0.0f;

	//style.ItemSpacing = ImVec2(4, 3);
	//style.ItemInnerSpacing = ImVec2(4, 4);
	//style.TouchExtraPadding = ImVec2(0, 0);
	//style.IndentSpacing = 21.0f;

	//style.ColumnsMinSpacing = 3.0f;
	//style.ScrollbarSize = 8.f;
	//style.ScrollbarRounding = 0.0f;
	//style.GrabMinSize = 0.f;
	//style.GrabRounding = 0.0f;
	//style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	//style.DisplayWindowPadding = ImVec2(0, 0);
	//style.DisplaySafeAreaPadding = ImVec2(0, 0);
	//style.AntiAliasedLines = true;
	style.CurveTessellationTol = 1.25f;

	style.Colors[ImGuiCol_Text] = fontColor;

	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.07058823529, 0.06666666666, 0.07058823529, 1.f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.07058823529, 0.06666666666, 0.07058823529, 1.f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.07058823529, 0.06666666666, 0.07058823529, 1.f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style.Colors[ImGuiCol_TitleBg] = mainColor;
	style.Colors[ImGuiCol_TitleBgActive] = mainColor;
	style.Colors[ImGuiCol_ScrollbarGrab] = mainColor;
	style.Colors[ImGuiCol_CheckMark] = mainColor;
	style.Colors[ImGuiCol_Button] = mainColor;
	style.Colors[ImGuiCol_Header] = mainColor;

	style.Colors[ImGuiCol_ResizeGrip] = mainColor;
	style.Colors[ImGuiCol_PlotLines] = mainColor;
	style.Colors[ImGuiCol_PlotHistogram] = mainColor;
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);

	style.AntiAliasedFill = false;
	style.AntiAliasedLines = false;
	ImGuiIO& io = ImGui::GetIO();

	//if (static bool once = false; !once)
	{
		big_font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\calibrib.ttf", 15.f);
		thin_font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\calibril.ttf", 11.f);
		thin_font2 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\calibril.ttf", 13.f);

		//if (static bool once = false; !once) {
			unsigned int flags = ImGuiFreeType::ForceAutoHint;//ImGuiFreeType::NoHinting;
			ImGuiFreeType::BuildFontAtlas(ImGui::GetIO().Fonts, flags);
		//	once = true;
		//}

		main_font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\calibri.ttf", 12.5f);

		medium_font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\calibrib.ttf", 13.5f);
	

		font = io.Fonts->AddFontFromFileTTF("C:/Users/Gebruiker.EGON/Documents/dev/mc/imgui-masters/imgui-masters/misc/fonts/Museo Sans Cyrl 700.ttf", 13.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		icon_font = io.Fonts->AddFontFromMemoryCompressedTTF(Fonts::iconfonth_compressed_data, 21, 21, NULL, io.Fonts->GetGlyphRangesDefault());
		
		//once = true;
	}
}

__forceinline bool TabButton(std::string label, ImVec2 Width, int id, std::string font_id)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id_ = window->GetID(id);

	const ImVec2 label_size = ImGui::CalcTextSize(label.c_str(), NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size = ImGui::CalcItemSize(Width, label_size.x * 2.0f, label_size.y * 2.0f);

	ImVec2 Max = ImVec2(pos.x + size.x, pos.y + size.y);
	ImRect bb(pos, Max);
	ImGui::ItemSize(size, 2);
	if (!ImGui::ItemAdd(bb, id_))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id_, &hovered, &held);

	static std::map<ImGuiID, float> filled_animation;

	auto it_filled = filled_animation.find(id_);
	if (it_filled == filled_animation.end())
	{
		filled_animation.insert({ id_, 0.f });
		it_filled = filled_animation.find(id_);
	}
	if (hovered && selected_id[0] == id && it_filled->second >= 0.5f)
		it_filled->second = math::clamp(it_filled->second - 1.7f * ImGui::GetIO().DeltaTime, 0.5f, 0.8f); // 0.9
	else
		it_filled->second = math::clamp(it_filled->second + (1.7f * ImGui::GetIO().DeltaTime * ((selected_id[0] == id) ? 1.f : -1.f)), 0.0f, 0.8f);


	ImVec2 one = ImVec2(bb.Min.x, bb.Min.y);
	ImVec2 two = ImVec2(bb.Max.x, bb.Max.y);


	if (selected_id[0] == id)
	{
		//window->DrawList->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(ImVec4(0.17, 0.53, 0.96, 0.95)));
		auto light_blue = ImGui::GetColorU32(ImVec4(0.17, 0.53, 0.96, 1));
		auto dark_blue = ImGui::GetColorU32(ImVec4(0.16, 0.52, 0.95, 1));

		//window->DrawList->AddRectFilledMultiColor(bb.Min, bb.Max, light_blue, dark_blue, light_blue, dark_blue);
		window->DrawList->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(ImVec4(0.17, 0.53, 0.96, it_filled->second)), 1.5f, 15);
		bb.Min.x -= 1;
		bb.Min.y -= 1;

		bb.Max.x += 1;
		bb.Max.y += 1;
		window->DrawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(ImVec4(0.16, 0.34, 0.54, it_filled->second)), 1.5f, 15);
	}

	//if (id == 1)
	//{
	//	one.x -= 17;
	//	two.x -= 17;
	//}
	//else if (id == 2)
	//{
	//	one.x -= 17 - 8;
	//	two.x -= 17 - 8;
	//}
	//else if (id == 3)
	//{
	//	one.x -= 17 + 4;
	//	two.x -= 17 + 4;
	//}
	//else if (id == 4)
	//{
	//	one.x -= 16;
	//	two.x -= 16;
	//}
	//else if (id == 5)
	//{
	//	one.x -= 17 + 8;
	//	two.x -= 17 + 8;
	//}

	ImVec2 ok = bb.Min;

//	ok.x -= Width.x / 2 - font_size + (font_size / 2);;

	//ok.x += label_size.x / 2;
	//ok.x += 40.f;


	//ImGui::SameLine( bb.GetWidth() / 2 - 20, 7);

	//ImGui::Text(label.c_str());

	window->DrawList->AddText(ImVec2(bb.Min.x + 30, bb.Min.y + 5.f), ImGui::GetColorU32(ImGuiCol_Text), label.c_str());
	//ImGui::RenderTextClipped(one, two, label.c_str(), NULL, &label_size, style.ButtonTextAlign, &bb);

	one.x -= 20;
	two.x -= 20;

	one.y -= 6;
	two.y -= 6;

	if (id == 4)
	{
		one.x -= 2;
		two.x -= 2;
	}
	if (id == 5)
	{
		one.x -= 1;
		two.x -= 1;
	}

	//ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.12, 0.66, 0.89, 1.f));
	float x = bb.Min.x;
	ImGui::PushFont(icon_font);
//	ImGui::RenderTextClipped(one, two, font_id.c_str(), NULL, &label_size, style.ButtonTextAlign, &bb);
	
	bb.Min.x = x - ImGui::CalcTextSize(font_id.c_str()).x;
	bb.Min.x += 20.f;
	ImGui::GetWindowDrawList()->AddText(icon_font, 21, bb.Min, ImGui::GetColorU32(selected_id[0] == id ? ImGuiCol_ButtonActive : ImGuiCol_Text), font_id.c_str());
	ImGui::PopFont();
	//ImGui::PopStyleColor();

	return pressed;
}


__forceinline bool TabButton(std::string label, ImVec2 Width, int id, ImTextureID image, ImVec2 img_size)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id_ = window->GetID(id);

	const ImVec2 label_size = ImGui::CalcTextSize(label.c_str(), NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size = ImGui::CalcItemSize(Width, label_size.x * 2.0f, label_size.y * 2.0f);

	ImVec2 Max = ImVec2(pos.x + size.x, pos.y + size.y);
	ImRect bb(pos, Max);
	ImGui::ItemSize(size, 2);
	if (!ImGui::ItemAdd(bb, id_))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id_, &hovered, &held);

	static std::map<ImGuiID, float> filled_animation;

	auto it_filled = filled_animation.find(id_);
	if (it_filled == filled_animation.end())
	{
		filled_animation.insert({ id_, 0.f });
		it_filled = filled_animation.find(id_);
	}
	if (hovered && selected_id[0] == id && it_filled->second >= 0.5f)
		it_filled->second = math::clamp(it_filled->second - 1.7f * ImGui::GetIO().DeltaTime, 0.5f, 0.8f); // 0.9
	else
		it_filled->second = math::clamp(it_filled->second + (1.7f * ImGui::GetIO().DeltaTime * ((selected_id[0] == id) ? 1.f : -1.f)), 0.0f, 0.8f);


	ImVec2 one = ImVec2(bb.Min.x, bb.Min.y);
	ImVec2 two = ImVec2(bb.Max.x, bb.Max.y);


	if (selected_id[0] == id)
	{
		//window->DrawList->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(ImVec4(0.17, 0.53, 0.96, 0.95)));
		auto light_blue = ImGui::GetColorU32(ImVec4(0.17, 0.53, 0.96, 1));
		auto dark_blue = ImGui::GetColorU32(ImVec4(0.16, 0.52, 0.95, 1));

		//window->DrawList->AddRectFilledMultiColor(bb.Min, bb.Max, light_blue, dark_blue, light_blue, dark_blue);
		window->DrawList->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(ImVec4(0.17, 0.53, 0.96, it_filled->second)), 1.5f, 15);
		bb.Min.x -= 1;
		bb.Min.y -= 1;

		bb.Max.x += 1;
		bb.Max.y += 1;
		window->DrawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(ImVec4(0.16, 0.34, 0.54, it_filled->second)), 1.5f, 15);

		float alphak = 0.020f;
		for (float offset = 0.f; offset < 6.f; offset++)
		{
			window->DrawList->AddRectFilled(ImVec2(bb.Min.x - offset, bb.Min.y - offset), ImVec2(bb.Max.x + offset, bb.Max.y + offset), ImGui::GetColorU32(ImVec4(alphak, alphak, alphak, alphak)), 1.5f, 15);
			alphak += offset * 2 / 1000;
		}
	}

	ImVec2 ok = bb.Min;

	window->DrawList->AddText(ImVec2(bb.Min.x + 30, bb.Min.y + 5.f), ImGui::GetColorU32(ImGuiCol_Text), label.c_str());

	ImVec2 iconsize(18, 18);

	bb.Min.x -= iconsize.x;
	bb.Min.x += 22.f;
	bb.Min.y += 3.f;
	
	ImRect bb2(bb.Min , ImVec2(bb.Min.x + iconsize.x, bb.Min.y + iconsize.y));

	window->DrawList->AddImage(image, bb2.Min, bb2.Max,  ImVec2(0, 0),  ImVec2(1, 1), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)));

	//ImGui::PopStyleColor();

	return pressed;
}


__forceinline void Custom_Separator()
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float thickness_draw = 1.0f;
	float thickness_layout = 0.0f;

	float x1 = window->Pos.x;
	float x2 = window->Pos.x + window->Size.x;
	if (!window->DC.GroupStack.empty())
		x1 += window->DC.Indent.x;

	//+ 4 //  - 21
	const ImRect bb(ImVec2(x1 - 3 , window->DC.CursorPos.y), ImVec2(x2, window->DC.CursorPos.y + thickness_draw));

	ImGui::ItemSize(ImVec2(0.0f, thickness_layout));
	const bool item_visible = ImGui::ItemAdd(bb, 0);
	if (item_visible)
	{
		window->DrawList->AddLine(bb.Min, ImVec2(bb.Max.x, bb.Min.y), ImGui::GetColorU32(ImGuiCol_Separator));
	}

}


__forceinline void MultiSelCombo(std::string& label, ImVec2 size, const std::vector<std::tuple<const char*, const char*>> items, bool* selected, size_t size_, const char* cusid)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	if (ImGui::BeginCombo(cusid, label.c_str()))
	{
		label = "";

		std::vector<std::string> vec;

		for (size_t i = 0; i < size_; i++)
		{
			ImGui::Selectable(std::get<0>(items[i]), &selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2(size.x, 12));

			if (selected[i])
				vec.push_back(std::get<0>(items[i]));
			if (ImGui::IsItemHovered() && strlen(std::get<1>(items[i])) > 0)
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(450.0f);
				ImGui::TextUnformatted(std::get<1>(items[i]));
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}

		bool done = false;
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (i >= 3)
				continue;

			if (i >= 2 && !done)
			{
				label += "...";
				done = true;
			}
			else {

				std::string to_add = vec.at(i);

				if (vec.size() == i)
					label += to_add;
				else if (!(i == vec.size() - 1))
				{
					label += to_add + ", ";
				}
				else
					label += to_add;
			}

		}
		ImGui::EndCombo();

	}
	else {

	}
}



__forceinline void MultiSelectableCombo(ImVec2 size, const std::vector<std::tuple<const char*, const char*>> items, bool* selected, size_t size_, const char* cusid)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	static std::string label = "";

	if (ImGui::BeginCombo(cusid, label.c_str()))
	{
		label = "";

		std::vector<std::string> vec;

		for (size_t i = 0; i < size_; i++)
		{
			ImGui::Selectable(std::get<0>(items[i]), &selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2(size.x, 12));
				
			if (selected[i])
				vec.push_back(std::get<0>(items[i]));
			if (ImGui::IsItemHovered() && strlen(std::get<1>(items[i])) > 0)
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(450.0f);
				ImGui::TextUnformatted(std::get<1>(items[i]));
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}

		bool done = false;
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (i >= 3)
				continue;

			if (i >= 2 && !done)
			{
				label += "...";
				done = true;
			}
			else {

				std::string to_add = vec.at(i);

				if (vec.size() == i)
					label += to_add;
				else if (!(i == vec.size() - 1))
				{
					label += to_add + ", ";
				}
				else
					label += to_add;
			}
			
		}
		ImGui::EndCombo();
	
	}
	else {

	}
}



__forceinline void MultiSelectableCombo2(ImVec2 size, const std::vector<std::tuple<const char*, const char*>> items, bool* selected, size_t size_, const char* cusid)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	static std::string label = "";

	if (ImGui::BeginCombo(cusid, label.c_str()))
	{
		label = "";

		std::vector<std::string> vec;

		for (size_t i = 0; i < size_; i++)
		{
			ImGui::Selectable(std::get<0>(items[i]), &selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2(size.x, 12));

			if (selected[i])
				vec.push_back(std::get<0>(items[i]));
			if (ImGui::IsItemHovered() && strlen(std::get<1>(items[i])) > 0)
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(450.0f);
				ImGui::TextUnformatted(std::get<1>(items[i]));
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}

		bool done = false;
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (i >= 3)
				continue;

			if (i >= 2 && !done)
			{
				label += "...";
				done = true;
			}
			else {

				std::string to_add = vec.at(i);

				if (vec.size() == i)
					label += to_add;
				else if (!(i == vec.size() - 1))
				{
					label += to_add + ", ";
				}
				else
					label += to_add;
			}

		}
		ImGui::EndCombo();

	}
	else {

	}
}
__forceinline void MultiSelectableCombo3(ImVec2 size, const std::vector<std::tuple<const char*, const char*>> items, bool* selected, size_t size_, const char* cusid)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	static std::string label = "";

	if (ImGui::BeginCombo(cusid, label.c_str()))
	{
		label = "";

		std::vector<std::string> vec;

		for (size_t i = 0; i < size_; i++)
		{
			ImGui::Selectable(std::get<0>(items[i]), &selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2(size.x, 12));

			if (selected[i])
				vec.push_back(std::get<0>(items[i]));
			if (ImGui::IsItemHovered() && strlen(std::get<1>(items[i])) > 0)
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(450.0f);
				ImGui::TextUnformatted(std::get<1>(items[i]));
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}

		bool done = false;
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (i >= 3)
				continue;

			if (i >= 2 && !done)
			{
				label += "...";
				done = true;
			}
			else {

				std::string to_add = vec.at(i);

				if (vec.size() == i)
					label += to_add;
				else if (!(i == vec.size() - 1))
				{
					label += to_add + ", ";
				}
				else
					label += to_add;
			}

		}
		ImGui::EndCombo();

	}
	else {

	}
}
__forceinline void MultiSelectableCombo4(ImVec2 size, const std::vector<std::tuple<const char*, const char*>> items, bool* selected, size_t size_, const char* cusid)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	static std::string label = "";

	if (ImGui::BeginCombo(cusid, label.c_str()))
	{
		label = "";

		std::vector<std::string> vec;

		for (size_t i = 0; i < size_; i++)
		{
			ImGui::Selectable(std::get<0>(items[i]), &selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2(size.x, 12));

			if (selected[i])
				vec.push_back(std::get<0>(items[i]));
			if (ImGui::IsItemHovered() && strlen(std::get<1>(items[i])) > 0)
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(450.0f);
				ImGui::TextUnformatted(std::get<1>(items[i]));
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}

		bool done = false;
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (i >= 3)
				continue;

			if (i >= 2 && !done)
			{
				label += "...";
				done = true;
			}
			else {

				std::string to_add = vec.at(i);

				if (vec.size() == i)
					label += to_add;
				else if (!(i == vec.size() - 1))
				{
					label += to_add + ", ";
				}
				else
					label += to_add;
			}

		}
		ImGui::EndCombo();

	}
	else {

	}
}
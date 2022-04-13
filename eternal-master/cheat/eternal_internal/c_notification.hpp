#pragma once
#include "Libs/imgui/imgui.h"
#include <string>
#include "Maths.hpp"


class c_notification {
private:
	ImVec2 pos;
	ImVec2 size;
	int type;
	int xshouldbe;
	ImFont* big_font;

public:
	std::string message;
	bool closing = false;

	DWORD startTime;

	friend bool operator== (const c_notification& n1, const c_notification& n2);

	c_notification(const DWORD _time_, const ImVec2 size_, const std::string msg) // add type
	{
		HWND mc_window = FindWindowA("LWJGL", nullptr); // static

		RECT res;
		GetClientRect(mc_window, &res);

		int width = res.right - res.left;
		int height = res.bottom - res.top;

		// - 200 - 5
		pos = ImVec2{ static_cast<float>(width), static_cast<float>(height - 75 - 10) };


		size = size_;
		message = msg;

		xshouldbe = pos.x - 200 - 5;
		startTime = _time_;

	}

	void run()
	{
		ImGuiIO& io = ImGui::GetIO();

		if (xshouldbe < pos.x)
		{
			pos.x = math::interpolate(pos.x, xshouldbe, io.DeltaTime * 8);
		}

		ImGui::SetNextWindowPos(pos);
		ImGui::SetNextWindowSize(size);
		ImGui::SetNextWindowBgAlpha(1.f);

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
		ImGui::Begin("###noti", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
		{
			ImGui::Spacing();
			//ImGui::PushFont(big_font);
			ImGui::SetCursorPosX(4);
			ImGui::Text("Notification");
			//ImGui::PopFont();

			ImGui::Spacing();

			ImGui::SetCursorPosX(4);
			ImGui::Text(message.c_str());
		}
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();

		ImGui::End();

		if (GetTickCount64() - startTime >= 2500)
		{
			this->closing = true;
		}
	}

	void close_withanim()
	{
		ImGuiIO& io = ImGui::GetIO();

		// messed up the font and styles here
		
		//if(!big_font)
		//	big_font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\calibrib.ttf", 15.f);


		pos.x = math::interpolate(pos.x, pos.x + 200, io.DeltaTime * 8);

		ImGui::SetNextWindowPos(pos);
		ImGui::SetNextWindowSize(size);
		ImGui::SetNextWindowBgAlpha(1.f);

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
		ImGui::Begin("###noti", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
		{
			ImGui::Spacing();
			//ImGui::PushFont(big_font);
			ImGui::SetCursorPosX(4);
			ImGui::Text("Notification");
			//ImGui::PopFont();

			ImGui::Spacing();

			ImGui::SetCursorPosX(4);
			ImGui::Text(message.c_str());
		}
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();

		ImGui::End();
	}


};

inline bool operator== (const c_notification& n1, const c_notification& n2)
{
	return n1.message == n2.message;
}

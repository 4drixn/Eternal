#pragma once

#include <map>
#include <tuple>
#include "TargetEnum.hpp"

namespace config {

	inline const char* targets[]{ "Crosshair", "Distance", "Health" };
	inline const char* crit_modes[]{ "Hypixel" };
	inline const char* autoblock_modes[]{ "None", "Hypixel" };
	inline const char* speed_modes[]{ "Hypixel", "Verus", "Cubecraft", "YPort", "Custom" };
	inline const char* flight_modes[]{ "Hypixel", "Vanilla" , "Custom" };
	inline const char* KA_modes[]{ "Single", "Multi" };
	inline const char* team_modes[] { "Vanilla", "Hypixel", "Team color"};
	
	inline const char* esp_modes[] { "2D", "3D"};

	namespace global {
		inline int team_mode = 0;
	}

	namespace visuals
	{
		namespace esp
		{
			inline int esp_mode = 0;

			inline float outline_width = 1.f;
			inline bool filled = false;
			inline bool hp_bar = false;
			inline bool boxes = false;
			inline bool outline = false;
			inline bool names = false;
			inline bool expand = false;

			inline float fill_color[4] = { 0.8, 0.1, 0.1, 0.1};
			inline float box_color[4] = { 1, 1, 1, 1};
		}

	}
	namespace reach {
		inline std::vector<std::tuple<const char*, const char*>> conditions;
		inline bool selected[3];
	}
	namespace aimassist {
		inline std::string previewValue = "";

		inline std::vector<std::tuple<const char*, const char*>> conditions;
		inline int selected_item = 0;
		inline bool selected[7];

	}
	namespace clicker {
		inline std::vector<std::tuple<const char*, const char*>> conditions;
		inline bool selected[4];
	}
	namespace crits {
		inline int selected_item = 0;
	}
	namespace killaura {
		inline int blockhit_selected = 0;
		inline int selected_item = 0;
		inline int selected_mode = 0;
	}

	namespace velocity {
		inline const char* mode[]{ "Normal", "Reverse", "Glitch", "AACpush", "Jump"};

		inline int selected_item = 0;
	}

	namespace speed {
		inline int selected_mode = 0;
	}

	namespace flight {
		inline int selected_mode = 0;
	}

	inline void init()
	{
		// Aim assist
		aimassist::conditions.push_back(std::make_tuple("Vertical Aim", ""));
		aimassist::conditions.push_back(std::make_tuple("Strafe", "Aims faster when u are strafing"));
		aimassist::conditions.push_back(std::make_tuple("Break Blocks", ""));
		aimassist::conditions.push_back(std::make_tuple("Teams", "Don't aim at teammates"));
		aimassist::conditions.push_back(std::make_tuple("Invisibles", "Target Invisible players"));
		aimassist::conditions.push_back(std::make_tuple("Visible Only", "Don't aim through blocks"));
		aimassist::conditions.push_back(std::make_tuple("NPCS", "Don't aim at NPCS"));

		// Reach
		reach::conditions.push_back(std::make_tuple("Only While sprinting", ""));
		reach::conditions.push_back(std::make_tuple("Visible Only", ""));
		reach::conditions.push_back(std::make_tuple("Disable in Water", ""));

		// Clicker
		clicker::conditions.push_back(std::make_tuple("Break Blocks", ""));
		clicker::conditions.push_back(std::make_tuple("Inventory", "Autoclick in inventory"));
		clicker::conditions.push_back(std::make_tuple("Sword Only", ""));
		clicker::conditions.push_back(std::make_tuple("Axe Only", ""));

	}

}
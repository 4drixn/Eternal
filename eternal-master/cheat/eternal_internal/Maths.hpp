#pragma once
#include <DirectXMath.h>
#include <map>
#include <unordered_map>
#include <Windows.h>

#include "Libs/imgui/imgui.h"

#define M_RADPI 57.295779513082f

#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)


inline std::unordered_map <ImGuiID, bool> state = {};
inline std::unordered_map <ImGuiID, float> size = {};
inline std::unordered_map<ImGuiID, float> size2 = {};

inline std::map<ImGuiID, float> slider_animation;

namespace math {

	template<class T, class U>
	static T clamp(const T& in, const U& low, const U& high)
	{
		if (in <= low)
			return low;

		if (in >= high)
			return high;

		return in;
	}

	//template <typename t>
	//static t interpolate(const t& t1, const t& t2, float progress)
	//{
	//	if (t1 == t2)
	//		return t1;

	//	return t2 * progress + t1 * (1.0f - progress);
	//}

	template <typename t>
	static t lerp(const t& t1, const t& t2, float progress)
	{
		return t1 + (t2 - t1) * progress;
	}

	template <typename t>
	static t lerp2(float progress, const t& t1, const t& t2)
	{
		return t1 + (t2 - t1) * progress;
	}

	//template <typename t>
	inline float interpolate(float a, float b, float amount) {
		return (a * (1.0f - amount) + b * amount);
	}

	inline auto calc_timer = [](DWORD start_time, float num) {
		double current_time = GetTickCount() - start_time;

		if (current_time >= num) {
			start_time = GetTickCount();
			return true;
		}
		return false;
	};

}
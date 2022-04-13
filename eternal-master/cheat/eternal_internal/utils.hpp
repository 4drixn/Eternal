#pragma once

#include <string>
#include <random>
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include <GL/GL.h>
#include <GL/GLU.h>
#include <jni.h>
#include <sstream>
#include <iomanip>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>

#include "c_glrender.hpp"

namespace utils {


	template< typename T >
	__forceinline std::string int_to_hex(T i)
	{
		std::stringstream stream;
			stream << std::hex << i;
		return stream.str();
	}

	namespace colours {
		

		__forceinline std::string RGB2MC(int r, int g, int b)
		{
			std::string hex = "0x";
			hex += int_to_hex(r);
			hex += int_to_hex(g);
			hex += int_to_hex(b);

			return hex;
		}
		__forceinline std::string RGBA2MC(int r, int g, int b, int a)
		{

			std::string hex = "";//0x
			hex += int_to_hex(r);
			hex += int_to_hex(g);
			hex += int_to_hex(b);
			hex += int_to_hex(a);

			return (hex);
		}
	}

	namespace WorldToScreen {

		__forceinline glm::vec4 Multiply(const glm::vec4& vec, std::vector<int> mat)
		{
			return glm::vec4(
				vec.x * mat[0] + vec.y * mat[4] + vec.z * mat[8] + vec.w * mat[12],
				vec.x * mat[1] + vec.y * mat[5] + vec.z * mat[9] + vec.w * mat[13],
				vec.x * mat[2] + vec.y * mat[6] + vec.z * mat[10] + vec.w * mat[14],
				vec.x * mat[3] + vec.y * mat[7] + vec.z * mat[11] + vec.w * mat[15]
			);
		}
		
		__forceinline glm::vec4 Multiply(const glm::vec4& vec, std::vector<GLfloat> mat)
		{
			return glm::vec4(
				vec.x * mat[0] + vec.y * mat[4] + vec.z * mat[8] + vec.w * mat[12],
				vec.x * mat[1] + vec.y * mat[5] + vec.z * mat[9] + vec.w * mat[13],
				vec.x * mat[2] + vec.y * mat[6] + vec.z * mat[10] + vec.w * mat[14],
				vec.x * mat[3] + vec.y * mat[7] + vec.z * mat[11] + vec.w * mat[15]
			);
		}

		__forceinline bool WorldToScreen(const glm::vec3& pointInWorld, glm::vec2& screen, const std::vector<GLfloat>& modelView, const std::vector<GLfloat>& projection, const std::vector<GLint>& viewPort)
		{
			if (projection.size() != 16 || modelView.size() != 16 || viewPort.size() != 16)
				return false;
			const auto clipSpacePos = Multiply(Multiply(glm::vec4(pointInWorld.x, pointInWorld.y, pointInWorld.z, 1.0f), modelView), projection);

			const auto ndcSpacePos = glm::vec3( clipSpacePos.x / clipSpacePos.w, clipSpacePos.y / clipSpacePos.w, clipSpacePos.z / clipSpacePos.w );

			if (ndcSpacePos.z < -1.0 || ndcSpacePos.z > 1.0)
			{
				return false;
			}

			screen.x = viewPort[2] * ((ndcSpacePos.x + 1.0f) / 2.0f);
			screen.y = viewPort[3] * ((1.0f - ndcSpacePos.y) / 2.0f);

			return true;
		}
	}

    __forceinline bool has_special_char(std::string const& str) {
        return std::find_if(str.begin(), str.end(),
            [](char ch) { return !(isalnum(ch) || ch == '_'); }) != str.end();
    }

	__forceinline float _rand_val(float min, float max) {

		++max;


		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_real_distribution<float> dist(min, max);
		return dist(rng);


	}
	__forceinline int _rand_val_int(int min, int max) {

		++max;


		std::random_device device;
		std::mt19937 engine(device());
		std::uniform_real_distribution<> random_value(min, max);
		return (int)random_value(engine);

	}

	//inline bool _is_pressed(byte x) {

	//	return GetAsyncKeyState(x) & 0x8000;
	//}
	
	__forceinline bool is_between(double value, double min, double max) {
		if (value < min) { return false; }
		if (value > max) { return false; }
		return true;
	}


	__forceinline void _vec_sort_2(std::vector<std::tuple<std::string, bool*>> v) {

		for (size_t i = 0; i < v.size(); i++) {

			for (size_t j = i + 1; j < v.size(); j++) {

				// 1 string > 2 string
				std::string one = std::get<0>(v[i]);
				std::string two = std::get<0>(v[j]);
				if (one.size() > two.size()) {

					std::tuple<std::string, bool*> x = v[i];
					v[i] = v[j];
					v[j] = x;
				}

			}

		}
	}


	__forceinline void _vec_sort(std::vector<double>& v) {

		for (size_t i = 0; i < v.size(); i++) {

			for (size_t j = i + 1; j < v.size(); j++) {

				if (v[i] > v[j]) {

					double x = v[i];
					v[i] = v[j];
					v[j] = x;
				}

			}

		}
	}

	/// Wrapper class for OpenGL context
	struct s_context {
		HDC m_hdc_devicectx;
		HGLRC m_glrenderctx;
		HGLRC m_oglrenderctx;
	};


	__forceinline s_context create_gl_context()
	{
		s_context res{};

		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
			1,
			PFD_DRAW_TO_WINDOW | // Format Must Support Window
				PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
				PFD_DOUBLEBUFFER,
			(BYTE)PFD_TYPE_RGBA,
			(BYTE)32,
			(BYTE)0, (BYTE)0, (BYTE)0, (BYTE)0, (BYTE)0, (BYTE)0, // Color Bits Ignored
			(BYTE)0, // Alpha Buffer
			(BYTE)0, // Shift Bit Ignored
			(BYTE)0, // No Accumulation Buffer
			(BYTE)0, (BYTE)0, (BYTE)0, (BYTE)0, // Accumulation Bits Ignored
			(BYTE)24, // 24Bit Z-Buffer (Depth Buffer)
			(BYTE)0, // No Stencil Buffer
			(BYTE)0, // No Auxiliary Buffer
			(BYTE)PFD_MAIN_PLANE, // Main Drawing Layer
			(BYTE)0, // Reserved
			0, 0, 0 // Layer Masks Ignored
		};

		//{
		//	sizeof(PIXELFORMATDESCRIPTOR),
		//	1,
		//	PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		//	PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		//	32,                   // Colordepth of the framebuffer.
		//	0, 0, 0, 0, 0, 0,
		//	0,
		//	0,
		//	0,
		//	0, 0, 0, 0,
		//	24,                   // Number of bits for the depthbuffer
		//	8,                    // Number of bits for the stencilbuffer
		//	0,                    // Number of Aux buffers in the framebuffer.
		//	PFD_MAIN_PLANE,
		//	0,
		//	0, 0, 0
		//};
		res.m_hdc_devicectx = GetDC(FindWindowA("LWJGL", nullptr));

		int pixel_format = ChoosePixelFormat(res.m_hdc_devicectx, &pfd);
		SetPixelFormat(res.m_hdc_devicectx, pixel_format, &pfd);

		res.m_oglrenderctx = wglGetCurrentContext();
		res.m_glrenderctx = wglCreateContext(res.m_hdc_devicectx);
		return res;
	}

	__forceinline bool startsWith(const std::string& s, const std::string& prefix) {
		return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
	}
	__forceinline bool endsWith(std::string const& s, std::string const& ending) {
		if (s.length() >= ending.length()) {
			return (0 == s.compare(s.length() - ending.length(), ending.length(), ending));
		}
		else {
			return false;
		}
	}
	__forceinline bool _in_range(double x, double y, double z) {

		if (y > z) {

			if (x > y) return false;
			if (x < z) return false;
			return true;

		}

		if (z > y) {

			if (x > z) return false;
			if (x < y) return false;
			return true;

		}

		return false;

	}

}

template<typename T>
class singleton {
protected:
	singleton() { }
	~singleton() { }

	singleton(const singleton&) = delete;
	singleton& operator=(const singleton&) = delete;

	singleton(singleton&&) = delete;
	singleton& operator=(singleton&&) = delete;
public:
	static T& get() {
		static T inst{};
		return inst;
	}
};
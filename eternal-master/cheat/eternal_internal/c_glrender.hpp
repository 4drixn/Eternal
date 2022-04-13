#pragma once

#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <Windows.h>
#include "vec2.hpp"
#include <gl\GLU.h>
#include <vector>
#include <gl\GL.h>

#include "AxisAlignedBB.hpp"
#include "Libs/imgui/imgui.h"

template<typename T>
class singleton2 {
protected:
	singleton2() { }
	~singleton2() { }

	singleton2(const singleton2&) = delete;
	singleton2& operator=(const singleton2&) = delete;

	singleton2(singleton2&&) = delete;
	singleton2& operator=(singleton2&&) = delete;
public:
	static T& get() {
		static T inst{};
		return inst;
	}
};

namespace n_glrender{
	inline GLfloat LineWidth;
	inline std::vector<GLfloat> PROJECTION = std::vector<GLfloat>();
	inline std::vector<GLfloat> MODELVIEW = std::vector<GLfloat>();
	inline std::vector<GLint> ViewPort = std::vector<GLint>();
	inline glm::vec3 render_pos;
	inline glm::vec2 view_pos;

	inline std::vector<std::tuple<int, s_axisalignedbb>> block_positions{};

	inline vec3 AimPoint{};
	//inline GLfloat PROJECTION[16];
	//inline GLfloat MODELVIEW[16];
	//inline GLint ViewPort[4];
}

class c_glrender : public singleton2<c_glrender> {
public:

	void setup_ortho();

	void restore_gl();

	void box(GLfloat x, GLfloat y, GLfloat, GLubyte r, GLubyte g, GLubyte b, GLubyte alpha);

	void box_outline(GLfloat width, GLfloat x, GLfloat y, GLfloat xLen, GLfloat yLen, GLubyte r, GLubyte g, GLubyte b, GLubyte alpha);

	void line(GLfloat width, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLubyte r, GLubyte g, GLubyte b, GLubyte alpha);

	void dot(GLfloat size, GLfloat x, GLfloat y, GLubyte r, GLubyte g, GLubyte b, GLubyte alpha);

	vec2 c_glrenderGetRes();

	void drawCrosshair();

	void DrawFilledRect(glm::vec4 w2sbb, const ImVec4 color);

	void drawFilledBoundingBox(s_axisalignedbb axisAlignedBB);

	void drawOutlineBoundingBox(s_axisalignedbb);
	
	void drawBorderedCorneredRect(float x, float y, float x2, float y2, float lineWidth, int lineColor, int bgColor);
	
	void drawCorneredBox(float xTL, float yTL, float width, float height, float lineWidth);

};
#pragma once

#include "main.hpp"
#include "vec3.hpp"


class c_gltext : public singleton<c_gltext> {
public:
	bool m_fontbuilt = false;
	HDC m_fonthdc = nullptr;

	void build_fonts()
	{
		m_fonthdc = wglGetCurrentDC();
		m_cheatfont = glGenLists(96);

		HFONT font = CreateFontA(-15, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Consolas");
		HFONT old_font = (HFONT)SelectObject(m_fonthdc, font);

		wglUseFontBitmaps(m_fonthdc, 32, 96, m_cheatfont);
		SelectObject(m_fonthdc, old_font);
		DeleteObject(font);

		m_fontbuilt = true;
	}
	void print(float x, float y, const unsigned char color[3], const char* format, ...)
	{
		glColor3ub(color[0], color[1], color[2]);
		glRasterPos2f(x, y);

		char text[100];
		va_list	args;

		va_start(args, format);
		vsprintf_s(text, 100, format, args);
		va_end(args);

		glPushAttrib(GL_LIST_BIT);
		glListBase(m_cheatfont - 32);
		glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
		glPopAttrib();
	}

	// centered on X and Y axis'
	vec3 centered_text(float x, float y, float width, float height, float textWidth, float textHeight);

	// centrally aligned on the x axis
	float aligned_text(float x, float width, float textWidth);

private:
	unsigned int m_cheatfont;
};
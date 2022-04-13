#include "c_gltext.hpp"
#include <stdio.h>
#include "main.hpp"

void c_gltext::build_fonts() {
	m_fonthdc = wglGetCurrentDC();
	m_cheatfont = glGenLists(96);

	HFONT font = CreateFontA(-10, 3, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
	HFONT old_font = (HFONT)SelectObject(m_fonthdc, font);

	wglUseFontBitmaps(m_fonthdc, 32, 96, m_cheatfont);
	SelectObject(m_fonthdc, old_font);
	DeleteObject(font);

	m_fontbuilt = true;
}

void c_gltext::print(float x, float y, const unsigned char color[3], const char* format, ...) {
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

vec3 c_gltext::centered_text(float x, float y, float width, float height, float textWidth, float textHeight) {
	vec3 text;
	text.x = x + (width - textWidth) / 2;
	text.y = y + textHeight;

	return text;
}

float c_gltext::aligned_text(float x, float width, float textWidth) {
	if (width > textWidth) {
		float difference = width - textWidth;
		return (x + (difference / 2));
	}
	else {
		float difference = textWidth - width;
		return (x - (difference / 2));
	}
}
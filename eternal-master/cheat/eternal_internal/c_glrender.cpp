#include "c_glrender.hpp"


void c_glrender::setup_ortho() {

	//GLint viewport[4];
	//glGetIntegerv(GL_VIEWPORT, viewport);
	//glViewport(0, 0, viewport[2], viewport[3]);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glDisable(GL_DEPTH_TEST);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(0, 0, viewport[2], viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);

	//glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//glLoadIdentity();
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glLoadIdentity();
	//glDisable(GL_SCISSOR_TEST);
	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_INDEX_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);
	//glDisableClientState(GL_EDGE_FLAG_ARRAY);
	//glMatrixMode(GL_TEXTURE);
	//glPushMatrix();
	//glLoadIdentity();
	//glPolygonMode(GL_FRONT, GL_FILL);
	//glPolygonMode(GL_BACK, GL_FILL);
	//glDisable(GL_LIGHTING);
	//glDisable(GL_FOG);
	//glDisable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_ALPHA_TEST);
	//glDisable(GL_TEXTURE_GEN_S);
	//glDisable(GL_TEXTURE_GEN_T);
	//glDisable(GL_TEXTURE_GEN_R);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glShadeModel(GL_SMOOTH);

	//glGetIntegerv(GL_VIEWPORT, n_glrender::ViewPort);

	//glMatrixMode(GL_PROJECTION);

	//glGetFloatv(GL_PROJECTION_MATRIX, n_glrender::PROJECTION);
	//glGetFloatv(GL_MODELVIEW_MATRIX, n_glrender::MODELVIEW);

	//glOrtho(0.0, static_cast<GLdouble>(n_glrender::ViewPort[2]), static_cast<GLdouble>(n_glrender::ViewPort[3]), 0.0, 0.0, 1.0);


	glGetFloatv(GL_LINE_WIDTH, &n_glrender::LineWidth);
	glLineWidth(4.f);

	glColor4ub(255, 0, 0, 255);

}

void AddLine(ImVec2 min, ImVec2 max, ImColor color)
{
	glLineWidth(1.);
	glEnable(GL_BLEND);
	glColor4f(color.Value.x, color.Value.y, color.Value.z, color.Value.w);
	
	glBegin(GL_LINES);
	glVertex2f(min.x, min.y);
	glVertex2f(max.x, max.y);
	glEnd();
}
void c_glrender::drawCorneredBox(float xTL, float yTL, float width, float height, float lineWidth)
{
	//int newWidth = width / 3;

	//glLineWidth(lineWidth);
	//// black
	////TL Frame
	//AddLine(ImVec2(xTL - 1, yTL - 1), ImVec2(xTL + 1 + newWidth, yTL - 1), IM_COL32_BLACK);
	//AddLine(ImVec2(xTL + 1, yTL + 1), ImVec2(xTL + 1 + newWidth, yTL + 1), IM_COL32_BLACK);
	//AddLine(ImVec2(xTL - 1, yTL - 1), ImVec2(xTL - 1, yTL + newWidth + 1), IM_COL32_BLACK);
	//AddLine(ImVec2(xTL + 1, yTL + 1), ImVec2(xTL + 1, yTL + newWidth + 1), IM_COL32_BLACK);
	////TL Frame Link
	//AddLine(ImVec2(xTL + newWidth, yTL - 1), ImVec2(xTL + newWidth, yTL + 1), IM_COL32_BLACK);
	//AddLine(ImVec2(xTL - 1, yTL + newWidth), ImVec2(xTL + 1, yTL + newWidth), IM_COL32_BLACK);
	////TL Filling

	//AddLine(ImVec2(xTL, yTL), ImVec2(xTL + newWidth, yTL), IM_COL32_WHITE);
	//AddLine(ImVec2(xTL, yTL), ImVec2(xTL, yTL + newWidth), IM_COL32_WHITE);

	////BL Frame
	//AddLine(ImVec2(xTL - 1, yTL + height + 1), ImVec2(xTL + 1 + newWidth, yTL + height + 1), IM_COL32_BLACK);
	//AddLine(ImVec2(xTL + 1, yTL + height - 1), ImVec2(xTL + 1 + newWidth, yTL + height - 1), IM_COL32_BLACK);
	//AddLine(ImVec2(xTL - 1, yTL + height + 1), ImVec2(xTL - 1, yTL + height - newWidth - 1), IM_COL32_BLACK);
	//AddLine(ImVec2(xTL + 1, yTL + height - 1), ImVec2(xTL + 1, yTL + height - newWidth - 1), IM_COL32_BLACK);
	////BL Frame Link
	//AddLine(ImVec2(xTL + newWidth, yTL + height + 1), ImVec2(xTL + newWidth, yTL + height - 1), IM_COL32_BLACK);
	//AddLine(ImVec2(xTL - 1, yTL + height - newWidth - 1), ImVec2(xTL + 1, yTL + height - newWidth - 1), IM_COL32_BLACK);
	////BL Filling

	//AddLine(ImVec2(xTL, yTL + height), ImVec2( xTL + newWidth, yTL + height), IM_COL32_WHITE);
	//AddLine(ImVec2(xTL, yTL + height), ImVec2( xTL, yTL + height - newWidth), IM_COL32_WHITE);

	////TR Frame
	//AddLine(ImVec2(xTL + width + 1, yTL - 1),ImVec2( xTL + width - 1 - newWidth, yTL - 1), IM_COL32_BLACK);
	//AddLine(ImVec2(xTL + width + 1, yTL + 1),ImVec2( xTL + width - 1 - newWidth, yTL + 1), IM_COL32_BLACK);
	//
	//AddLine(ImVec2(xTL + width + 1, yTL - 1),ImVec2( xTL + width + 1, yTL + newWidth + 1), IM_COL32_BLACK);	  
	//AddLine(ImVec2(xTL + width - 1, yTL + 1),ImVec2( xTL + width - 1, yTL + newWidth + 1), IM_COL32_BLACK);
	//
	////TR Frame Link
	//AddLine(ImVec2(xTL + width - newWidth - 0, yTL - 1), ImVec2(xTL + width - newWidth - 1, yTL + 1), IM_COL32_BLACK); // x - 1
	//AddLine(ImVec2(xTL + width, yTL + newWidth), ImVec2(xTL + width - 1, yTL + newWidth), IM_COL32_BLACK); // corrected
	////
	////TR Filling

	//AddLine(ImVec2(xTL + width, yTL), ImVec2(xTL + width - newWidth, yTL), IM_COL32_WHITE);
	//AddLine(ImVec2(xTL + width, yTL), ImVec2(xTL + width, yTL + newWidth), IM_COL32_WHITE);


	////BR Frame
	//AddLine(ImVec2(xTL + width + 1, yTL + height + 1), ImVec2(xTL + width - 1 - newWidth, yTL + height + 1), IM_COL32_BLACK);
	//AddLine(ImVec2(xTL + width + 1, yTL + height - 1), ImVec2(xTL + width - 1 - newWidth, yTL + height - 1), IM_COL32_BLACK);
	//AddLine(ImVec2(xTL + width + 1, yTL + height + 1), ImVec2(xTL + width + 1, yTL + height - newWidth - 1), IM_COL32_BLACK);
	//AddLine(ImVec2(xTL + width - 1, yTL + height - 1), ImVec2(xTL + width - 1, yTL + height - newWidth - 1), IM_COL32_BLACK);
	////BR Frame Link
	//AddLine(ImVec2(xTL + width - newWidth - 1, yTL + height + 1), ImVec2(xTL + width - newWidth - 1, yTL + height - 1), IM_COL32_BLACK);
	//AddLine(ImVec2(xTL + width + 1, yTL + height - newWidth - 1), ImVec2(xTL + width - 1, yTL + height - newWidth - 1), IM_COL32_BLACK);
	////BR Filling
	//AddLine(ImVec2(xTL + width, yTL + height), ImVec2(xTL + width - newWidth, yTL + height), IM_COL32_WHITE);
	//AddLine(ImVec2(xTL + width, yTL + height), ImVec2(xTL + width, yTL + height - newWidth), IM_COL32_WHITE);
	
	//float lineW = width / 4;
	//float lineH = height / 3;
	//
	//const auto draw_corner = [&]()
	//{
	//	glBegin(GL_LINES);

	//	glVertex2f(x, y); glVertex2f(x + lineW, y);
	//	glVertex2f(x, y); glVertex2f(x, y + lineH);


	//	glVertex2f(x + width, y); glVertex2f(x + width, y + lineH);
	//	glVertex2f(x + width, y); glVertex2f(x + width - lineW, y);


	//	glVertex2f(x, y + height); glVertex2f(x, y + height - lineH);
	//	glVertex2f(x, y + height); glVertex2f(x + lineW, y + height);


	//	glVertex2f(x + width, y + height); glVertex2f(x + width, y + height - lineH);
	//	glVertex2f(x + width, y + height); glVertex2f(x + width - lineW, y + height);
	//	glEnd();

	//};

	//width++;
	//height++;
	//glLineWidth(lineWidth + 2);
	//glColor4f(0, 0 , 0, 1);
	//draw_corner();
	//glLineWidth(lineWidth);
	//glColor4f(1, 1, 1, 1);
	//draw_corner();
}

void drawRect(float g, float h, float i, float j, int col1) {
	float f = (float)(col1 >> 24 & 255) / 255.0f;
	float f2 = (float)(col1 >> 16 & 255) / 255.0f;
	float f3 = (float)(col1 >> 8 & 255) / 255.0f;
	float f4 = (float)(col1 & 255) / 255.0f;
	glEnable(3042);
	glDisable(3553);
	glBlendFunc(770, 771);
	glEnable(2848);
	glPushMatrix();
	glColor4f(f2, f3, f4, f);
	glBegin(7);
	glVertex2d(i, h);
	glVertex2d(g, h);
	glVertex2d(g, j);
	glVertex2d(i, j);
	glEnd();
	glPopMatrix();
	glEnable(3553);
	glDisable(3042);
	glDisable(2848);
}

void c_glrender::drawBorderedCorneredRect(float x, float y, float x2, float y2, float lineWidth, int lineColor, int bgColor) {
	drawRect(x, y, x2, y2, bgColor);
	float f = (float)(lineColor >> 24 & 255) / 255.0f;
	float f2 = (float)(lineColor >> 16 & 255) / 255.0f;
	float f3 = (float)(lineColor >> 8 & 255) / 255.0f;
	float f4 = (float)(lineColor & 255) / 255.0f;
	glEnable(3042);
	glEnable(3553);
	drawRect(x - 1.0f, y, x2 + 1.0f, y - lineWidth, lineColor);
	drawRect(x, y, x - lineWidth, y2, lineColor);
	drawRect(x - 1.0f, y2, x2 + 1.0f, y2 + lineWidth, lineColor);
	drawRect(x2, y, x2 + lineWidth, y2, lineColor);
	glDisable(3553);
	glDisable(3042);
}

void c_glrender::restore_gl() {

	glPopMatrix();
	glPopAttrib();

	//glEnable(GL_DEPTH_TEST);
	//glLineWidth(n_glrender::LineWidth);

	//glMatrixMode(GL_TEXTURE);
	//glPopMatrix();
	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);
	//glPopMatrix();
	//glPopAttrib();
	//glPopClientAttrib();

	//glPopMatrix();
	//glPopAttrib();

	glLineWidth(1.f);

	//glMatrixMode(GL_TEXTURE);
	//glPopMatrix();
	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);
	//glPopMatrix();
	//glPopAttrib();
	//glPopClientAttrib();

	//glPopMatrix();
	//glPopAttrib();
	//glEnable(GL_DEPTH_TEST);

	//glPopMatrix();
}


void c_glrender::DrawFilledRect(glm::vec4 w2sbb, const ImVec4 color)
{
	glEnable(GL_BLEND);

	glColor4f(color.x, color.y, color.z, color.w);
	
	glBegin(GL_QUADS);
	glVertex2f(w2sbb.x, w2sbb.y);
	glVertex2f(w2sbb.x, w2sbb.w);
	glVertex2f(w2sbb.z, w2sbb.w);
	glVertex2f(w2sbb.z, w2sbb.y);
	glEnd();

	
}

void c_glrender::box(GLfloat x, GLfloat y, GLfloat sidelength, GLubyte r, GLubyte g, GLubyte b, GLubyte alpha) {
	//Set our attributes
	glEnable(GL_BLEND);
	glColor4ub(r, g, b, alpha);

	GLfloat halfside = sidelength / 2;

	//Draw our box
	glBegin(GL_QUADS);
	glVertex2d(x + halfside, y + halfside);
	glVertex2d(x + halfside, y - halfside);
	glVertex2d(x - halfside, y - halfside);
	glVertex2d(x - halfside, y + halfside);
	glEnd();

	glDisable(GL_BLEND);
}

void c_glrender::box_outline(GLfloat width, GLfloat x, GLfloat y, GLfloat xLen, GLfloat yLen, GLubyte r, GLubyte g, GLubyte b, GLubyte alpha) {
	//Grab the current line width to prevent clashes
	GLfloat glTemp[1];
	glGetFloatv(GL_LINE_WIDTH, glTemp);

	//Set our attributes
	glEnable(GL_BLEND);
	glColor4ub(r, g, b, alpha);
	glLineWidth(width);

	//Draw out box
	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y);
	glVertex2f(x + xLen, y);
	glVertex2f(x + xLen, y + yLen);
	glVertex2f(x, y + yLen);
	glEnd();

	//Restore the line width
	glLineWidth(glTemp[0]);
}

void c_glrender::line(GLfloat width, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLubyte r, GLubyte g, GLubyte b, GLubyte alpha) {
	//Grab the current line width to prevent clashes
	GLfloat glTemp[1];
	glGetFloatv(GL_LINE_WIDTH, glTemp);

	//Set our attributes
	glEnable(GL_BLEND);
	glColor4ub(r, g, b, alpha);
	glLineWidth(width);

	//Draw our line
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();

	//Restore the line width
	glLineWidth(glTemp[0]);
}

void c_glrender::dot(GLfloat size, GLfloat x, GLfloat y, GLubyte r, GLubyte g, GLubyte b, GLubyte alpha) {
	//Grab the current line width to prevent clashes
	GLfloat glTemp[1];
	glGetFloatv(GL_POINT_SIZE, glTemp);

	//Set our attributes
	glEnable(GL_BLEND);
	glColor4ub(r, g, b, alpha);
	glPointSize(size);

	//Draw our point
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();

	//Restore to prevent clashing
	glPointSize(glTemp[0]);
}

vec2 c_glrender::c_glrenderGetRes()
{
	vec2 screenRes;

	float viewport[4] = { 0 };
	glGetFloatv(GL_VIEWPORT, viewport);

	screenRes.x = viewport[2];
	screenRes.y = viewport[3];

	return screenRes;
}

void c_glrender::drawCrosshair()
{
	glPushMatrix();
	GLfloat tmp_viewport[4];
	glGetFloatv(GL_VIEWPORT, tmp_viewport);

	glViewport(0, 0, tmp_viewport[2], tmp_viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, tmp_viewport[2], tmp_viewport[3], 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3ub(240, 240, 240);//white
	glLineWidth(2.0);

	glGetFloatv(GL_VIEWPORT, tmp_viewport);

	int crossHair[8] = { tmp_viewport[2] / 2 - 7, tmp_viewport[3] / 2,// H line
		tmp_viewport[2] / 2 + 7, tmp_viewport[3] / 2,
		tmp_viewport[2] / 2, tmp_viewport[3] / 2 + 7, //V line
		tmp_viewport[2] / 2, tmp_viewport[3] / 2 - 7 };

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_INT, 0, crossHair);
	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_LINES, 2, 2);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

void c_glrender::drawOutlineBoundingBox(s_axisalignedbb axisAlignedBB) {

	glBegin(GL_LINES);

	float incr = axisAlignedBB.minX > axisAlignedBB.maxX ? -1.f : 1.f;
	float end = axisAlignedBB.maxX + incr;
	for (float x = axisAlignedBB.minX; x != end; x += incr) {
		glVertex3f(x, axisAlignedBB.minY, axisAlignedBB.minZ);
		glVertex3f(x, axisAlignedBB.minY, axisAlignedBB.maxZ);
		glVertex3f(x, axisAlignedBB.minY, axisAlignedBB.minZ);
		glVertex3f(x, axisAlignedBB.maxY, axisAlignedBB.minZ);
		glVertex3f(x, axisAlignedBB.maxY, axisAlignedBB.maxZ);
		glVertex3f(x, axisAlignedBB.minY, axisAlignedBB.maxZ);
		glVertex3f(x, axisAlignedBB.maxY, axisAlignedBB.maxZ);
		glVertex3f(x, axisAlignedBB.maxY, axisAlignedBB.minZ);
	}
	incr = axisAlignedBB.minY > axisAlignedBB.maxY ? -1.f : 1.f;
	end = axisAlignedBB.maxY + incr;
	for (float y = axisAlignedBB.minY; y != end; y += incr) {
		glVertex3f(axisAlignedBB.minX, y, axisAlignedBB.minZ);
		glVertex3f(axisAlignedBB.minX, y, axisAlignedBB.maxZ);
		glVertex3f(axisAlignedBB.minX, y, axisAlignedBB.minZ);
		glVertex3f(axisAlignedBB.maxX, y, axisAlignedBB.minZ);
		glVertex3f(axisAlignedBB.maxX, y, axisAlignedBB.maxZ);
		glVertex3f(axisAlignedBB.minX, y, axisAlignedBB.maxZ);
		glVertex3f(axisAlignedBB.maxX, y, axisAlignedBB.maxZ);
		glVertex3f(axisAlignedBB.maxX, y, axisAlignedBB.minZ);
	}
	incr = axisAlignedBB.minZ > axisAlignedBB.maxZ ? -1.f : 1.f;
	end = axisAlignedBB.maxZ + incr;
	for (float z = axisAlignedBB.minZ; z != end; z += incr) {
		glVertex3f(axisAlignedBB.minX, axisAlignedBB.minY, z);
		glVertex3f(axisAlignedBB.minX, axisAlignedBB.maxY, z);
		glVertex3f(axisAlignedBB.minX, axisAlignedBB.minY, z);
		glVertex3f(axisAlignedBB.maxX, axisAlignedBB.minY, z);
		glVertex3f(axisAlignedBB.maxX, axisAlignedBB.maxY, z);
		glVertex3f(axisAlignedBB.minX, axisAlignedBB.maxY, z);
		glVertex3f(axisAlignedBB.maxX, axisAlignedBB.maxY, z);
		glVertex3f(axisAlignedBB.maxX, axisAlignedBB.minY, z);
	}

	glEnd();

}

void c_glrender::drawFilledBoundingBox(s_axisalignedbb axisAlignedBB) {


	glBegin(GL_QUADS);
	{
		glEdgeFlag(GL_TRUE);
		glVertex3d(axisAlignedBB.minX, axisAlignedBB.minY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.minX, axisAlignedBB.maxY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.minY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.maxY, axisAlignedBB.minZ);

		glVertex3d(axisAlignedBB.minX, axisAlignedBB.minY, axisAlignedBB.maxZ);
		glVertex3d(axisAlignedBB.minX, axisAlignedBB.maxY, axisAlignedBB.maxZ);
		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.minY, axisAlignedBB.maxZ);
		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.maxY, axisAlignedBB.maxZ);

		glVertex3d(axisAlignedBB.minX, axisAlignedBB.maxY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.minX, axisAlignedBB.minY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.maxY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.minY, axisAlignedBB.minZ);

		glVertex3d(axisAlignedBB.minX, axisAlignedBB.maxY, axisAlignedBB.maxZ);
		glVertex3d(axisAlignedBB.minX, axisAlignedBB.minY, axisAlignedBB.maxZ);
		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.maxY, axisAlignedBB.maxZ);
		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.minY, axisAlignedBB.maxZ);

		glVertex3d(axisAlignedBB.minX, axisAlignedBB.minY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.minX, axisAlignedBB.maxY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.minX, axisAlignedBB.minY, axisAlignedBB.maxZ);
		glVertex3d(axisAlignedBB.minX, axisAlignedBB.maxY, axisAlignedBB.maxZ);

		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.minY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.maxY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.minY, axisAlignedBB.maxZ);
		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.maxY, axisAlignedBB.maxZ);

		glVertex3d(axisAlignedBB.minX, axisAlignedBB.maxY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.minX, axisAlignedBB.minY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.minX, axisAlignedBB.maxY, axisAlignedBB.maxZ);
		glVertex3d(axisAlignedBB.minX, axisAlignedBB.minY, axisAlignedBB.maxZ);

		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.maxY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.minY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.maxY, axisAlignedBB.maxZ);
		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.minY, axisAlignedBB.maxZ);

		glVertex3d(axisAlignedBB.minX, axisAlignedBB.minY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.minY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.minY, axisAlignedBB.maxZ);
		glVertex3d(axisAlignedBB.minX, axisAlignedBB.minY, axisAlignedBB.maxZ);

		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.minY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.minX, axisAlignedBB.minY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.minX, axisAlignedBB.minY, axisAlignedBB.maxZ);
		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.minY, axisAlignedBB.maxZ);

		glVertex3d(axisAlignedBB.minX, axisAlignedBB.maxY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.maxY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.maxY, axisAlignedBB.maxZ);
		glVertex3d(axisAlignedBB.minX, axisAlignedBB.maxY, axisAlignedBB.maxZ);

		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.maxY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.minX, axisAlignedBB.maxY, axisAlignedBB.minZ);
		glVertex3d(axisAlignedBB.minX, axisAlignedBB.maxY, axisAlignedBB.maxZ);
		glVertex3d(axisAlignedBB.maxX, axisAlignedBB.maxY, axisAlignedBB.maxZ);
	}
	glEnd();
}
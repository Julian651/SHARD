#include "shardWelcomePane.h"
#include <wx/dcclient.h>
#include <GL/freeglut.h>

shardWelcomePane::shardWelcomePane(wxWindow* parent) : shardRenderPane(parent)
{
   glClearColor(0.f, 0.f, 0.f, 0.f);
}

void shardWelcomePane::Render(wxPaintEvent& evt)
{
   wxGLCanvas::SetCurrent(*shardRenderPane::m_context);
   wxPaintDC(this);

   glLoadIdentity();
   glMatrixMode(GL_MODELVIEW);

   glClear(GL_COLOR_BUFFER_BIT);

   for (int i = 0; i < 25; i++)
   {
      float x = (float)i / 1000.f;
      glPushMatrix();
      glColor3f(0.f, 0.1f, 1.f);
      glTranslatef(-0.51f - x, 0.f, 0.f);
      glScalef(0.0025f, 0.0025f, 1.f);
      glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)"SHARD");
      glPopMatrix();
   }

   glBegin(GL_QUADS);
   glColor3f(1.f, 0.f, 0.f);
   glVertex2f(-0.25f, -0.6f);
   glVertex2f(0.25f, -0.6f);
   glVertex2f(0.25f, -0.1f);
   glVertex2f(-0.25f, -0.1f);
   glEnd();

   glFlush();
   SwapBuffers();
}

void shardWelcomePane::onResize(wxSizeEvent& evt)
{
   int w, h;
   this->GetParent()->GetSize(&w, &h);
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-(w>>1), w>>1, -(h>>1), h>>1, 0, 0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   Refresh(); // refresh window.
}

wxBEGIN_EVENT_TABLE(shardWelcomePane, shardRenderPane)
EVT_SIZE(shardWelcomePane::onResize)
wxEND_EVENT_TABLE()

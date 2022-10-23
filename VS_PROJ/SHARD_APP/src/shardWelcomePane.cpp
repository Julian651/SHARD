#include "shardWelcomePane.h"
#include <wx/dcclient.h>

shardWelcomePane::shardWelcomePane(wxWindow* parent) : shardRenderPane(parent)
{
}

void shardWelcomePane::Render(wxPaintEvent& evt)
{
   wxGLCanvas::SetCurrent(*shardRenderPane::m_context);
   wxPaintDC(this);

   glClearColor(0.f, 0.f, 0.f, 0.f);
   glClear(GL_COLOR_BUFFER_BIT);

   glBegin(GL_QUADS);
   glColor3f(1.f, 0.f, 0.f);
   glVertex2f(-0.5f, -0.5f);
   glVertex2f(0.5f, -0.5f);
   glVertex2f(0.5f, 0.5f);
   glVertex2f(-0.5f, 0.5f);
   glEnd();

   glFlush();
   SwapBuffers();
}

#include "shardRenderPane.h"
#include <wx/wx.h>

shardRenderPane::shardRenderPane(wxWindow* parent) : wxGLCanvas(parent)
{
   this->SetName("GLCanvas");
   m_context = new wxGLContext(this);
   int argc = 1;
   char* argv[1] = { wxString((wxTheApp->argv)[0]).char_str() };
   glutInit(&argc, argv);
}

shardRenderPane::~shardRenderPane()
{
   delete m_context;
}

void shardRenderPane::Render(wxPaintEvent& evt)
{
   wxGLCanvas::SetCurrent(*m_context);
   wxPaintDC::wxPaintDC(this);

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

wxBEGIN_EVENT_TABLE(shardRenderPane, wxGLCanvas)
   EVT_PAINT(shardRenderPane::Render)
wxEND_EVENT_TABLE()
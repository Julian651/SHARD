#include "shardModelEditorPane.h"
#include <wx/dcclient.h>

shardModelEditorPane::shardModelEditorPane(wxWindow* parent) : shardRenderPane(parent)
{
}

void shardModelEditorPane::Render(wxPaintEvent& evt)
{
   wxGLCanvas::SetCurrent(*shardRenderPane::m_context);
   wxPaintDC(this);

   glClearColor(0.f, 0.f, 0.f, 0.f);
   glClear(GL_COLOR_BUFFER_BIT);

   glFlush();
   SwapBuffers();
}

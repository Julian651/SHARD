#pragma once
#include <wx/glcanvas.h>
#include <GL/glut.h>

class shardRenderPane : public wxGLCanvas
{
private:
   wxGLContext* m_context;

public:
   shardRenderPane(wxWindow* parent);
   ~shardRenderPane();
   void Render(wxPaintEvent& evt);
   wxDECLARE_EVENT_TABLE();
};
#pragma once
#include <wx/glcanvas.h>
#include <GL/glut.h>

class shardRenderPane : public wxGLCanvas
{
protected:
   wxGLContext* m_context;

public:
   shardRenderPane(wxWindow* parent);
   virtual ~shardRenderPane();
   virtual void Render(wxPaintEvent& evt) = 0;
   wxDECLARE_EVENT_TABLE();
};
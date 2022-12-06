#pragma once
#include <RenderUtilities.h>
#include <wx/glcanvas.h>

class shardRenderPane : public wxGLCanvas
{
protected:

   GLuint m_VAO;
   GLuint m_VBO;
   GLuint m_EBO;

   GLfloat m_width;
   GLfloat m_height;

   ShaderProgram* m_program;;

   wxGLContext* m_context;

public:
   shardRenderPane(wxWindow* parent);
   virtual ~shardRenderPane();
   virtual void Render(wxPaintEvent& evt) = 0;
   wxDECLARE_EVENT_TABLE();
};
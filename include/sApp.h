#pragma once

#include <wx/app.h>
#include <wx/frame.h>
#include <wx/glcanvas.h>

// ====================================================================================================================

class sApp : public wxApp
{
public:

   bool OnInit() override;
};

// ====================================================================================================================

class sFrame : public wxFrame
{
public:

   sFrame();
};

// ====================================================================================================================

class sGLCanvas : public wxGLCanvas
{
private:

   bool firstMouse = true;
   int lastX = -1;
   int lastY = -1;

   int m_screenWidth = -1;
   int m_screenHeight = -1;

   float pitch = 0.f;
   float yaw = -90.f;

   Camera cam;
   wxGLContext* m_context;
   ShaderProgram* m_program;

   glm::mat4 m_projection;
   glm::mat4 m_view;
   
   Line l;

   wxDECLARE_EVENT_TABLE();

   void OnSize(wxSizeEvent& event);
   void OnPaint(wxPaintEvent& event);
   void OnKeyboard(wxKeyEvent& event);
   void OnMouseMove(wxMouseEvent& event);
   void OnRMouseClick(wxMouseEvent& event);
   void OnLMouseClick(wxMouseEvent& event);
   GLuint m_VAO = 0;

#define Bind(theClass) \
   glBindVertexArray(m_VAO); \
   glBindBuffer(GL_ARRAY_BUFFER, theClass::VBO()); \
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theClass::EBO()); \
   theClass::DeclareAttribPointers(); \
   glBindBuffer(GL_ARRAY_BUFFER, 0); \
   glBindVertexArray(0); \
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)

public:

   sGLCanvas(wxWindow* parent, wxGLAttributes& canvasAttribs);
   ~sGLCanvas();
};

// ====================================================================================================================

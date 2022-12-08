#pragma once

#include <wx/glcanvas.h>

#include <Scenes.h>

// ====================================================================================================================

class sGLCanvas : public wxGLCanvas
{
private:

   static bool m_initialized;
   static wxGLContext* m_context;

   wxDECLARE_EVENT_TABLE();

   void OnPaint(wxPaintEvent& event);

public:

   virtual void OnReSize(wxSizeEvent& event) = 0;
   virtual void OnRender() = 0;
   virtual void OnKeyboard(wxKeyEvent& event) = 0;
   virtual void OnMouseMove(wxMouseEvent& event) = 0;
   virtual void OnRMouseClick(wxMouseEvent& event) = 0;
   virtual void OnLMouseClick(wxMouseEvent& event) = 0;

   static void DestroyContext();

   sGLCanvas(wxWindow* parent, wxGLAttributes& canvasAttribs);
   
   virtual ~sGLCanvas();
};

// ====================================================================================================================

class sModelEditor : public sGLCanvas
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
   ShaderProgram* m_program;

   glm::mat4 m_projection;
   glm::mat4 m_view;

   HexaScene* h;
   Line* l;

   void OnReSize(wxSizeEvent& event) override;
   void OnRender() override;
   void OnKeyboard(wxKeyEvent& event) override;
   void OnMouseMove(wxMouseEvent& event) override;
   void OnRMouseClick(wxMouseEvent& event) override;
   void OnLMouseClick(wxMouseEvent& event) override;

public:

   sModelEditor(wxWindow* parent, wxGLAttributes& canvasAttribs);
   ~sModelEditor();
};
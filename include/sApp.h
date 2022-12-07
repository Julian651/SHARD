#pragma once

#include <RenderUtilities.h>
#include <Objects.h>

#include <wx/app.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/glcanvas.h>

// ====================================================================================================================

class sApp : public wxApp
{
public:

   bool OnInit() override;
};

// ====================================================================================================================

enum class eMode
{
   DEFAULT,
   SIMULATION,
   PHYSICAL
};


class sMenuBar : public wxMenuBar
{
private:

   wxMenu* m_menuFile;
   wxMenu* m_menuEdit;
   wxMenu* m_menuView;
   wxMenu* m_menuSettings;
   wxMenu* m_menuHelp;

   // Dynamic file menu items
                                 // new  - static: present in all modes
                                 // open - static: present in all modes
   wxMenuItem* m_itemSave;       // save
   wxMenuItem* m_itemSaveAs;     // save as
   wxMenuItem* m_itemSaveAll;    // save all
   wxMenuItem* m_itemClose;      // close
   wxMenuItem* m_itemCloseAll;   // close all
   wxMenuItem* m_itemRun;        // run sim

   // Dynamic edit menu items
                                    // redo - static: present in all modes
                                    // undo - static: present in all modes
   wxMenuItem* m_itemAddProjEvent;  // add projectile event (smdl)

   eMode m_currentMode = eMode::DEFAULT;

public:

   sMenuBar(eMode mode = eMode::DEFAULT);
   ~sMenuBar();

   void Set(eMode mode);
};

// ====================================================================================================================

class sFrame : public wxFrame
{
private:

    sMenuBar* m_menuBar;

public:

   sFrame();
   ~sFrame();
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

#define GLBind(theClass) \
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

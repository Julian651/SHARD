#pragma once

#include <wx/glcanvas.h>
#include <GLLayer.h>

// ===============================================================================================================

class sGLCanvas : public wxGLCanvas
{
private:

   // Static flag to test whether the canvas is initialized (utilized only for first time initialization
   // of OpenGL and the wxGLContext
   static bool m_initialized;
   // Static gl context which determines the window/canvas being used at varying points in the application
   static wxGLContext* m_context;

   wxDECLARE_EVENT_TABLE();

   /// <summary>
   /// Paint method
   /// </summary>
   void OnPaint(wxPaintEvent& event);

public:

   /// <summary>
   /// Resizing input function
   /// </summary>
   virtual void OnReSize(wxSizeEvent& event) = 0;

   /// <summary>
   /// Rendering input function
   /// </summary>
   virtual void OnRender() = 0;

   /// <summary>
   /// Keyboard input function
   /// </summary>
   virtual void OnKeyboard(wxKeyEvent& event) = 0;

   /// <summary>
   /// Mouse move input function
   /// </summary>
   virtual void OnMouseMove(wxMouseEvent& event) = 0;

   /// <summary>
   /// Right Mouse click input function
   /// </summary>
   virtual void OnRMouseClick(wxMouseEvent& event) = 0;

   /// <summary>
   /// Left mouse click input function
   /// </summary>
   virtual void OnLMouseClick(wxMouseEvent& event) = 0;

   /// <summary>
   /// Destroy the context, used when shutting down the application
   /// </summary>
   static void DestroyContext();

   sGLCanvas(wxWindow* parent, wxGLAttributes& canvasAttribs);
   
   virtual ~sGLCanvas();
};


// ===============================================================================================================

class wxCamera : public Camera
{
private:

   bool firstMouse = true;
   bool confirmTemp = false;
   int lastX = -1;
   int lastY = -1;

   float pitch = 0.f;
   float yaw = -90.f;

public:

   /// <summary>
   /// Determines what happens to the camera when the user inputs a key.
   /// Only works if called directly by programmer from a windows/panels own callback function
   /// </summary>
   /// <returns>Tell the application to refresh</returns>
   bool OnKeyboard(wxKeyEvent& event);

   /// <summary>
   /// Determines what happens to the camera when the user moves the mouse.
   /// Only works if called directly by programmer from a windows/panels own callback function
   /// </summary>
   /// <returns>Tell the application to refresh</returns>
   bool OnMouseMove(wxMouseEvent& event);

   /// <summary>
   /// Determines what happens to the camera when the user right clicks the mouse.
   /// Only works if called directly by programmer from a windows/panels own callback function
   /// </summary>
   /// <returns>Tell the application to refresh</returns>
   bool OnRMouseClick(wxMouseEvent& event);

   /// <summary>
   /// Determines what happens to the camera when the user left clicks the mouse.
   /// Only works if called directly by programmer from a windows/panels own callback function
   /// </summary>
   /// <returns>Tell the application to refresh</returns>
   bool OnLMouseClick(wxMouseEvent& event);

};

// ===============================================================================================================

class sModelEditor : public sGLCanvas
{
private:

   HexaScene* scene;

   wxCamera cam;

   int m_screenWidth = -1;
   int m_screenHeight = -1;

   glm::mat4 m_projection;
   glm::mat4 m_view;

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
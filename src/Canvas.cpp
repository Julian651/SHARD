#include <Canvas.h>
#include <glm/gtc/type_ptr.hpp>
#include <wx/dcclient.h>

wxBEGIN_EVENT_TABLE(sGLCanvas, wxGLCanvas)
EVT_SIZE(OnReSize)
EVT_PAINT(OnPaint)
EVT_KEY_DOWN(OnKeyboard)
EVT_MOTION(OnMouseMove)
EVT_RIGHT_DOWN(OnRMouseClick)
EVT_LEFT_DOWN(OnLMouseClick)
EVT_MOUSEWHEEL(OnScroll)
wxEND_EVENT_TABLE()


// ====================================================================================================================

bool sGLCanvas::m_initialized = false;

wxGLContext* sGLCanvas::m_context = nullptr;

void sGLCanvas::OnPaint(wxPaintEvent& event)
{
   wxPaintDC dc(this);
   SetCurrent(*m_context);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   OnRender();

   SwapBuffers();
}

void sGLCanvas::DestroyContext()
{
   if (m_context)
   {
      delete m_context;
      m_context = nullptr;
   }
}

sGLCanvas::sGLCanvas(wxWindow* parent, wxGLAttributes& canvasAttribs) : wxGLCanvas(parent, canvasAttribs, -1, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS | wxFULL_REPAINT_ON_RESIZE)
{
   if (!m_initialized)
   {
      wxGLContextAttrs ctxAttrs;
      ctxAttrs.PlatformDefaults().CoreProfile().OGLVersion(3, 3).EndList();
      m_context = new wxGLContext(this, nullptr, &ctxAttrs);

      SetCurrent(*m_context);

      if (gl3wInit())
      {
         fprintf(stderr, "failed to initialize OpenGL\n");
         return;
      }
      if (!gl3wIsSupported(3, 3))
      {
         fprintf(stderr, "OpenGL 3.3 not supported\n");
         return;
      }
      m_initialized = true;
   }
   else
   {
      SetCurrent(*m_context);
   }
}

sGLCanvas::~sGLCanvas()
{
}

// ====================================================================================================================

sModelEditor::sModelEditor(wxWindow* parent, wxGLAttributes& canvasAttribs) : sGLCanvas(parent, canvasAttribs)
{

   cam.Move(glm::vec3(0.f, 0.f, 3.f));
   m_view = glm::lookAt(cam.Position(), cam.Position() + cam.Looking(), glm::vec3(0.f, 1.f, 0.f));

   m_projection = glm::mat4(0.f);

   scene = new HexaScene();
}

sModelEditor::~sModelEditor()
{
   if (scene)
   {
      delete scene;
      scene = nullptr;
   }
}

void sModelEditor::OnReSize(wxSizeEvent& event)
{
   int nx = event.GetSize().x;
   int ny = event.GetSize().y;

   m_screenWidth = nx;
   m_screenHeight = ny;

   glViewport(0, 0, nx, ny);

   m_projection = glm::perspective(glm::radians(45.f), 1.f * static_cast<float>(nx) / static_cast<float>(ny), 0.1f, 160.f);
}

void sModelEditor::OnRender()
{
   // do magic
   scene->Render(m_projection, m_view);
   // end do magic
}

void sModelEditor::OnKeyboard(wxKeyEvent& event)
{
   int key = event.GetKeyCode();

   bool ref = cam.OnKeyboard(event);
   m_view = glm::lookAt(cam.Position(), cam.Position() + cam.Looking(), glm::vec3(0.f, 1.f, 0.f));

   switch (key)
   {
   case WXK_RETURN:
   {
       ref = scene->ConfirmTempHex() || ref;
       break;
   }
   }

   Refresh(ref);
}

void sModelEditor::OnMouseMove(wxMouseEvent& event)
{
   bool ref = cam.OnMouseMove(event);
   m_view = glm::lookAt(cam.Position(), cam.Position() + cam.Looking(), glm::vec3(0.f, 1.f, 0.f));
   Refresh(ref);
}

void sModelEditor::OnRMouseClick(wxMouseEvent& event)
{
   Refresh(cam.OnRMouseClick(event));
}

void sModelEditor::OnLMouseClick(wxMouseEvent& event)
{
   int mouse_x = event.GetX();
   int mouse_y = event.GetY();

   float x = (2.f * static_cast<float>(mouse_x)) / static_cast<float>(m_screenWidth) - 1.f;
   float y = 1.f - (2.f * static_cast<float>(mouse_y)) / static_cast<float>(m_screenHeight);
   float z = 1.f;
   glm::vec3 ray_nds = glm::vec3(x, y, z);

   glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.f, 1.f);

   auto projection = glm::perspective(glm::radians(45.f), 1.f * static_cast<float>(m_screenWidth) / static_cast<float>(m_screenHeight), 0.1f, 160.f);
   glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;

   ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.f, 0.f);

   auto view = glm::lookAt(cam.Position(), cam.Position() + cam.Looking(), glm::vec3(0.f, 1.f, 0.f));
   glm::vec4 w = glm::inverse(view) * ray_eye;
   glm::vec3 ray_wor = glm::vec3(w.x, w.y, w.z);

   ray_wor = glm::normalize(ray_wor);

   scene->CheckIntersections(m_projection, m_view, cam.Position(), m_screenWidth, m_screenHeight, mouse_x, mouse_y);
   //if (!confirmTemp && h->CheckIntersections(cam.Position(), ray_wor))
   //{
   //   confirmTemp = true;
   //}

   Refresh(true);
}

void sModelEditor::OnScroll(wxMouseEvent& event)
{
    bool forwardScroll = event.GetWheelRotation() > 0 ? true : false;
    Refresh(scene->RotateTempHex(forwardScroll, 15.f));
}

bool wxCamera::OnKeyboard(wxKeyEvent& event)
{
   int key = event.GetKeyCode();

   switch (key)
   {
   case 'W':
   {
      Move(Position() + Looking());
      break;
   }
   case 'S':
   {
      Move(Position() - Looking());
      break;
   }
   case 'A':
   {
      glm::mat4 rotation = glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f));
      glm::vec3 perp = rotation * glm::vec4(Looking(), 1.f);
      perp.y = 0.f;
      Move(Position() - glm::normalize(perp));
      break;
   }
   case 'D':
   {
      glm::mat4 rotation = glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f));
      glm::vec3 perp = rotation * glm::vec4(Looking(), 1.f);
      perp.y = 0.f;
      Move(Position() + glm::normalize(perp));
      break;
   }
   case WXK_SPACE:
   {
      Move(Position() + glm::vec3(0.f, 1.f, 0.f));
      break;
   }
   case WXK_CONTROL:
   {
      Move(Position() - glm::vec3(0.f, 1.f, 0.f));
      break;
   }
   }
   return true;
}

bool wxCamera::OnMouseMove(wxMouseEvent& event)
{
   if (event.ButtonIsDown(wxMOUSE_BTN_RIGHT))
   {
      float xPos = static_cast<float>(event.GetX());
      float yPos = static_cast<float>(event.GetY());

      if (firstMouse)
      {
         lastX = xPos;
         lastY = yPos;
         firstMouse = false;
         return false;
      }

      float xoffset = xPos - lastX;
      float yoffset = lastY - yPos;
      lastX = xPos;
      lastY = yPos;

      float sensitivity = 0.1f;
      xoffset *= sensitivity;
      yoffset *= sensitivity;

      yaw += xoffset;
      pitch += yoffset;

      if (pitch > 89.f)
      {
         pitch = 89.f;
      }
      if (pitch < -89.f)
      {
         pitch = -89.f;
      }

      glm::vec3 direction = glm::vec3(0.f);
      direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
      direction.y = sin(glm::radians(pitch));
      direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
      LookAt(glm::normalize(direction));
      return true;
   }
   if (event.ButtonIsDown(wxMOUSE_BTN_LEFT))
   {
      return OnLMouseClick(event);
   }
   return false;
}

bool wxCamera::OnRMouseClick(wxMouseEvent& event)
{
   firstMouse = true;
   return false;
}

bool wxCamera::OnLMouseClick(wxMouseEvent& event)
{
   return false;
}

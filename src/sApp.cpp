#include <RenderUtilities.h>
#include <Toolbar.h>
#include <shard.h>
#include <Objects.h>
#include <sApp.h>
#include <wx/dcclient.h>

// ====================================================================================================================

bool sApp::OnInit()
{
	wxFrame* fr = new sFrame;

	fr->Show(true);

	return true;
}

// ====================================================================================================================

sMenuBar::sMenuBar(eMode mode) : wxMenuBar(wxBORDER_NONE)
{
   m_menuFile = new wxMenu;
   m_menuEdit = new wxMenu;
   m_menuView = new wxMenu;
   m_menuSettings = new wxMenu;
   m_menuHelp = new wxMenu;

   m_menuFile->Append(shardMENU_NEW);
   m_menuFile->Append(shardMENU_OPEN);
   m_itemSave     = new wxMenuItem(m_menuFile, shardMENU_SAVE);
   m_itemSaveAs   = new wxMenuItem(m_menuFile, shardMENU_SAVEAS);
   m_itemSaveAll  = new wxMenuItem(m_menuFile, shardMENU_SAVEALL, wxT("Save All"));
   m_itemClose    = new wxMenuItem(m_menuFile, shardMENU_CLOSE);
   m_itemCloseAll = new wxMenuItem(m_menuFile, shardMENU_CLOSE_ALL, wxT("Close All"));
   m_itemRun      = new wxMenuItem(m_menuFile, shardMENU_RUN_SIM, wxT("Run Sim"));
   m_menuFile->AppendSeparator();
   m_menuFile->Append(shardMENU_EXIT);

   m_menuEdit->Append(shardMENU_UNDO);
   m_menuEdit->Append(shardMENU_REDO);
   m_itemAddProjEvent = new wxMenuItem(m_menuEdit, shardMENU_ADDPROJ,
      wxT("Add Projectile Event..."));

   m_menuView->Append(shardMENU_TOGTOOLBAR, wxT("Toggle Toolbar"));
   m_menuView->Append(shardMENU_TOGSTATUSBAR, wxT("Toggle Status Bar"));
   m_menuView->Append(shardMENU_ZOOMIN);
   m_menuView->Append(shardMENU_ZOOMOUT);
   m_menuView->Append(shardMENU_ROTATE, wxT("Rotate..."));
   m_menuView->Append(shardMENU_TRANSLATE, wxT("Translate..."));
   m_menuView->Append(shardMENU_ROTANDTRANS, wxT("Rotate and Translate"));

   m_menuSettings->Append(shardMENU_KEYBINDINGS, wxT("Keybindings..."));

   m_menuHelp->Append(shardMENU_ABOUT);

   this->Append(m_menuFile, wxT("File"));
   this->Append(m_menuSettings, wxT("Settings"));
   this->Append(m_menuHelp, wxT("Help"));

   //Set(mode);
}

sMenuBar::~sMenuBar()
{
   Set(eMode::SIMULATION); // set mode to mode where all menus are being used for automatic deletion via wxwidgets
}

void sMenuBar::Set(eMode mode)
{
   if (m_currentMode == mode)
   {
      return;
   }

   switch (mode)
   {
      case eMode::DEFAULT:
      {
         m_menuFile->Remove(shardMENU_SAVE);
         m_menuFile->Remove(shardMENU_SAVEAS);
         m_menuFile->Remove(shardMENU_SAVEALL);
         m_menuFile->Remove(shardMENU_CLOSE);
         m_menuFile->Remove(shardMENU_CLOSE_ALL);
         m_menuFile->Remove(shardMENU_RUN_SIM);

         this->Remove(size_t(1)); // remove edit menu
         this->Remove(size_t(1)); // remove view menu

         if (m_currentMode == eMode::SIMULATION)
         {
            m_menuEdit->Remove(shardMENU_ADDPROJ);
         }
      }
      break;
      case eMode::SIMULATION:
      {
         m_menuEdit->Append(m_itemAddProjEvent);
         if (m_currentMode == eMode::PHYSICAL)
         {
            break;
         }
      }
      case eMode::PHYSICAL:
      {

         if (m_currentMode == eMode::SIMULATION)
         {
            m_menuEdit->Remove(shardMENU_ADDPROJ);
         }
         else
         {
            m_menuFile->Insert(2, m_itemSave);
            m_menuFile->Insert(3, m_itemSaveAs);
            m_menuFile->Insert(4, m_itemSaveAll);
            m_menuFile->Insert(5, m_itemClose);
            m_menuFile->Insert(6, m_itemCloseAll);
            m_menuFile->Insert(7, m_itemRun);
            this->Insert(1, m_menuEdit, "Edit");
            this->Insert(2, m_menuView, "View");
         }

      }
      break;
   }
   m_currentMode = mode;
}

// ====================================================================================================================

wxIMPLEMENT_APP(sApp);

sFrame::sFrame() : wxFrame(nullptr, wxID_ANY, _("OpenGL wxtest"), wxPoint(300, 300), wxSize(1200, 720))
{
    m_menuBar = new sMenuBar;

   auto toolBar = new Toolbar(this);

   wxPanel* panel = new wxPanel(this);
    
	wxGLAttributes attribs;
	attribs.PlatformDefaults().Defaults().EndList();
	wxGLCanvas* canvas = new sGLCanvas(panel, attribs);
    
   wxBoxSizer* bs = new wxBoxSizer(wxHORIZONTAL);
   bs->Add(canvas, wxSizerFlags(1).Expand());
   panel->SetSizer(bs);

   wxFlexGridSizer* frameSizer = new wxFlexGridSizer(2, 1, 1, 0);
   frameSizer->AddGrowableRow(1);
   frameSizer->AddGrowableCol(0);
   frameSizer->Add(toolBar, wxSizerFlags(1));
   frameSizer->Add(panel, wxSizerFlags(1).Expand());
   this->SetSizer(frameSizer);

   this->SetMenuBar(m_menuBar);
   this->CreateStatusBar();

   auto menuMode = new wxMenu;
   menuMode->Append(10, wxT("DEF"));
   menuMode->Append(11, wxT("SIM"));
   menuMode->Append(12, wxT("PHY"));
   m_menuBar->Append(menuMode, wxT("Mode"));
   this->Bind(wxEVT_MENU, [this](wxCommandEvent& evt)
      {
         switch (evt.GetId())
         {
         case 10:
         {
            this->m_menuBar->Set(eMode::DEFAULT);
         }
         break;
         case 11:
         {
            this->m_menuBar->Set(eMode::SIMULATION);
         }
         break;
         case 12:
         {
            this->m_menuBar->Set(eMode::PHYSICAL);
         }
         break;
         case shardMENU_NEW:
         {
				std::cout << "Testing new menu item functionality\n";
         }
         break;
         }
      });
}

sFrame::~sFrame()
{
}

// ====================================================================================================================

wxBEGIN_EVENT_TABLE(sGLCanvas, wxGLCanvas)
EVT_SIZE(sGLCanvas::OnSize)
EVT_PAINT(sGLCanvas::OnPaint)
EVT_KEY_DOWN(sGLCanvas::OnKeyboard)
EVT_MOTION(sGLCanvas::OnMouseMove)
EVT_RIGHT_DOWN(sGLCanvas::OnRMouseClick)
EVT_LEFT_DOWN(sGLCanvas::OnLMouseClick)
wxEND_EVENT_TABLE()

void sGLCanvas::OnSize(wxSizeEvent& event)
{
	int nx = event.GetSize().x;
	int ny = event.GetSize().y;

	m_screenWidth = nx;
	m_screenHeight = ny;

	glViewport(0, 0, nx, ny);

	m_projection = glm::perspective(glm::radians(45.f), 1.f * static_cast<float>(nx) / static_cast<float>(ny), 0.1f, 160.f);
}

void sGLCanvas::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// do magic
	Cube c(25);
	Hexagon c2(glm::vec3(0.f, 0.f, 25.f));
	glBindVertexArray(m_VAO);

	c.Render(*m_program, m_projection, m_view);
	c2.Render(*m_program, m_projection, m_view);
	l.Render(*m_program, m_projection, m_view);
	c2.GetFace(1);
	// end do magic

	SwapBuffers();
}

void sGLCanvas::OnKeyboard(wxKeyEvent& event)
{
	int key = event.GetKeyCode();
	bool ref = false;

	switch (key)
	{
		case 'W':
		{
			cam.Move(cam.Position() + cam.Looking());
			m_view = glm::lookAt(cam.Position(), cam.Position() + cam.Looking(), glm::vec3(0.f, 1.f, 0.f));
			ref = true;
			break;
		}
		case 'S':
		{
			cam.Move(cam.Position() - cam.Looking());
			m_view = glm::lookAt(cam.Position(), cam.Position() + cam.Looking(), glm::vec3(0.f, 1.f, 0.f));
			ref = true;
			break;
		}
		case 'A':
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f));
			glm::vec3 perp = rotation * glm::vec4(cam.Looking(), 1.f);
			perp.y = 0.f;
			cam.Move(cam.Position() - glm::normalize(perp));
			m_view = glm::lookAt(cam.Position(), cam.Position() + cam.Looking(), glm::vec3(0.f, 1.f, 0.f));
			ref = true;
			break;
		}
		case 'D':
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f));
			glm::vec3 perp = rotation * glm::vec4(cam.Looking(), 1.f);
			perp.y = 0.f;
			cam.Move(cam.Position() + glm::normalize(perp));
			m_view = glm::lookAt(cam.Position(), cam.Position() + cam.Looking(), glm::vec3(0.f, 1.f, 0.f));
			ref = true;
			break;
		}
		case WXK_SPACE:
		{
			cam.Move(cam.Position() + glm::vec3(0.f, 1.f, 0.f));
			m_view = glm::lookAt(cam.Position(), cam.Position() + cam.Looking(), glm::vec3(0.f, 1.f, 0.f));
			ref = true;
			break;
		}
		case WXK_CONTROL:
		{
			cam.Move(cam.Position() - glm::vec3(0.f, 1.f, 0.f));
			m_view = glm::lookAt(cam.Position(), cam.Position() + cam.Looking(), glm::vec3(0.f, 1.f, 0.f));
			ref = true;
			break;
		}
	}
	
	Refresh(ref);
}

void sGLCanvas::OnMouseMove(wxMouseEvent& event)
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
			return;
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
		cam.LookAt(glm::normalize(direction));
		m_view = glm::lookAt(cam.Position(), cam.Position() + cam.Looking(), glm::vec3(0.f, 1.f, 0.f));
		Refresh(true);
	}
	if (event.ButtonIsDown(wxMOUSE_BTN_LEFT))
	{
		OnLMouseClick(event);
	}
}

void sGLCanvas::OnRMouseClick(wxMouseEvent& event)
{
	firstMouse = true;
}

void sGLCanvas::OnLMouseClick(wxMouseEvent& event)
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

	l.Move(cam.Position() - cam.Looking(), ray_wor, 300.f);

	Refresh(true);
}

sGLCanvas::sGLCanvas(wxWindow* parent, wxGLAttributes& canvasAttribs) : wxGLCanvas(parent, canvasAttribs)
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

	m_program = new ShaderProgram;
	m_program->Attach(compileShader("resources\\myshader.vert", eShaderType::VERTEX), compileShader("resources\\myfragment.frag", eShaderType::FRAGMENT));
	m_program->Link();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glGenVertexArrays(1, &m_VAO);

	Hexagon::Initialize();
	Cube::Initialize();
	Line::Initialize();
	GLBind(Line);
	GLBind(Hexagon);
	GLBind(Object<Cube>);

	cam.Move(glm::vec3(0.f, 0.f, 100.f));
	m_view = glm::lookAt(cam.Position(), cam.Position() + cam.Looking(), glm::vec3(0.f, 1.f, 0.f));
}

sGLCanvas::~sGLCanvas()
{
	if (m_context)
	{
		delete m_context;
		m_context = nullptr;
	}

	if (m_program)
	{
		delete m_program;
		m_program = nullptr;
	}

	glDeleteVertexArrays(1, &m_VAO);

	Hexagon::Destroy();
	Cube::Destroy();
	Line::Destroy();
}

// ====================================================================================================================

int main(int argc, char** argv)
{
	return WinMain(::GetModuleHandle(NULL), NULL, ::GetCommandLineA(), SW_SHOWNORMAL);
}

// ====================================================================================================================
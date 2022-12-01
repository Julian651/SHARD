#include <RenderUtilities.h>
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

wxIMPLEMENT_APP(sApp);

sFrame::sFrame() : wxFrame(nullptr, wxID_ANY, _("OpenGL wxtest"), wxPoint(300, 300), wxSize(1200, 720))
{
	wxGLAttributes attribs;

	attribs.PlatformDefaults().Defaults().EndList();

	wxGLCanvas* canvas = new sGLCanvas(this, attribs);
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
	m_screenHeight = nx;

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
}

void sGLCanvas::OnRMouseClick(wxMouseEvent& event)
{
	firstMouse = true;
}

void sGLCanvas::OnLMouseClick(wxMouseEvent& event)
{
	int mouse_x = event.GetX();
	int mouse_y = event.GetY();
	float x = (2.f * static_cast<float>(mouse_x)) / m_screenWidth - 1.f;
	float y = 1.f - (2.f * static_cast<float>(mouse_y)) / m_screenHeight;
	float z = 1.f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);

	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.f, 1.f);

	auto projection = glm::perspective(glm::radians(45.f), 1.f * static_cast<float>(m_screenWidth / m_screenHeight), 0.1f, 160.f);
	glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;

	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.f, 0.f);

	auto view = glm::lookAt(cam.Position(), cam.Position() + cam.Looking(), glm::vec3(0.f, 1.f, 0.f));
	glm::vec4 w = glm::inverse(view) * ray_eye;
	glm::vec3 ray_wor = glm::vec3(w.x, w.y, w.z);

	ray_wor = glm::normalize(ray_wor);

	l.Move(cam.Position() + cam.Looking(), ray_wor);

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
	Bind(Line);
	Bind(Hexagon);
	Bind(Object<Cube>);

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
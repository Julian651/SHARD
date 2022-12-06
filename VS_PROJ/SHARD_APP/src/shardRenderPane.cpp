#include "shardRenderPane.h"

shardRenderPane::shardRenderPane(wxWindow* parent) : wxGLCanvas(parent)
{
   this->SetName("GLCanvas");
   wxGLContextAttrs attribs;
   attribs.PlatformDefaults().CoreProfile().OGLVersion(3, 3).EndList();
   m_context = new wxGLContext(this, nullptr, &attribs);
   SetCurrent(*m_context);
   if (int err = gl3wInit())
   {
      fprintf(stderr, "gl3wInit() error: %d\n", err);
      return;
   }

   if (!gl3wIsSupported(3, 3)) {
      fprintf(stderr, "OpenGL 3.2 not supported\n");
      return;
   }

   m_program = new ShaderProgram;
   m_program->Attach(
      compileShader("resources\\myshader.vert", eShaderType::VERTEX),
      compileShader("resources\\myfragment.frag", eShaderType::FRAGMENT));
   m_program->Link();

   GLsizei l, w, h;
   l = 50;
   w = 50;
   h = 50;
   GLint vertices[] = {
      // front
      -1 * w, -1 * h,  1 * l,
       1 * w, -1 * h,  1 * l,
       1 * w,  1 * h,  1 * l,
      -1 * w,  1 * h,  1 * l,
      // back
      -1 * w, -1 * h, -1 * l,
       1 * w, -1 * h, -1 * l,
       1 * w,  1 * h, -1 * l,
      -1 * w,  1 * h, -1 * l
   };
   GLuint indices[] = {
      0, 1,
      1, 2,
      2, 3,
      3, 0,

      1, 5,
      5, 6,
      6, 2,
      2, 1,

      7, 6,
      6, 5,
      5, 4,
      4, 7,

      4, 0,
      0, 3,
      3, 7,
      7, 4,

      4, 5,
      5, 1,
      1, 0,
      0, 4,

      3, 2,
      2, 6,
      6, 7,
      7, 3
   };

   glGenVertexArrays(1, &m_VAO);
   glGenBuffers(1, &m_VBO);
   glGenBuffers(1, &m_EBO);

   glBindVertexArray(m_VAO);
   glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(0);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

shardRenderPane::~shardRenderPane()
{
   delete m_context;
   delete m_program;

   glDeleteVertexArrays(1, &m_VAO);
   glDeleteBuffers(1, &m_VBO);
   glDeleteBuffers(1, &m_EBO);
}

wxBEGIN_EVENT_TABLE(shardRenderPane, wxGLCanvas)
   EVT_PAINT(shardRenderPane::Render)
wxEND_EVENT_TABLE()
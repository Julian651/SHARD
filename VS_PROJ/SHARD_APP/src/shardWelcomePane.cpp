#include "shardWelcomePane.h"
#include <wx/dcclient.h>

shardWelcomePane::shardWelcomePane(wxWindow* parent) : shardRenderPane(parent)
{
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glEnable(GL_DEPTH_TEST);
   glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void shardWelcomePane::Render(wxPaintEvent& evt)
{
   //wxGLCanvas::SetCurrent(*shardRenderPane::m_context);
   wxPaintDC(this);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Do the magic

   // First render the scene box
   GLint size;

   glUseProgram(m_program->ID);
   glBindVertexArray(m_VAO);
   glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
   glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

   auto model = glm::translate(glm::mat4(1.f), glm::vec3(0.f));
   auto view = glm::lookAt(glm::vec3(0.f, 0.f, 100.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
   auto projection = glm::perspective(glm::radians(45.f), 1.f * m_width / m_height, 0.1f, 160.f);
   auto mvp = projection * view * model;
   glUniformMatrix4fv(m_program->GetUniform("m_transform"), 1, GL_FALSE, glm::value_ptr(mvp));

   glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 0, (GLvoid*)0);
   glDrawElements(GL_LINES, size / sizeof(GLuint), GL_UNSIGNED_INT, 0);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   SwapBuffers();
}

void shardWelcomePane::onResize(wxSizeEvent& evt)
{
   GLfloat width = static_cast<GLfloat>(evt.GetSize().x);
   GLfloat height = static_cast<GLfloat>(evt.GetSize().y);
   glViewport(0, 0, width, height);
   m_width = width;
   m_height = height;
}

wxBEGIN_EVENT_TABLE(shardWelcomePane, shardRenderPane)
EVT_SIZE(shardWelcomePane::onResize)
wxEND_EVENT_TABLE()

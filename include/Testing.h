#pragma once

#include <RenderUtilities.h>

// ====================================================================================================================

class SphereObject : public Object<SphereObject>
{
private:

   friend Object;

   static void _init()
   {
      glBindVertexArray(0);

      glGenBuffers(1, &m_VBO);
      glGenBuffers(1, &m_EBO);

      BlenderData("resources\\sphere.obj", vertices, indices);

      glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

      fprintf(stderr, "Sphere init\n");
   }

   static void _destroy()
   {
      glDeleteBuffers(1, &m_VBO);
      glDeleteBuffers(1, &m_EBO);

      fprintf(stderr, "Sphere destroyed\n");
   }

   static void _declare_attrib_pointers()
   {
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
      glEnableVertexAttribArray(0);
   }

   static void _draw()
   {
      glDrawElements(GL_TRIANGLES, indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, 0);
   }

public:
};

// ====================================================================================================================



// ====================================================================================================================

class SphereScene : public Scene<SphereScene>
{
private:

   friend Scene;

   static void _init()
   {

   }

   static void _destroy()
   {

   }

   static void _render()
   {

   }

public:


};
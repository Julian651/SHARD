#include <Objects.h>

// ======================================= HEXAGON OBJECT =========================================

Hexagon::Hexagon(glm::vec3 pos)
{
   m_pos = pos;
}

void Hexagon::GetFace(int face)
{
   if (face < 0 || face > 6)
   {
      return;
   }

   fprintf(stderr, "FACE %d\n", face);

   int a = 0;
   int b = 6 * 3;
   int c = 11 * 3;
   int d = 5 * 3;
   fprintf(stderr, "(%.2f %.2f %.2f)\n", vertices[a] + m_pos.x, vertices[a + 1] + m_pos.y, vertices[a + 2] + m_pos.z);
   fprintf(stderr, "(%.2f %.2f %.2f)\n", vertices[b] + m_pos.x, vertices[b + 1] + m_pos.y, vertices[b + 2] + m_pos.z);
   fprintf(stderr, "(%.2f %.2f %.2f)\n", vertices[c] + m_pos.x, vertices[c + 1] + m_pos.y, vertices[c + 2] + m_pos.z);
   fprintf(stderr, "(%.2f %.2f %.2f)\n", vertices[d] + m_pos.x, vertices[d + 1] + m_pos.y, vertices[d + 2] + m_pos.z);
}

void Hexagon::_init()
{
   glBindVertexArray(0);

   glGenBuffers(1, &m_VBO);
   glGenBuffers(1, &m_EBO);


   float w = 150.f / 150.f;
   float h = 57 / 150.f;
   float d = (75 * sqrtf(3.f)) / 150.f;

   vertices = {
      w, -h, 0,
      w / 2.f, -h, d,
      -w / 2.f, -h, d,
      -w, -h, 0,
      -w / 2.f, -h, -d,
      w / 2.f, -h, -d,

      w, h, 0,
      w / 2.f, h, d,
      -w / 2.f, h, d,
      -w, h, 0,
      -w / 2.f, h, -d,
      w / 2.f, h, -d
   };

   indices = {
      0,1,1,2,2,3,3,4,4,5,5,0,
      6,7,7,8,8,9,9,10,10,11,11,6,
      0,6,1,7,2,8,3,9,4,10,5,11
   };

   glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
   glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   fprintf(stderr, "Hexagon init\n");
}

void Hexagon::_destroy()
{
   glDeleteBuffers(1, &m_VBO);
   glDeleteBuffers(1, &m_EBO);

   fprintf(stderr, "Hexagon destroyed\n");
}

void Hexagon::_declare_attrib_pointers()
{
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
   glEnableVertexAttribArray(0);
}

void Hexagon::_draw()
{
   glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
}

glm::mat4 Hexagon::Model()
{
   return Object::Model();
}

// ======================================= CUBE OBJECT ============================================

Cube::Cube(int size) : m_size(size)
{

}

void Cube::_init()
{
   glBindVertexArray(0);

   glGenBuffers(1, &m_VBO);
   glGenBuffers(1, &m_EBO);


   // define data here
   GLint vertices[] = {
      // front
      -1, -1,  1,
       1, -1,  1,
       1,  1,  1,
      -1,  1,  1,
      // back
      -1, -1, -1,
       1, -1, -1,
       1,  1, -1,
      -1,  1, -1
   };
   GLuint indices[] = {
      0,1,1,2,2,3,3,0,
      1,5,5,6,6,2,2,1,
      7,6,6,5,5,4,4,7,
      4,0,0,3,3,7,7,4,
      4,5,5,1,1,0,0,4,
      3,2,2,6,6,7,7,3
   };

   glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   fprintf(stderr, "Cube init\n");
}

void Cube::_destroy()
{
   glDeleteBuffers(1, &m_VBO);
   glDeleteBuffers(1, &m_EBO);

   fprintf(stderr, "Cube destroyed\n");
}

void Cube::_declare_attrib_pointers()
{
   glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 0, (GLvoid*)0);
   glEnableVertexAttribArray(0);
}

void Cube::_draw()
{
   GLint size;
   glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
   glDrawElements(GL_LINES, size / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}

glm::mat4 Cube::Model()
{
   float fsize = static_cast<float>(m_size);
   auto translate = glm::translate(glm::mat4(1.f), m_pos);
   return glm::scale(translate, glm::vec3(fsize, fsize, fsize));
}

//========================================= LINE OBJECT ===========================================

Line::Line(glm::vec3 pos, glm::vec3 rot)
{
   m_rot = rot;
   m_pos = pos;
}

void Line::Move(glm::vec3 pos, glm::vec3 rot)
{
   m_pos = pos;
   m_rot = rot;
}

void Line::_init()
{
   glBindVertexArray(0);

   glGenBuffers(1, &m_VBO);
   glGenBuffers(1, &m_EBO);


   // define data here
   GLint vertices[] = {
      0, 0, -1,
      0, 0, 1
   };
   GLuint indices[] = {
      0, 1
   };

   glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   fprintf(stderr, "Line init\n");
}

void Line::_destroy()
{
   glDeleteBuffers(1, &m_VBO);
   glDeleteBuffers(1, &m_EBO);

   fprintf(stderr, "Line destroyed\n");
}

void Line::_declare_attrib_pointers()
{
   glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 0, (GLvoid*)0);
   glEnableVertexAttribArray(0);
}

void Line::_draw()
{
   GLint size;
   glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
   glDrawElements(GL_LINES, size / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}

glm::mat4 Line::Model()
{
   auto translate = glm::translate(glm::mat4(1.f), m_pos);
   auto rotate = glm::rotate(translate, m_rot.x, glm::vec3(0.f, -1.f, 0.f));
   auto rotate2 = glm::rotate(rotate, m_rot.y, glm::vec3(1.f, 0.f, 0.f));
   auto rotate3 = glm::rotate(rotate2, m_rot.z, glm::vec3(0.f, 0.f, 0.5f));
   return rotate3;
}
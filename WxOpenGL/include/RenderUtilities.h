#pragma once

#include <GL/gl3w.h>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_double4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <fstream>

// ====================================================================================================================

enum class eShaderType
{
   VERTEX, FRAGMENT
};

inline GLuint compileShader(const char* filename, eShaderType type)
{
   GLchar* buffer;

   std::ifstream infile;
   infile.open(filename, std::ios::in | std::ios::binary | std::ios::ate);

   if (!infile.is_open())
   {
      fprintf(stderr, "error\n");
      return -1;
   }
   infile.seekg(0, std::ios_base::end);
   int len = (int)infile.tellg();
   infile.seekg(0, std::ios_base::beg);

   buffer = new char[len + 1];
   infile.read(buffer, len);
   buffer[len] = '\0';

   infile.close();

   GLuint shader;
   GLint success;
   GLchar infoLog[512];
   if (type == eShaderType::VERTEX)
   {
      // vertex shader
      shader = glCreateShader(GL_VERTEX_SHADER);
   }
   else
   {
      // fragment shader
      shader = glCreateShader(GL_FRAGMENT_SHADER);
   }
   glShaderSource(shader, 1, &buffer, NULL);
   glCompileShader(shader);
   // check for shader compile errors
   glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      for (int i = 0; i < len + 1; i++)
      {
         fprintf(stderr, "%c", buffer[i]);
      }
      fprintf(stderr, "\n");
      glGetShaderInfoLog(shader, 512, NULL, infoLog);
      fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s", infoLog);
   }

   delete[] buffer;
   return shader;
}

struct ShaderProgram
{
   const GLint ID;

   inline ShaderProgram() : ID(glCreateProgram()) {}

   inline ~ShaderProgram()
   {
      glDeleteProgram(ID);
   }

   inline void Attach(GLuint shader, eShaderType type)
   {
      if (type == eShaderType::VERTEX)
      {
         glAttachShader(ID, shader);
      }
      else
      {
         glAttachShader(ID, shader);
      }
   }

   inline void Attach(GLuint vertexShader, GLuint fragmentShader)
   {
      glAttachShader(ID, vertexShader);
      glAttachShader(ID, fragmentShader);
   }

   GLint inline GetAttrib(const char* attribName)
   {
      GLint attrib = glGetAttribLocation(ID, attribName);
      if (attrib == -1)
      {
         fprintf(stderr, "Could not bind attribute %s\n", attribName);
         return -1;
      }
      return attrib;
   }

   inline GLint GetUniform(const char* uniformName)
   {
      GLint uniform = glGetUniformLocation(ID, uniformName);
      if (uniform == -1)
      {
         fprintf(stderr, "Could not bind uniform %s\n", uniformName);
         return -1;
      }
      return uniform;
   }

   inline void Link()
   {
      GLint success;
      GLchar infoLog[512];
      glLinkProgram(ID);
      // check for linking errors
      glGetProgramiv(ID, GL_LINK_STATUS, &success);
      if (!success)
      {
         glGetProgramInfoLog(ID, 512, NULL, infoLog);
         fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n %s", infoLog);
      }
   }
};

// ====================================================================================================================

class Camera
{
private:

   glm::vec3 m_pos;
   glm::vec3 m_look;

public:

   Camera(glm::vec3 pos = glm::vec3(0.f), glm::vec3 look = glm::vec3(0.f, 0.f, -1.f)) : m_pos(pos), m_look(look) {}
   glm::vec3 Position() const { return m_pos; }
   glm::vec3 Looking() const { return m_look; }
   void Move(glm::vec3 pos) { m_pos = pos; }
   void LookAt(glm::vec3 look) { m_look = look; }
};

// ====================================================================================================================

inline void BlenderData(const char* fileName, std::vector<GLfloat>& outVertices, std::vector<GLuint>& outIndices)
{
   std::vector<GLfloat> vertices;
   std::vector<GLfloat> uvs;
   std::vector<GLfloat> normals;
   std::vector<GLuint> vertIndices, uvIndices, normalIndices;

   std::ifstream infile;
   infile.open(fileName, std::ios::in);

   if (!infile.is_open())
   {
      fprintf(stderr, "error\n");
      return;
   }
   infile.seekg(0, std::ios::beg);
   while (infile.peek() != EOF)
   {
      char ch;
      infile.read(&ch, 1);
      if (ch == 'v')
      {
         GLfloat x, y, z;
         infile.read(&ch, 1);
         if (ch == ' ')
         {
            infile >> x >> y >> z;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
         }
         else if (ch == 'n')
         {
            infile >> x >> y >> z;
            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
         }
         else if (ch == 't')
         {
            infile >> x >> y;
            uvs.push_back(x);
            uvs.push_back(y);
         }
      }
      else if (ch == 'f')
      {
         infile.read(&ch, 1); // space
         while (ch != '\n')
         {
            GLuint v1, v2, v3, n1, n2, n3, t1, t2, t3;
            infile >> v1;
            infile.read(&ch, 1); // slash
            infile >> t1;
            infile.read(&ch, 1); // slash
            infile >> n1;
            infile.read(&ch, 1); // space
            infile >> v2;
            infile.read(&ch, 1); // slash
            infile >> t2;
            infile.read(&ch, 1); // slash
            infile >> n2;
            infile.read(&ch, 1); // space
            infile >> v3;
            infile.read(&ch, 1); // slash
            infile >> t3;
            infile.read(&ch, 1); // slash
            infile >> n3;
            infile.read(&ch, 1); // new line
            vertIndices.push_back(v1 - 1);
            vertIndices.push_back(v2 - 1);
            vertIndices.push_back(v3 - 1);
            normalIndices.push_back(n1 - 1);
            normalIndices.push_back(n2 - 1);
            normalIndices.push_back(n3 - 1);
            uvIndices.push_back(t1 - 1);
            uvIndices.push_back(t2 - 1);
            uvIndices.push_back(t3 - 1);
         }
      }
   }
   infile.close();

   outVertices = vertices;
   outIndices = vertIndices;
}

template <class T>
class Object
{
private:

   static bool m_initialized;

protected:

   static GLuint m_VBO;
   static GLuint m_EBO;

   static std::vector<GLfloat> vertices;
   static std::vector<GLuint> indices;

   glm::vec3 m_pos = glm::vec3(0.f);

public:

   static inline void Initialize()
   {
      if (m_initialized)
      {
         return;
      }

      T::_init();

      m_initialized = true;
   }

   static inline void Destroy()
   {
      if (!m_initialized)
      {
         return;
      }

      T::_destroy();

      m_initialized = false;
   }

   static inline GLuint VBO()
   {
      return m_VBO;
   }

   static inline GLuint EBO()
   {
      return m_EBO;
   }

   static inline void DeclareAttribPointers()
   {
      if (!m_initialized)
      {
         return;
      }

      T::_declare_attrib_pointers();
   }

   static inline void Draw()
   {
      if (!m_initialized)
      {
         return;
      }

      T::_draw();
   }

   inline void Render(ShaderProgram& prog, glm::mat4 projection, glm::mat4 view)
   {
      glUseProgram(prog.ID);
      glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

      auto model = Model();
      auto mvp = projection * view * model;
      glUniformMatrix4fv(prog.GetUniform("m_transform"), 1, GL_FALSE, glm::value_ptr(mvp));

      DeclareAttribPointers();
      Draw();
   }

   virtual inline glm::mat4 Model()
   {
      return glm::translate(glm::mat4(1.f), m_pos);
   }
};

template <class T>
bool Object<T>::m_initialized = false;

template <class T>
GLuint Object<T>::m_VBO = 0;

template <class T>
GLuint Object<T>::m_EBO = 0;

template <class T>
std::vector<GLfloat> Object<T>::vertices = std::vector<GLfloat>();

template <class T>
std::vector<GLuint> Object<T>::indices = std::vector<GLuint>();

// ====================================================================================================================

template <class T>
class Scene
{
private:

   static bool m_initialized;
   static GLuint m_VAO;

protected:



public:

   static inline void Initialize()
   {
      if (m_initialized)
      {
         return;
      }

      T::_init();

      m_initialized = true;
   }

   static inline void Destroy()
   {
      if (!m_initialized)
      {
         return;
      }

      T::_destroy();

      m_initialized = false;
   }

   static inline void Render()
   {
      if (!m_initialized)
      {
         return;
      }

      T::_render();
   }

};

template <class T>
bool Scene<T>::m_initialized = false;

template <class T>
GLuint Scene<T>::m_VAO = 0;
// ====================================================================================================================
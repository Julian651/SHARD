#pragma once

#ifdef GL_VERSION_1_1
#undef GL_VERSION_1_1
#endif

#include <GL/gl3w.h>

#include <glm/vec3.hpp>
#include <glm/ext/matrix_double4x4.hpp>
#include <memory>

#include <vector>

// ===============================================================================================================

class ShaderProgram
{
private:

    const GLint m_ID;

    GLuint compileShader(const char* filename, bool vert);

public:

    ShaderProgram(const char* vertShaderFilename, const char* fragShaderFilename);

    ~ShaderProgram();

    GLint GetUniform(const char* uniformName);

    GLint ID();

};

// ===============================================================================================================

class Scene
{
private:

    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;

protected:

    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 color;
    };

    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;

    std::unique_ptr<ShaderProgram> m_program;

    virtual void _render();

public:
    
    Scene();

    ~Scene();

    void Render(glm::mat4 projection, glm::mat4 view);

    GLuint VAO();
    GLuint VBO();
    GLuint EBO();
};

class HexaScene : public Scene
{
private:

    struct Hexagon
    {
        GLuint vbo_offset;
        glm::vec3 position;
    };

    int hexa_id = 0;
    std::vector<std::unique_ptr<Hexagon>> hexes;

    Vertex m_highlight_vertices[4];
    GLuint m_hightlight_indices[8];
    GLuint m_VAO_highlight;
    GLuint m_VBO_highlight;
    GLuint m_EBO_highlight;

    bool temp_hex = false;
    float temp_hex_angle = 0.f;
    glm::vec3 temp_hex_pos;
    Vertex m_temphex_vertices[12];
    GLuint m_temphex_indices[36];
    GLuint m_VAO_temphex;
    GLuint m_VBO_temphex;
    GLuint m_EBO_temphex;

    void InsertTempHex(glm::vec3 position, glm::vec3 color);

public:

    HexaScene();
    
    ~HexaScene();

    void _render() override;

    int InsertHexagon(glm::vec3 position, glm::vec3 color, int rotation, float rotate);

    bool RotateTempHex(bool forwardScroll, float rotate);

    bool ConfirmTempHex();

    void CheckIntersections(glm::mat4 projection, glm::mat4 model, glm::vec3 camPos, int width, int height, int mouse_x, int mouse_y);
};

// ===============================================================================================================

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

// ===============================================================================================================

glm::vec2 WorldToScreen(glm::mat4 projection, glm::mat4 model, glm::vec3 worldcoords, int screenWidth, int screenHeight);

glm::vec3 ScreenToWorld(glm::mat4 projection, glm::mat4 model, glm::vec3 worldcoords, int screenWidth, int screenHeight);
#include <GLLayer.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

void Scene::_render()
{
}

Scene::Scene()
{
    m_program = std::make_unique<ShaderProgram>("resources\\myshader.vert", "resources\\myfragment.frag");

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
}

Scene::~Scene()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void Scene::Render(glm::mat4 projection, glm::mat4 view)
{
    glUseProgram(m_program->ID());

    auto mvp = projection * view;
    glUniformMatrix4fv(m_program->GetUniform("m_transform"), 1, GL_FALSE, glm::value_ptr(mvp));

    glBindVertexArray(m_VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_LINES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, (const void*)0);
    _render();
}

GLuint Scene::VAO()
{
    return m_VAO;
}

GLuint Scene::VBO()
{
    return m_VBO;
}

GLuint Scene::EBO()
{
    return m_EBO;
}

GLuint ShaderProgram::compileShader(const char* filename, bool vert)
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
    if (vert)
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

ShaderProgram::ShaderProgram(const char* vertShaderFilename, const char* fragShaderFilename) : m_ID(glCreateProgram())
{
    glAttachShader(m_ID, compileShader(vertShaderFilename, true));
    glAttachShader(m_ID, compileShader(fragShaderFilename, false));

    GLint success;
    GLchar infoLog[512];
    glLinkProgram(m_ID);
    // check for linking errors
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n %s", infoLog);
    }
}

ShaderProgram::~ShaderProgram()
{
    glDeleteShader(m_ID);
}

GLint ShaderProgram::GetUniform(const char* uniformName)
{
    GLint uniform = glGetUniformLocation(m_ID, uniformName);
    if (uniform == -1)
    {
        fprintf(stderr, "Could not bind uniform %s\n", uniformName);
        return -1;
    }
    return uniform;
}

GLint ShaderProgram::ID()
{
    return m_ID;
}

HexaScene::HexaScene()
{
    glGenVertexArrays(1, &m_VAO_highlight);
    glGenBuffers(1, &m_VBO_highlight);
    glGenBuffers(1, &m_EBO_highlight);
    glGenVertexArrays(1, &m_VAO_temphex);
    glGenBuffers(1, &m_VBO_temphex);
    glGenBuffers(1, &m_EBO_temphex);

    float sqrt3 = sqrtf(3.f);
    int hexid1 = InsertHexagon(glm::vec3(0.f, 0.f, 0.f),            glm::vec3(1.f, 1.f, 0.f), 0, 0.f);
    int hexid2 = InsertHexagon(glm::vec3(1.5f, 0.f, sqrt3 / 2.f),   glm::vec3(1.f, 1.f, 0.f), 0, 0.f);
    int hexid3 = InsertHexagon(glm::vec3(0.f, 0.f, sqrt3),          glm::vec3(1.f, 1.f, 0.f), 0, 0.f);
    int hexid4 = InsertHexagon(glm::vec3(0.f, 0.f, -sqrt3),         glm::vec3(1.f, 1.f, 0.f), 0, 0.f);
    int hexid5 = InsertHexagon(glm::vec3(1.5f, 0.f, -sqrt3 / 2.f),  glm::vec3(1.f, 1.f, 0.f), 0, 0.f);
    int hexid6 = InsertHexagon(glm::vec3(-1.5f, 0.f, -sqrt3 / 2.f), glm::vec3(1.f, 1.f, 0.f), 0, 0.f);
    int hexid7 = InsertHexagon(glm::vec3(-1.5f, 0.f, sqrt3 / 2.f),  glm::vec3(1.f, 1.f, 0.f), 0, 0.f);

    m_hightlight_indices[0] = 0;
    m_hightlight_indices[1] = 1;
    m_hightlight_indices[2] = 1;
    m_hightlight_indices[3] = 2;
    m_hightlight_indices[4] = 2;
    m_hightlight_indices[5] = 3;
    m_hightlight_indices[6] = 3;
    m_hightlight_indices[7] = 0;
    //srand(time(NULL));
    //
    ////float sqrt3 = sqrtf(3.f);
    //bool xswap = true;
    //for (int i = 0; i < 100; i++)
    //{
    //    float x = (float)i;
    //    x *= 1.5f;
    //    for (int j = 0; j < 100; j++)
    //    {
    //        float y = (float)j;
    //        xswap ? y *= sqrt3 : y = y * sqrt3 + sqrt3 / 2.f;
    //
    //        float r = (float)rand() / (float)RAND_MAX;
    //        float g = (float)rand() / (float)RAND_MAX;
    //        float b = (float)rand() / (float)RAND_MAX;
    //
    //        InsertHexagon(glm::vec3(x, 0.f, y), glm::vec3(r, g, b), 0.f);
    //    }
    //    xswap ? xswap = false : xswap = true;
    //}
}

HexaScene::~HexaScene()
{
    glDeleteVertexArrays(1, &m_VAO_highlight);
    glDeleteBuffers(1, &m_VBO_highlight);
    glDeleteBuffers(1, &m_EBO_highlight);
    glDeleteVertexArrays(1, &m_VAO_temphex);
    glDeleteBuffers(1, &m_VBO_temphex);
    glDeleteBuffers(1, &m_EBO_temphex);
}

void HexaScene::_render()
{
    glBindVertexArray(m_VAO_highlight);
    glDrawElements(GL_LINES, sizeof(m_hightlight_indices), GL_UNSIGNED_INT, (const void*)0);

    if (temp_hex)
    {
        glBindVertexArray(m_VAO_temphex);
        glDrawElements(GL_LINES, sizeof(m_temphex_indices), GL_UNSIGNED_INT, (const void*)0);
    }
}

void rotateHexAroundSomePoint(glm::vec3 vertices[12], float angle, glm::vec3 rotationCenter, glm::vec3 axis)
{
    glm::mat4 translationMatrix = glm::translate(glm::identity<glm::mat4>(), -rotationCenter);
    glm::mat4 rotationMatrix = glm::rotate(glm::identity<glm::mat4>(), glm::radians(angle), axis);
    glm::mat4 reverseTranslationMatrix = glm::translate(glm::identity<glm::mat4>(), rotationCenter);

    for (size_t i = 0; i < 12; i++) {

        vertices[i] = glm::vec3(
            reverseTranslationMatrix * rotationMatrix * translationMatrix * glm::vec4(vertices[i], 1.0f));
    }
}

void rotatePointAroundSomePoint(glm::vec3& point, float angle, glm::vec3 rotationCenter, glm::vec3 axis)
{
    glm::mat4 translationMatrix = glm::translate(glm::identity<glm::mat4>(), -rotationCenter);
    glm::mat4 rotationMatrix = glm::rotate(glm::identity<glm::mat4>(), glm::radians(angle), axis);
    glm::mat4 reverseTranslationMatrix = glm::translate(glm::identity<glm::mat4>(), rotationCenter);
    point = glm::vec3(reverseTranslationMatrix * rotationMatrix * translationMatrix * glm::vec4(point, 1.0f));
}

int HexaScene::InsertHexagon(glm::vec3 position, glm::vec3 color, int rotation, float rotate)
{
    glm::vec4 dimensions = glm::vec4(1.f, 57.f / 150.f, (75.f * sqrtf(3.f)) / 150.f, 1.f);

    glm::vec4 pos = glm::vec4(position.x, position.y, position.z, 1.f);

    std::unique_ptr<Hexagon> hex = std::make_unique<Hexagon>();
    hex->vbo_offset = m_vertices.size();
    hex->position = position;
    hexes.push_back(std::move(hex));
    hexa_id++;

    int os = m_vertices.size();

    float w = dimensions.x;
    float h = dimensions.y;
    float d = dimensions.z;

    float x = pos.x;
    float y = pos.y;
    float z = pos.z;


    glm::vec3 hexVertices[12] = {
        glm::vec3(w + x, -h + y, 0.f + z),
        glm::vec3(w / 2.f + x, -h + y, d + z),
        glm::vec3(-w / 2.f + x, -h + y, d + z),
        glm::vec3(-w + x, -h + y, 0.f + z),
        glm::vec3(-w / 2.f + x, -h + y, -d + z),
        glm::vec3(w / 2.f + x, -h + y, -d + z),
        glm::vec3(w + x, h + y, 0.f + z),
        glm::vec3(w / 2.f + x, h + y, d + z),
        glm::vec3(-w / 2.f + x, h + y, d + z),
        glm::vec3(-w + x, h + y, 0 + z),
        glm::vec3(-w / 2.f + x, h + y, -d + z),
        glm::vec3(w / 2.f + x, h + y, -d + z)
    };

    if (rotate == 0.f) rotation = 0;

    switch (rotation)
    {
    case 0:
        // No rotation
        break;
    case 1:
        // Rotate back
        if (rotate > 0)
            rotateHexAroundSomePoint(hexVertices, rotate, glm::vec3(x, y - h, z - d), glm::vec3(1.f, 0.f, 0.f));
        else if (rotate < 0)
            rotateHexAroundSomePoint(hexVertices, rotate, glm::vec3(x, y + h, z - d), glm::vec3(1.f, 0.f, 0.f));
        rotatePointAroundSomePoint(hexes[hexes.size()-1]->position, 30.f, glm::vec3(x, y - h, z - d), glm::vec3(1.f, 0.f, 0.f));
        break;
    case 2:
        // Rotate back left
        rotateHexAroundSomePoint(hexVertices, rotate, glm::vec3(x, y - h, z - d), glm::vec3(1.f, 0.f, 0.f));
        rotateHexAroundSomePoint(hexVertices, 60, glm::vec3(x, y, z), glm::vec3(0.f, 1.f, 0.f));
        break;
    case 3:
        // Rotate front left
        rotateHexAroundSomePoint(hexVertices, rotate, glm::vec3(x, y - h, z - d), glm::vec3(1.f, 0.f, 0.f));
        rotateHexAroundSomePoint(hexVertices, 120, glm::vec3(x, y, z), glm::vec3(0.f, 1.f, 0.f));
        break;
    case 4:
        // Rotate front
        rotateHexAroundSomePoint(hexVertices, rotate, glm::vec3(x, y - h, z - d), glm::vec3(1.f, 0.f, 0.f));
        rotateHexAroundSomePoint(hexVertices, 180, glm::vec3(x, y, z), glm::vec3(0.f, 1.f, 0.f));
        break;
    case 5:
        // Rotate front right
        rotateHexAroundSomePoint(hexVertices, rotate, glm::vec3(x, y - h, z - d), glm::vec3(1.f, 0.f, 0.f));
        rotateHexAroundSomePoint(hexVertices, 240, glm::vec3(x, y, z), glm::vec3(0.f, 1.f, 0.f));
        break;
    case 6:
        // Rotate back right
        rotateHexAroundSomePoint(hexVertices, rotate, glm::vec3(x, y - h, z - d), glm::vec3(1.f, 0.f, 0.f));
        rotateHexAroundSomePoint(hexVertices, 300, glm::vec3(x, y, z), glm::vec3(0.f, 1.f, 0.f));
        break;
    }



    {
        m_vertices.push_back(Vertex{ hexVertices[0], color });
        m_vertices.push_back(Vertex{ hexVertices[1], color });
        m_vertices.push_back(Vertex{ hexVertices[2], color });
        m_vertices.push_back(Vertex{ hexVertices[3], color });
        m_vertices.push_back(Vertex{ hexVertices[4], color });
        m_vertices.push_back(Vertex{ hexVertices[5], color });

        m_vertices.push_back(Vertex{ hexVertices[6], color });
        m_vertices.push_back(Vertex{ hexVertices[7], color });
        m_vertices.push_back(Vertex{ hexVertices[8], color });
        m_vertices.push_back(Vertex{ hexVertices[9], color });
        m_vertices.push_back(Vertex{ hexVertices[10], color });
        m_vertices.push_back(Vertex{ hexVertices[11], color });
    }

    {
        m_indices.push_back(0 + os); m_indices.push_back(1 + os); m_indices.push_back(1 + os); m_indices.push_back(2 + os);
        m_indices.push_back(2 + os); m_indices.push_back(3 + os); m_indices.push_back(3 + os); m_indices.push_back(4 + os);
        m_indices.push_back(4 + os); m_indices.push_back(5 + os); m_indices.push_back(5 + os); m_indices.push_back(0 + os);
        m_indices.push_back(6 + os); m_indices.push_back(7 + os); m_indices.push_back(7 + os); m_indices.push_back(8 + os);
        m_indices.push_back(8 + os); m_indices.push_back(9 + os); m_indices.push_back(9 + os); m_indices.push_back(10 + os);
        m_indices.push_back(10 + os); m_indices.push_back(11 + os); m_indices.push_back(11 + os); m_indices.push_back(6 + os);
        m_indices.push_back(0 + os); m_indices.push_back(6 + os); m_indices.push_back(1 + os); m_indices.push_back(7 + os);
        m_indices.push_back(2 + os); m_indices.push_back(8 + os); m_indices.push_back(3 + os); m_indices.push_back(9 + os);
        m_indices.push_back(4 + os); m_indices.push_back(10 + os); m_indices.push_back(5 + os); m_indices.push_back(11 + os);
    }

    glBindVertexArray(VAO());

    glBindBuffer(GL_ARRAY_BUFFER, VBO());
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const void*)offsetof(Vertex, color));

    return hexa_id - 1;
}

bool HexaScene::RotateTempHex(bool forwardScroll, float rotate)
{
    if (!temp_hex) return false;

    rotate *= forwardScroll ? 1.f : -1.f;
    if (temp_hex_angle >= 360.f || temp_hex_angle <= -360.f)
    {
        temp_hex_angle = 0.f;
        return false;
    }

    glm::vec3 vertices[] =
    {
        m_temphex_vertices[0].position, m_temphex_vertices[1].position, m_temphex_vertices[2].position,
        m_temphex_vertices[3].position, m_temphex_vertices[4].position, m_temphex_vertices[5].position,
        m_temphex_vertices[6].position, m_temphex_vertices[7].position, m_temphex_vertices[8].position,
        m_temphex_vertices[9].position, m_temphex_vertices[10].position, m_temphex_vertices[11].position
    };

    glm::vec4 dimensions = glm::vec4(1.f, 57.f / 150.f, (75.f * sqrtf(3.f)) / 150.f, 1.f);

    float w = dimensions.x;
    float h = dimensions.y;
    float d = dimensions.z;

    float x = temp_hex_pos.x;
    float y = temp_hex_pos.y;
    float z = temp_hex_pos.z;
    if (temp_hex_angle == 0)
    {
        if (forwardScroll)
        {
            rotateHexAroundSomePoint(vertices, rotate, glm::vec3(x, y - h, z - d), glm::vec3(1.f, 0.f, 0.f));
        }
        else
        {
            rotateHexAroundSomePoint(vertices, rotate, glm::vec3(x, y + h, z - d), glm::vec3(1.f, 0.f, 0.f));
        }
        temp_hex_angle += rotate;
    }
    else if (temp_hex_angle <= 0)
    {
        rotateHexAroundSomePoint(vertices, rotate, glm::vec3(x, y + h, z - d), glm::vec3(1.f, 0.f, 0.f));
        temp_hex_angle += rotate;
    }
    else if (temp_hex_angle >= 0)
    {
        rotateHexAroundSomePoint(vertices, rotate, glm::vec3(x, y - h, z - d), glm::vec3(1.f, 0.f, 0.f));
        temp_hex_angle += rotate;
    }
    
    m_temphex_vertices[0].position = vertices[0]; m_temphex_vertices[1].position  = vertices[1]; m_temphex_vertices[2].position  = vertices[2];
    m_temphex_vertices[3].position = vertices[3]; m_temphex_vertices[4].position  = vertices[4]; m_temphex_vertices[5].position  = vertices[5];
    m_temphex_vertices[6].position = vertices[6]; m_temphex_vertices[7].position  = vertices[7]; m_temphex_vertices[8].position  = vertices[8];
    m_temphex_vertices[9].position = vertices[9]; m_temphex_vertices[10].position = vertices[10]; m_temphex_vertices[11].position = vertices[11];


    glBindVertexArray(m_VAO_temphex);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_temphex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_temphex_vertices), m_temphex_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO_temphex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_temphex_indices), m_temphex_indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const void*)offsetof(Vertex, color));

    return true;
}

bool HexaScene::ConfirmTempHex()
{
    if (!temp_hex) return false;

    InsertHexagon(temp_hex_pos, glm::vec3(1.f, 1.f, 0.f), 1, temp_hex_angle);
    temp_hex = false;

    return true;
}

bool inBounds(glm::vec2 mouse, glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d)
{
    // lines go like
    // c__________b
    // |          |
    // |          |
    // |          |
    // |          |
    // d__________a
    // 
    // y - y1 = m(x - x1)
    // y - y1 = mx - mx1
    // y = mx - mx1 + y1
    // b = -mx1 + y1 = -(mx1 - y1)
    float m;
    float nb;
    m = (b.y - c.y) / (b.x - c.x);
    nb = -(m * b.x - b.y);
    glm::vec2 lineTop(m, nb);
    m = (a.y - d.y) / (a.x - d.x);
    nb = -(m * a.x - a.y);
    glm::vec2 lineBot(m, nb);
    m = (a.x - b.x) / (a.y - b.y);
    nb = -(m * a.y - a.x);
    glm::vec2 lineRit(m, nb);
    m = (c.x - d.x) / (c.y - d.y);
    nb = -(m * c.y - c.x);
    glm::vec2 lineLef(m, nb);

    bool belowTop = mouse.y > (lineTop.x * mouse.x + lineTop.y);
    bool aboveBot = mouse.y < (lineBot.x * mouse.x + lineBot.y);
    bool leftRit  = mouse.x < (lineRit.x * mouse.y + lineRit.y);
    bool rightLef = mouse.x > (lineLef.x * mouse.y + lineLef.y);
    
    return belowTop && aboveBot && leftRit && rightLef;
}

void HexaScene::InsertTempHex(glm::vec3 position, glm::vec3 color)
{
    temp_hex_angle = 0.f;
    glm::vec4 dimensions = glm::vec4(1.f, 57.f / 150.f, (75.f * sqrtf(3.f)) / 150.f, 1.f);

    temp_hex_pos = position;

    float w = dimensions.x;
    float h = dimensions.y;
    float d = dimensions.z;

    float x = position.x;
    float y = position.y;
    float z = position.z;


    glm::vec3 hexVertices[12] = {
        glm::vec3(w + x, -h + y, 0.f + z),
        glm::vec3(w / 2.f + x, -h + y, d + z),
        glm::vec3(-w / 2.f + x, -h + y, d + z),
        glm::vec3(-w + x, -h + y, 0.f + z),
        glm::vec3(-w / 2.f + x, -h + y, -d + z),
        glm::vec3(w / 2.f + x, -h + y, -d + z),
        glm::vec3(w + x, h + y, 0.f + z),
        glm::vec3(w / 2.f + x, h + y, d + z),
        glm::vec3(-w / 2.f + x, h + y, d + z),
        glm::vec3(-w + x, h + y, 0 + z),
        glm::vec3(-w / 2.f + x, h + y, -d + z),
        glm::vec3(w / 2.f + x, h + y, -d + z)
    };

    

    {
        m_temphex_vertices[0] = (Vertex{ hexVertices[0], color });
        m_temphex_vertices[1] = (Vertex{ hexVertices[1], color });
        m_temphex_vertices[2] = (Vertex{ hexVertices[2], color });
        m_temphex_vertices[3] = (Vertex{ hexVertices[3], color });
        m_temphex_vertices[4] = (Vertex{ hexVertices[4], color });
        m_temphex_vertices[5] = (Vertex{ hexVertices[5], color });

        m_temphex_vertices[6] = (Vertex{ hexVertices[6], color });
        m_temphex_vertices[7] = (Vertex{ hexVertices[7], color });
        m_temphex_vertices[8] = (Vertex{ hexVertices[8], color });
        m_temphex_vertices[9] = (Vertex{ hexVertices[9], color });
        m_temphex_vertices[10] = (Vertex{ hexVertices[10], color });
        m_temphex_vertices[11] = (Vertex{ hexVertices[11], color });
    }

    {
        m_temphex_indices[0] = 0;  m_temphex_indices[1] = 1;  m_temphex_indices[2] = 1;  m_temphex_indices[3] = 2;
        m_temphex_indices[4] = 2;  m_temphex_indices[5] = 3;  m_temphex_indices[6] = 3;  m_temphex_indices[7] = 4;
        m_temphex_indices[8] = 4;  m_temphex_indices[9] = 5;  m_temphex_indices[10] = 5;  m_temphex_indices[11] = 0;
        m_temphex_indices[12] = 6;  m_temphex_indices[13] = 7;  m_temphex_indices[14] = 7;  m_temphex_indices[15] = 8;
        m_temphex_indices[16] = 8;  m_temphex_indices[17] = 9;  m_temphex_indices[18] = 9;  m_temphex_indices[19] = 10;
        m_temphex_indices[20] = 10; m_temphex_indices[21] = 11; m_temphex_indices[22] = 11; m_temphex_indices[23] = 6;
        m_temphex_indices[24] = 0;  m_temphex_indices[25] = 6;  m_temphex_indices[26] = 1;  m_temphex_indices[27] = 7;
        m_temphex_indices[28] = 2;  m_temphex_indices[29] = 8;  m_temphex_indices[30] = 3;  m_temphex_indices[31] = 9;
        m_temphex_indices[32] = 4;  m_temphex_indices[33] = 10; m_temphex_indices[34] = 5;  m_temphex_indices[35] = 11;
    }

    glBindVertexArray(m_VAO_temphex);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_temphex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_temphex_vertices), m_temphex_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO_temphex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_temphex_indices), m_temphex_indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const void*)offsetof(Vertex, color));

    temp_hex = true;
    /*glm::vec3 b = pot_hits[closest_ind][0];
    glm::vec3 r = pot_hits[closest_ind][1];
    glm::vec3 s = pot_hits[closest_ind][2];
    glm::vec3 normal = glm::cross(r - b, s - b);
    float x = normal.x * 0.f + hex_min->position.x;
    float y = normal.y * sqrtf(3.f) + hex_min->position.y;
    float z = normal.z * 0.f + hex_min->position.z + sqrtf(3.f);
    pos = glm::vec3(x, y, z);
    InsertHexagon(pos, glm::vec3(1.f, 0.f, 0.f), 1, 30.f);*/
}

void HexaScene::CheckIntersections(glm::mat4 projection, glm::mat4 model, glm::vec3 camPos, int width, int height, int mouse_x, int mouse_y)
{
    float min = -INFINITY;
    Hexagon* hex_min = nullptr;
    std::vector<std::vector<glm::vec3>> pot_hits;
    int closest_ind = -1;
    uint8_t dir = 0;
    for (auto& hex : hexes)
    {
        int vbo_index = hex->vbo_offset;
        glm::vec2 mouse((float)mouse_x, (float)mouse_y);
        
        // face_front
        glm::vec2 a = WorldToScreen(projection, model, m_vertices[vbo_index + 1].position, width, height);
        glm::vec2 b = WorldToScreen(projection, model, m_vertices[vbo_index + 7].position, width, height);
        glm::vec2 c = WorldToScreen(projection, model, m_vertices[vbo_index + 8].position, width, height);
        glm::vec2 d = WorldToScreen(projection, model, m_vertices[vbo_index + 2].position, width, height);
        bool in_bounds = inBounds(mouse, a, b, c, d);
        if (in_bounds)
        {
            // add to list of potential new hits
            pot_hits.push_back({
                m_vertices[vbo_index + 1].position,
                m_vertices[vbo_index + 7].position,
                m_vertices[vbo_index + 8].position,
                m_vertices[vbo_index + 2].position
            });

            if ((m_vertices[vbo_index + 1].position - camPos).z > min)
            {
                min = (m_vertices[vbo_index + 1].position - camPos).z;
                closest_ind = pot_hits.size() - 1;
                hex_min = hex.get();
            }
            dir = 1;
        }

        // face_front_left
        a = WorldToScreen(projection, model, m_vertices[vbo_index + 2].position, width, height);
        b = WorldToScreen(projection, model, m_vertices[vbo_index + 8].position, width, height);
        c = WorldToScreen(projection, model, m_vertices[vbo_index + 9].position, width, height);
        d = WorldToScreen(projection, model, m_vertices[vbo_index + 3].position, width, height);
        in_bounds = inBounds(mouse, a, b, c, d);
        if (in_bounds)
        {
            // add to list of potential new hits
            pot_hits.push_back({
                m_vertices[vbo_index + 2].position,
                m_vertices[vbo_index + 8].position,
                m_vertices[vbo_index + 9].position,
                m_vertices[vbo_index + 3].position
                });

            if ((m_vertices[vbo_index + 2].position - camPos).z > min)
            {
                min = (m_vertices[vbo_index + 2].position - camPos).z;
                closest_ind = pot_hits.size() - 1;
                hex_min = hex.get();
            }
            dir = 2;
        }

        // face_front_right
        a = WorldToScreen(projection, model, m_vertices[vbo_index + 0].position, width, height);
        b = WorldToScreen(projection, model, m_vertices[vbo_index + 6].position, width, height);
        c = WorldToScreen(projection, model, m_vertices[vbo_index + 7].position, width, height);
        d = WorldToScreen(projection, model, m_vertices[vbo_index + 1].position, width, height);
        in_bounds = inBounds(mouse, a, b, c, d);
        if (in_bounds)
        {
            // add to list of potential new hits
            pot_hits.push_back({
                m_vertices[vbo_index + 0].position,
                m_vertices[vbo_index + 6].position,
                m_vertices[vbo_index + 7].position,
                m_vertices[vbo_index + 1].position
                });

            if ((m_vertices[vbo_index + 1].position - camPos).z > min)
            {
                min = (m_vertices[vbo_index + 1].position - camPos).z;
                closest_ind = pot_hits.size() - 1;
                hex_min = hex.get();
            }
            dir = 6;
        }

        // face_back_right
        a = WorldToScreen(projection, model, m_vertices[vbo_index + 5].position, width, height);
        b = WorldToScreen(projection, model, m_vertices[vbo_index + 11].position, width, height);
        c = WorldToScreen(projection, model, m_vertices[vbo_index + 6].position, width, height);
        d = WorldToScreen(projection, model, m_vertices[vbo_index + 0].position, width, height);
        in_bounds = inBounds(mouse, a, b, c, d);
        if (in_bounds)
        {
            // add to list of potential new hits
            pot_hits.push_back({
                m_vertices[vbo_index + 5].position,
                m_vertices[vbo_index + 11].position,
                m_vertices[vbo_index + 6].position,
                m_vertices[vbo_index + 0].position
                });

            if ((m_vertices[vbo_index + 5].position - camPos).z > min)
            {
                min = (m_vertices[vbo_index + 5].position - camPos).z;
                closest_ind = pot_hits.size() - 1;
                hex_min = hex.get();
            }
            dir = 5;
        }

        // face_back
        a = WorldToScreen(projection, model, m_vertices[vbo_index + 4].position, width, height);
        b = WorldToScreen(projection, model, m_vertices[vbo_index + 10].position, width, height);
        c = WorldToScreen(projection, model, m_vertices[vbo_index + 11].position, width, height);
        d = WorldToScreen(projection, model, m_vertices[vbo_index + 5].position, width, height);
        in_bounds = inBounds(mouse, a, b, c, d);
        if (in_bounds)
        {
            // add to list of potential new hits
            pot_hits.push_back({
                m_vertices[vbo_index + 4].position,
                m_vertices[vbo_index + 10].position,
                m_vertices[vbo_index + 11].position,
                m_vertices[vbo_index + 5].position
                });

            if ((m_vertices[vbo_index + 4].position - camPos).z > min)
            {
                min = (m_vertices[vbo_index + 4].position - camPos).z;
                closest_ind = pot_hits.size() - 1;
                hex_min = hex.get();
            }
            dir = 4;
        }

        // face_back_left
        a = WorldToScreen(projection, model, m_vertices[vbo_index + 3].position, width, height);
        b = WorldToScreen(projection, model, m_vertices[vbo_index + 9].position, width, height);
        c = WorldToScreen(projection, model, m_vertices[vbo_index + 10].position, width, height);
        d = WorldToScreen(projection, model, m_vertices[vbo_index + 4].position, width, height);
        in_bounds = inBounds(mouse, a, b, c, d);
        if (in_bounds)
        {
            // add to list of potential new hits
            pot_hits.push_back({
                m_vertices[vbo_index + 3].position,
                m_vertices[vbo_index + 9].position,
                m_vertices[vbo_index + 10].position,
                m_vertices[vbo_index + 4].position
                });

            if ((m_vertices[vbo_index + 3].position - camPos).z > min)
            {
                min = (m_vertices[vbo_index + 3].position - camPos).z;
                closest_ind = pot_hits.size() - 1;
                hex_min = hex.get();
            }
            dir = 3;
        }
    }
    if (closest_ind > -1)
    {
        // check if there is already a tile at the position

        // insert tile at relevant position

        glm::vec3 pos;

        switch (dir)
        {
        case 1: // front
        {
            // get the normal of the plane of the face
            // x remains the same
            // y and z change
            glm::vec3 b = pot_hits[closest_ind][0];
            glm::vec3 r = pot_hits[closest_ind][1];
            glm::vec3 s = pot_hits[closest_ind][2];
            glm::vec3 normal = glm::cross(r - b, s - b);
            float x = normal.x * 0.f + hex_min->position.x;
            float y = normal.y * 0.f + hex_min->position.y;
            float z = normal.z * 0.f + hex_min->position.z + sqrtf(3.f);
            pos = glm::vec3(x, y, z);
            InsertTempHex(pos, glm::vec3(1.f, 0.f, 0.f));
            break;
        }
        case 2: // front left
            break;
        case 3: // back left
            break;
        case 4: // back
            break;
        case 5: // back right
            break;
        case 6: // front rihgt
            break;
        }

        // highlight
        m_highlight_vertices[0] = Vertex { pot_hits[closest_ind][0], glm::vec3(0.f, 1.f, 1.f) };
        m_highlight_vertices[1] = Vertex { pot_hits[closest_ind][1], glm::vec3(0.f, 1.f, 1.f) };
        m_highlight_vertices[2] = Vertex { pot_hits[closest_ind][2], glm::vec3(0.f, 1.f, 1.f) };
        m_highlight_vertices[3] = Vertex { pot_hits[closest_ind][3], glm::vec3(0.f, 1.f, 1.f) };

        glBindVertexArray(m_VAO_highlight);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO_highlight);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_highlight_vertices), m_highlight_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO_highlight);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_hightlight_indices), m_hightlight_indices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const void*)offsetof(Vertex, position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const void*)offsetof(Vertex, color));
    }
}

/*void HexaScene::EraseHexagon(int id)
{
    auto it = map_hexes.find(id);
    if (it == map_hexes.end()) return;

    int vbo_offset = it->second->vbo_offset;
    fprintf(stderr, "size before %d %d\n", (int)m_vertices.size(), (int)m_indices.size());
    m_vertices.erase(m_vertices.begin() + 60, m_vertices.begin() + 60 + 12);
    m_indices.erase(m_indices.begin() + 180, m_indices.begin() + 180 + 36);
    m_vertices.shrink_to_fit();
    m_indices.shrink_to_fit();
    fprintf(stderr, "size after %d %d\n", (int)m_vertices.size(), (int)m_indices.size());

    map_hexes.erase(id);

    glBindVertexArray(VAO());

    glBindBuffer(GL_ARRAY_BUFFER, VBO());
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const void*)offsetof(Vertex, color));
}*/

glm::vec2 WorldToScreen(glm::mat4 projection, glm::mat4 model, glm::vec3 worldcoords, int screenWidth, int screenHeight)
{
    /*
    1. with homogeneous = P . M . world, the homogeneous vector is a 4-d vector in homogeneous space.
    In particular, to bring it back to the [-1:1]^3 cube, you need to divide its first 3 coordinates by
    homogeneous.w. cube.x = homogeneous.x / homogeneous.w (what some people call the projection divide).
    2. You then need to transform the [-1:1]^3 cube to window coordinates by applying the Viewport
    transformation to it. window.x = viewport.x + viewport.width * (cube.x+1)/2
    */
    glm::vec4 clipSpacePos = projection * (model * glm::vec4(worldcoords, 1.f));
    glm::vec3 ndcSpacePos = glm::vec3(
        clipSpacePos.x / clipSpacePos.w,
        clipSpacePos.y / clipSpacePos.w,
        clipSpacePos.z / clipSpacePos.w);
    glm::vec2 vec;
    vec.x = (float)screenWidth * (ndcSpacePos.x + 1.f) / 2.f;
    vec.y = (float)screenHeight * (1.f - ndcSpacePos.y) / 2.f;
    return vec;
}

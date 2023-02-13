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
}

void HexaScene::_render()
{
    glBindVertexArray(m_VAO_highlight);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO_highlight);
    glDrawElements(GL_LINES, sizeof(m_hightlight_indices), GL_UNSIGNED_INT, (const void*)0);
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

    switch (rotation)
    {
    case 0:
        // No rotation
        break;
    case 1:
        // Rotate back
        rotateHexAroundSomePoint(hexVertices, rotate, glm::vec3(x, y - h, z - d), glm::vec3(1.f, 0.f, 0.f));
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

void HexaScene::CheckIntersections(glm::mat4 projection, glm::mat4 model, glm::vec3 camPos, int width, int height, int mouse_x, int mouse_y)
{
    float min = -INFINITY;
    Hexagon* hex_min = nullptr;
    std::vector<std::vector<glm::vec3>> pot_hits;
    int closest_ind = -1;
    for (auto& hex : hexes)
    {
        int vbo_index = hex->vbo_offset;

        std::vector<glm::vec2> face_front = {
            WorldToScreen(projection, model, m_vertices[vbo_index + 1].position, width, height),
            WorldToScreen(projection, model, m_vertices[vbo_index + 7].position, width, height),
            WorldToScreen(projection, model, m_vertices[vbo_index + 8].position, width, height),
            WorldToScreen(projection, model, m_vertices[vbo_index + 2].position, width, height)
        };

        // determine if point is inside bounds of face_front
        if (mouse_x < face_front[1].x && mouse_x > face_front[2].x &&
            mouse_y < face_front[0].y && mouse_y > face_front[1].y)
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
        }

        std::vector<glm::vec2> face_front_left = {
            WorldToScreen(projection, model, m_vertices[vbo_index + 2].position, width, height),
            WorldToScreen(projection, model, m_vertices[vbo_index + 8].position, width, height),
            WorldToScreen(projection, model, m_vertices[vbo_index + 9].position, width, height),
            WorldToScreen(projection, model, m_vertices[vbo_index + 3].position, width, height)
        };
        if (mouse_x < face_front_left[1].x && mouse_x > face_front_left[2].x &&
            mouse_y < face_front_left[0].y && mouse_y > face_front_left[1].y)
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
        }

        std::vector<glm::vec2> face_front_right = {
            WorldToScreen(projection, model, m_vertices[vbo_index + 0].position, width, height),
            WorldToScreen(projection, model, m_vertices[vbo_index + 6].position, width, height),
            WorldToScreen(projection, model, m_vertices[vbo_index + 7].position, width, height),
            WorldToScreen(projection, model, m_vertices[vbo_index + 1].position, width, height)
        };
        if (mouse_x < face_front_right[1].x && mouse_x > face_front_right[2].x &&
            mouse_y < face_front_right[0].y && mouse_y > face_front_right[1].y)
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
        }
    }
    if (closest_ind > -1)
    {
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

        /*int vbo_index = hex_min->vbo_offset;
        m_vertices[vbo_index + 1].color = glm::vec3(0.f, 1.f, 1.f);
        m_vertices[vbo_index + 7].color = glm::vec3(0.f, 1.f, 1.f);
        m_vertices[vbo_index + 8].color = glm::vec3(0.f, 1.f, 1.f);
        m_vertices[vbo_index + 2].color = glm::vec3(0.f, 1.f, 1.f);
        glBindVertexArray(VAO());
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO());
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);
    }*/
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

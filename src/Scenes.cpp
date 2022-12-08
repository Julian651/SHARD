#include <Scenes.h>

void HexaScene::_init()
{
   glGenVertexArrays(1, &m_VAO);

   Line::Initialize();
   Cube::Initialize();
   Hexagon::Initialize();

   GLBind(Line, HexaScene);
   GLBind(Cube, HexaScene);
   GLBind(Hexagon, HexaScene);
}

void HexaScene::_render(ShaderProgram& program, glm::mat4 projection, glm::mat4 view)
{
   Cube bbox = Cube(glm::vec3(0.f), 25);
   bbox.Render(program, projection, view);

   for (auto& hex : hexes)
   {
      hex.Render(program, projection, view);
   }
   l.Render(program, projection, view);
}

void HexaScene::_destroy()
{
   glDeleteVertexArrays(1, &m_VAO);

   Line::Destroy();
   Cube::Destroy();
   Hexagon::Destroy();
}

void HexaScene::AddHexagon(glm::vec3 pos)
{
   hexes.push_back(Hexagon(pos));
}

void HexaScene::CheckIntersections(glm::vec3 camPos, glm::vec3 ray)
{
   for (auto& hex : hexes)
   {
      glm::vec3 pos = hex.Position();
      glm::vec3 d = camPos - pos;
      GLfloat len = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);

      for (float i = 0.f; i <= len; i += 0.01f)
      {
         glm::vec3 point = glm::vec3((camPos.x + ray.x * i), (camPos.y + ray.y * i), (camPos.z + ray.z * i));
         float xbound = fabsf(point.x - pos.x);
         float ybound = fabsf(point.y - pos.y);
         float zbound = fabsf(point.z - pos.z);

         float hexa_x = 150.f / 150.f;
         float hexa_y = 57.f / 150.f;
         float hexa_z = 75.f * sqrtf(3.f) / 150.f;

         bool isInside = (zbound <= hexa_z) && (hexa_z * xbound + (hexa_x / 2.f) * zbound <= hexa_x * hexa_z) && (ybound <= hexa_y);

         if (isInside)
         {
            fprintf(stderr, "within hex\n\n");
            l.Move(camPos + glm::vec3(0.f, 0.f, -0.01f), ray, i);
            break;
         }
      }


      /*// get one closes to the camera in the z direction
      glm::vec3 pos = hex.Position();
      glm::vec3 d = camPos - pos;
      GLfloat len = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);

      for (float i = 0.f; i <= len; i += 0.01f)
      {
         glm::vec3 point = glm::vec3((camPos.x + ray.x * i), (camPos.y + ray.y * i), (camPos.z + ray.z * i));
         float xbound = fabsf(point.x - pos.x);
         float ybound = fabsf(point.y - pos.y);
         float zbound = fabsf(point.z - pos.z);
         if (xbound <= 1.f && ybound <= 0.38f && zbound <= 0.866f)
         {
            fprintf(stderr, "within box\n");

            if (xbound <= 0.5f)
            {
               fprintf(stderr, "within hex\n");
               l.Move(camPos + glm::vec3(0.f, 0.f, -0.01f), ray, i);
               break;
            }
            else
            {

            }

            fprintf(stderr, "\n");
         }
      }*/
   }
}
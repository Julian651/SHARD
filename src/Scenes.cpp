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
      hex->hexa.Render(program, projection, view);
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
   HexaNode* hex = new HexaNode{ Hexagon(pos) };
   hexes.push_back(hex);
}

void HexaScene::CheckIntersections(glm::vec3 camPos, glm::vec3 ray)
{
   for (auto& hex : hexes)
   {
      glm::vec3 pos = hex->hexa.Position();
      glm::vec3 d = camPos - pos;
      GLfloat len = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);

      float hexa_x = 150.f / 150.f;
      float hexa_y = 57.f / 150.f;
      float hexa_z = 75.f * sqrtf(3.f) / 150.f;

      for (float i = 0.f; i <= len; i += 0.0078125f)
      {
         glm::vec3 point = glm::vec3((camPos.x + ray.x * i), (camPos.y + ray.y * i), (camPos.z + ray.z * i));
         float xbound = fabsf(point.x - pos.x);
         float ybound = fabsf(point.y - pos.y);
         float zbound = fabsf(point.z - pos.z);

         bool isInside = (zbound <= hexa_z) && (hexa_z * xbound + (hexa_x / 2.f) * zbound <= hexa_x * hexa_z) && (ybound <= hexa_y);

         if (isInside)
         {
            //fprintf(stderr, "within hex (%.3f %.3f %.3f)\n\n", point.x, point.y, point.z);
            l.Move(camPos + glm::vec3(0.f, 0.f, -0.01f), ray, i);

            // try to find out which face it is on
            if (fabsf(point.y) > 0.35)
            {
               if (point.y < 0)
               {
                  // bottom
                  fprintf(stderr, "8\n");
               }
               else
               {
                  // top
                  fprintf(stderr, "7\n");
               }
            }
            else if (fabsf(point.x) < fabsf(pos.x) + 0.5f)
            {
               if (point.z < pos.z && !hex->back)
               {
                  // back
                  fprintf(stderr, "4\n");
                  HexaNode* nnode = new HexaNode{ Hexagon(glm::vec3(pos.x, pos.y, pos.z - hexa_z * 2)) };
                  hex->back = nnode;
                  nnode->front = hex;
                  hexes.push_back(nnode);

                  nnode->fright = hex->bright;
                  nnode->fleft = hex->bleft;
               }
               else if (point.z > pos.z && !hex->front)
               {
                  // front
                  fprintf(stderr, "1\n");
                  HexaNode* nnode = new HexaNode{ Hexagon(glm::vec3(pos.x, pos.y, pos.z + hexa_z * 2)) };
                  hex->front = nnode;
                  nnode->back = hex;
                  hexes.push_back(nnode);

                  nnode->bleft = hex->fleft;
                  nnode->bright = hex->fright;
                  if (nnode->bleft) nnode->fleft = nnode->bleft->front;
                  if (nnode->fleft) nnode->front = nnode->fleft->fright;
               }
               
            }
            else if (point.x < pos.x - 0.5f)
            {
               if (point.z < pos.z && !hex->bleft)
               {
                  // back left
                  fprintf(stderr, "5\n");
                  HexaNode* nnode = new HexaNode{ Hexagon(glm::vec3(pos.x - hexa_x - hexa_x / 2.f, pos.y, pos.z - hexa_z)) };
                  hex->bleft = nnode;
                  nnode->fright = hex;
                  hexes.push_back(nnode);
               }
               else if (point.z > pos.z && !hex->fleft)
               {
                  // front left
                  fprintf(stderr, "6\n");
                  HexaNode* nnode = new HexaNode{ Hexagon(glm::vec3(pos.x - hexa_x - hexa_x / 2.f, pos.y, pos.z + hexa_z)) };
                  hex->fleft = nnode;
                  nnode->bright = hex;
                  hexes.push_back(nnode);
               }
            }
            else
            {
               if (point.z < pos.z && !hex->bright)
               {
                  // back right
                  fprintf(stderr, "3\n");
                  HexaNode* nnode = new HexaNode{ Hexagon(glm::vec3(pos.x + hexa_x + hexa_x / 2.f, pos.y, pos.z - hexa_z)) };
                  hex->fleft = nnode;
                  nnode->bright = hex;
                  hexes.push_back(nnode);
               }
               else if (point.z > pos.z && !hex->fright)
               {
                  // front right
                  fprintf(stderr, "2\n");
                  HexaNode* nnode = new HexaNode{ Hexagon(glm::vec3(pos.x + hexa_x + hexa_x / 2.f, pos.y, pos.z + hexa_z)) };
                  hex->fleft = nnode;
                  nnode->bright = hex;
                  hexes.push_back(nnode);
               }
            }

            return;
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

HexaScene::~HexaScene()
{
   for (auto& hex : hexes)
   {
      delete hex;
   }
}
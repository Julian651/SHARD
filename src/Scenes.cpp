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
            size_t size = hexes.size();

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
            else if (point.x < pos.x + 0.5f && point.x > pos.x - 0.5f)
            {
               if (point.z < pos.z && !hex->back)
               {
                  // back
                  fprintf(stderr, "4\n");
                  HexaNode* nnode = new HexaNode{ Hexagon(glm::vec3(pos.x, pos.y, pos.z - hexa_z * 2)) };

                  hex->back = nnode;
                  nnode->front = hex;
                  nnode->fright = hex->bright;
                  nnode->fleft = hex->bleft;
                  if (nnode->fright)
                  {
                     nnode->bright = nnode->fright->back;
                     nnode->fright->bleft = nnode;
                  }
                  if (nnode->fleft)
                  {
                     nnode->bleft = nnode->fleft->back;
                     nnode->fleft->bright = nnode;
                  }
                  if (nnode->bright)
                  {
                     nnode->back = nnode->bright->bleft;
                     nnode->bright->fleft = nnode;
                  }
                  if (nnode->bleft)
                  {
                     nnode->bleft->fright = nnode;
                  }
                  if (nnode->back)
                  {
                     nnode->back->front = nnode;
                  }


                  hexes.push_back(nnode);
               }
               else if (point.z > pos.z && !hex->front)
               {
                  // front
                  fprintf(stderr, "1\n");
                  HexaNode* nnode = new HexaNode{ Hexagon(glm::vec3(pos.x, pos.y, pos.z + hexa_z * 2)) };

                  hex->front = nnode;
                  nnode->back = hex;
                  nnode->bright = hex->fright;
                  nnode->bleft = hex->fleft;
                  if (nnode->bright)
                  {
                     nnode->fright = nnode->bright->front;
                     nnode->bright->fleft = nnode;
                  }
                  if (nnode->bleft)
                  {
                     nnode->fleft = nnode->bleft->front;
                     nnode->bleft->fright = nnode;
                  }
                  if (nnode->fright)
                  {
                     nnode->front = nnode->fright->fleft;
                     nnode->fright->bleft = nnode;
                  }
                  if (nnode->fleft)
                  {
                     nnode->fleft->bright = nnode;
                  }
                  if (nnode->front)
                  {
                     nnode->front->back = nnode;
                  }

                  hexes.push_back(nnode);
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
                  nnode->front = hex->fleft;
                  nnode->bright = hex->back;
                  if (nnode->bright)
                  {
                     nnode->back = nnode->bright->bleft;
                     nnode->bright->fleft = nnode;
                  }
                  if (nnode->front)
                  {
                     nnode->fleft = nnode->front->bleft;
                     nnode->front->back = nnode;
                  }
                  if (nnode->back)
                  {
                     nnode->bleft = nnode->back->fleft;
                     nnode->back->front = nnode;
                  }
                  if (nnode->bleft)
                  {
                     nnode->bleft->fright = nnode;
                  }
                  if (nnode->fleft)
                  {
                     nnode->fleft->bright = nnode;
                  }

                  hexes.push_back(nnode);
               }
               else if (point.z > pos.z && !hex->fleft)
               {
                  // front left
                  fprintf(stderr, "6\n");
                  HexaNode* nnode = new HexaNode{ Hexagon(glm::vec3(pos.x - hexa_x - hexa_x / 2.f, pos.y, pos.z + hexa_z)) };

                  hex->fleft = nnode;
                  nnode->bright = hex;
                  nnode->back = hex->bleft;
                  nnode->fright = hex->front;
                  if (nnode->fright)
                  {
                     nnode->front = nnode->fright->fleft;
                     nnode->fright->bleft = nnode;
                  }
                  if (nnode->back)
                  {
                     nnode->bleft = nnode->back->fleft;
                     nnode->back->front = nnode;
                  }
                  if (nnode->front)
                  {
                     nnode->fleft = nnode->front->bleft;
                     nnode->front->back = nnode;
                  }
                  if (nnode->bleft)
                  {
                     nnode->bleft->fright = nnode;
                  }
                  if (nnode->fleft)
                  {
                     nnode->fleft->bright = nnode;
                  }

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

                  hex->bright = nnode;
                  nnode->fleft = hex;
                  nnode->front = hex->fright;
                  nnode->bleft = hex->front;
                  if (nnode->bleft)
                  {
                     nnode->back = nnode->bleft->bright;
                     nnode->bleft->fright = nnode;
                  }
                  if (nnode->front)
                  {
                     nnode->fright = nnode->front->bright;
                     nnode->front->back = nnode;
                  }
                  if (nnode->back)
                  {
                     nnode->bright = nnode->back->fright;
                     nnode->back->front = nnode;
                  }
                  if (nnode->fright)
                  {
                     nnode->fright->bleft = nnode;
                  }
                  if (nnode->bright)
                  {
                     nnode->bright->fleft = nnode;
                  }

                  hexes.push_back(nnode);
               }
               else if (point.z > pos.z && !hex->fright)
               {
                  // front right
                  fprintf(stderr, "2\n");
                  HexaNode* nnode = new HexaNode{ Hexagon(glm::vec3(pos.x + hexa_x + hexa_x / 2.f, pos.y, pos.z + hexa_z)) };

                  hex->fright = nnode;
                  nnode->bleft = hex;
                  nnode->back = hex->bright;
                  nnode->fleft = hex->front;
                  if (nnode->fleft)
                  {
                     nnode->front = nnode->fleft->fright;
                     nnode->fleft->bright = nnode;
                  }
                  if (nnode->back)
                  {
                     nnode->bright = nnode->back->fright;
                     nnode->back->front = nnode;
                  }
                  if (nnode->front)
                  {
                     nnode->fright = nnode->front->bright;
                     nnode->front->back = nnode;
                  }
                  if (nnode->bright)
                  {
                     nnode->bright->fleft = nnode;
                  }
                  if (nnode->fright)
                  {
                     nnode->fright->bleft = nnode;
                  }

                  hexes.push_back(nnode);
               }
            }

            for (auto& hex2 : hexes)
            {
               int count = 0;
               if (hex2->back) count++;
               if (hex2->front) count++;
               if (hex2->bleft) count++;
               if (hex2->bright) count++;
               if (hex2->fright) count++;
               if (hex2->fleft) count++;
               fprintf(stderr, "adj: %d\n", count);
            }
            fprintf(stderr, "size: %zd\n", hexes.size());
            if (hexes.size() != size) return;
            else break;
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
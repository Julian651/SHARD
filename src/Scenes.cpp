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
   if (tempNode)
   {
      tempNode->hexa.Render(program, projection, view);
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

bool HexaScene::CheckIntersections(glm::vec3 camPos, glm::vec3 ray)
{
   if (tempNode) return false;
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
                  tempNode = new HexaNode{ Hexagon(glm::vec3(pos.x, pos.y, pos.z - hexa_z * 2)) };

                  hex->back = tempNode;
                  tempNode->front = hex;
                  tempNode->fright = hex->bright;
                  tempNode->fleft = hex->bleft;
                  if (tempNode->fright)
                  {
                     tempNode->bright = tempNode->fright->back;
                     tempNode->fright->bleft = tempNode;
                  }
                  if (tempNode->fleft)
                  {
                     tempNode->bleft = tempNode->fleft->back;
                     tempNode->fleft->bright = tempNode;
                  }
                  if (tempNode->bright)
                  {
                     tempNode->back = tempNode->bright->bleft;
                     tempNode->bright->fleft = tempNode;
                  }
                  if (tempNode->bleft)
                  {
                     tempNode->bleft->fright = tempNode;
                  }
                  if (tempNode->back)
                  {
                     tempNode->back->front = tempNode;
                  }
               }
               else if (point.z > pos.z && !hex->front)
               {
                  // front
                  fprintf(stderr, "1\n");
                  tempNode = new HexaNode{ Hexagon(glm::vec3(pos.x, pos.y, pos.z + hexa_z * 2)) };

                  hex->front = tempNode;
                  tempNode->back = hex;
                  tempNode->bright = hex->fright;
                  tempNode->bleft = hex->fleft;
                  if (tempNode->bright)
                  {
                     tempNode->fright = tempNode->bright->front;
                     tempNode->bright->fleft = tempNode;
                  }
                  if (tempNode->bleft)
                  {
                     tempNode->fleft = tempNode->bleft->front;
                     tempNode->bleft->fright = tempNode;
                  }
                  if (tempNode->fright)
                  {
                     tempNode->front = tempNode->fright->fleft;
                     tempNode->fright->bleft = tempNode;
                  }
                  if (tempNode->fleft)
                  {
                     tempNode->fleft->bright = tempNode;
                  }
                  if (tempNode->front)
                  {
                     tempNode->front->back = tempNode;
                  }
               }
               
            }
            else if (point.x < pos.x - 0.5f)
            {
               if (point.z < pos.z && !hex->bleft)
               {
                  // back left
                  fprintf(stderr, "5\n");
                  tempNode = new HexaNode{ Hexagon(glm::vec3(pos.x - hexa_x - hexa_x / 2.f, pos.y, pos.z - hexa_z)) };

                  hex->bleft = tempNode;
                  tempNode->fright = hex;
                  tempNode->front = hex->fleft;
                  tempNode->bright = hex->back;
                  if (tempNode->bright)
                  {
                     tempNode->back = tempNode->bright->bleft;
                     tempNode->bright->fleft = tempNode;
                  }
                  if (tempNode->front)
                  {
                     tempNode->fleft = tempNode->front->bleft;
                     tempNode->front->back = tempNode;
                  }
                  if (tempNode->back)
                  {
                     tempNode->bleft = tempNode->back->fleft;
                     tempNode->back->front = tempNode;
                  }
                  if (tempNode->bleft)
                  {
                     tempNode->bleft->fright = tempNode;
                  }
                  if (tempNode->fleft)
                  {
                     tempNode->fleft->bright = tempNode;
                  }
               }
               else if (point.z > pos.z && !hex->fleft)
               {
                  // front left
                  fprintf(stderr, "6\n");
                  tempNode = new HexaNode{ Hexagon(glm::vec3(pos.x - hexa_x - hexa_x / 2.f, pos.y, pos.z + hexa_z)) };

                  hex->fleft = tempNode;
                  tempNode->bright = hex;
                  tempNode->back = hex->bleft;
                  tempNode->fright = hex->front;
                  if (tempNode->fright)
                  {
                     tempNode->front = tempNode->fright->fleft;
                     tempNode->fright->bleft = tempNode;
                  }
                  if (tempNode->back)
                  {
                     tempNode->bleft = tempNode->back->fleft;
                     tempNode->back->front = tempNode;
                  }
                  if (tempNode->front)
                  {
                     tempNode->fleft = tempNode->front->bleft;
                     tempNode->front->back = tempNode;
                  }
                  if (tempNode->bleft)
                  {
                     tempNode->bleft->fright = tempNode;
                  }
                  if (tempNode->fleft)
                  {
                     tempNode->fleft->bright = tempNode;
                  }
               }
            }
            else
            {
               if (point.z < pos.z && !hex->bright)
               {
                  // back right
                  fprintf(stderr, "3\n");
                  tempNode = new HexaNode{ Hexagon(glm::vec3(pos.x + hexa_x + hexa_x / 2.f, pos.y, pos.z - hexa_z)) };

                  hex->bright = tempNode;
                  tempNode->fleft = hex;
                  tempNode->front = hex->fright;
                  tempNode->bleft = hex->front;
                  if (tempNode->bleft)
                  {
                     tempNode->back = tempNode->bleft->bright;
                     tempNode->bleft->fright = tempNode;
                  }
                  if (tempNode->front)
                  {
                     tempNode->fright = tempNode->front->bright;
                     tempNode->front->back = tempNode;
                  }
                  if (tempNode->back)
                  {
                     tempNode->bright = tempNode->back->fright;
                     tempNode->back->front = tempNode;
                  }
                  if (tempNode->fright)
                  {
                     tempNode->fright->bleft = tempNode;
                  }
                  if (tempNode->bright)
                  {
                     tempNode->bright->fleft = tempNode;
                  }
               }
               else if (point.z > pos.z && !hex->fright)
               {
                  // front right
                  fprintf(stderr, "2\n");
                  tempNode = new HexaNode{ Hexagon(glm::vec3(pos.x + hexa_x + hexa_x / 2.f, pos.y, pos.z + hexa_z)) };

                  hex->fright = tempNode;
                  tempNode->bleft = hex;
                  tempNode->back = hex->bright;
                  tempNode->fleft = hex->front;
                  if (tempNode->fleft)
                  {
                     tempNode->front = tempNode->fleft->fright;
                     tempNode->fleft->bright = tempNode;
                  }
                  if (tempNode->back)
                  {
                     tempNode->bright = tempNode->back->fright;
                     tempNode->back->front = tempNode;
                  }
                  if (tempNode->front)
                  {
                     tempNode->fright = tempNode->front->bright;
                     tempNode->front->back = tempNode;
                  }
                  if (tempNode->bright)
                  {
                     tempNode->bright->fleft = tempNode;
                  }
                  if (tempNode->fright)
                  {
                     tempNode->fright->bleft = tempNode;
                  }
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
            if (hexes.size() != size) return false;
            else return true;
         }
      }
   }
   return false;
}

void HexaScene::ConfirmTemp()
{
   if (tempNode)
   {
      fprintf(stderr, "confirmed selcetion");
      hexes.push_back(tempNode);
      tempNode = nullptr;
   }
}

void HexaScene::CancelTemp()
{
   if (!tempNode) return; // sloppy, but i dont know why tempNode is sometimes 0x00000000 (null) when being canceled (it should be a value)
   if (tempNode->back && tempNode->back->front == tempNode) { tempNode->back->front = nullptr;  }
   if (tempNode->front && tempNode->front->back == tempNode) { tempNode->front->back = nullptr; }
   if (tempNode->bleft && tempNode->bleft->fright == tempNode) { tempNode->bleft->fright = nullptr; }
   if (tempNode->bright && tempNode->bright->fleft == tempNode) { tempNode->bright->fleft = nullptr; }
   if (tempNode->fleft && tempNode->fleft->bright == tempNode) { tempNode->fleft->bright = nullptr; }
   if (tempNode->fright && tempNode->fright->bleft == tempNode) { tempNode->fright->bleft = nullptr; }
   delete tempNode;
   tempNode = nullptr;
}

HexaScene::~HexaScene()
{
   for (auto& hex : hexes)
   {
      delete hex;
      hex = nullptr;
   }
   if (tempNode)
   {
      delete tempNode;
      tempNode = nullptr;
   }
}
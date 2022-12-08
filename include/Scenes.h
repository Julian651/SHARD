#pragma once
#include <RenderUtilities.h>
#include <Objects.h>


#define GLBind(theObject, theScene) \
   glBindVertexArray(theScene::VAO()); \
   glBindBuffer(GL_ARRAY_BUFFER, theObject::VBO()); \
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theObject::EBO()); \
   theObject::DeclareAttribPointers(); \
   glBindBuffer(GL_ARRAY_BUFFER, 0); \
   glBindVertexArray(0); \
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)

class HexaScene : public Scene<HexaScene>
{
private:

   friend Scene;

   struct HexaNode
   {
      Hexagon hexa;
      HexaNode* front = nullptr;
      HexaNode* back = nullptr;
      HexaNode* bleft = nullptr;
      HexaNode* fleft = nullptr;
      HexaNode* bright = nullptr;
      HexaNode* fright = nullptr;
      HexaNode* top = nullptr;
      HexaNode* bottom = nullptr;
   };

   std::vector<HexaNode*> hexes;
   Line l;

   static void _init();
   void _render(ShaderProgram& program, glm::mat4 projection, glm::mat4 view);
   static void _destroy();

public:

   void AddHexagon(glm::vec3 pos);
   void CheckIntersections(glm::vec3 camPos, glm::vec3 ray);
   ~HexaScene();
};
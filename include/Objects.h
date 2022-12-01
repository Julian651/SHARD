#pragma once

#include <RenderUtilities.h>

class Hexagon : public Object<Hexagon>
{
private:

   friend Object;

   static void _init();
   static void _destroy();
   static void _declare_attrib_pointers();
   static void _draw();

   glm::mat4 Model() override;

public:

   Hexagon(glm::vec3 pos);
   void GetFace(int face);
};

class Cube : public Object<Cube>
{
private:

   friend Object;

   static void _init();
   static void _destroy();
   static void _declare_attrib_pointers();
   static void _draw();

   glm::mat4 Model() override;

   const int m_size;

public:

   Cube(int size);
};

class Line : public Object<Line>
{
private:

   friend Object;

   static void _init();
   static void _destroy();
   static void _declare_attrib_pointers();
   static void _draw();

   glm::mat4 Model() override;

   glm::vec3 m_rot;

public:

   Line(glm::vec3 pos = glm::vec3(0.f), glm::vec3 rot = glm::vec3(0.f));
   void Move(glm::vec3 pos, glm::vec3 rot);
};
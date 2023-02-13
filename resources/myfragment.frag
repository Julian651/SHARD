#version 330 // Specify which version of GLSL we are using.
precision highp float; // Video card drivers require this line to function properly
in vec3 fragColor;
out vec3 finalColor;
void main()
{
    finalColor = fragColor;
}
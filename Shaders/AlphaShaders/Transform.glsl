#ifndef TRANSFORM_GLSL
#define TRANSFORM_GLSL

struct Transform
{
    mat4 model;
    mat4 view;
    mat4 proj;
    mat4 mvp;
};

#endif // TRANSFORM_GLSL
#ifndef UTILS_GLSL
#define UTILS_GLSL

#define GAMMA 2.2

vec4 ApplyGammaCorrection(float gamma, vec4 color)
{
    vec3 rgbColor = pow(color.rgb, vec3(1.0 / gamma));;
    vec4 rgbaColor = vec4(rgbColor, color.a);

    return rgbaColor;
}

#endif //UTILS_GLSL
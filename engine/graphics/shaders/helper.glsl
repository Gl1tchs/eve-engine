#ifndef HELPER_GLSL_
#define HELPER_GLSL_

vec4 get_output_color(vec4 sampled, vec4 base_color) {
#ifdef OPENGL
  return sampled * vec4(0.31, 0.62, 0.93, 1.0);
#else
  return sampled * base_color;
#endif
}

#endif // HELPER_GLSL_

uniform float square_size;

vec3 fragment(vec3 in_color) {
  // Basic checkerboard
  int x = int(gl_FragCoord.x / square_size);
  int y = int(gl_FragCoord.y / square_size);

  bool is_black = (x + y) % 2 == 0;

  return is_black ? in_color : vec3(0);
}
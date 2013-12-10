#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vmath.h"
#include "astronaut.h"

extern astronaut *player;

void callback_mousepos(GLFWwindow *thiswindow __attribute__((unused)), double x, double y) {
  /// React to mouse position updates
  player->rotate_mouse(Vector2d(x, y));
}

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "astronaut.h"

extern astronaut *player;

void callback_windowresize(GLFWwindow *thiswindow __attribute__((unused)), int newwidth, int newheight) {
  /// Callback for handling window resize events
  player->update_window(Vector2i(newwidth, newheight));

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  player->render_firstperson();                                                 // test render while the window is being dragged
  glfwSwapBuffers(thiswindow);

  //config->mousecentrex = (windowwidth  / 2.0);
  //config->mousecentrey = (windowheight / 2.0);
}

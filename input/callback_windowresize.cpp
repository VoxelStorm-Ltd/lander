#include <GL/glew.h>
#include <GLFW/glfw3.h>

bool extern keeprunning;
GLFWwindow extern *window_main;
int extern windowwidth;
int extern windowheight;


void callback_windowresize_3d(GLFWwindow *thiswindow __attribute__((unused)), int newwidth, int newheight) {
  /// Callback for handling window resize events
  windowwidth  = newwidth;
  windowheight = newheight;
  glViewport(0, 0, windowwidth, windowheight);

  double aspect_ratio = static_cast<double>(windowheight) / windowwidth;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-0.5, 0.5,
            -0.5 * aspect_ratio,
            0.5 * aspect_ratio,
            0.5, 10);
  glMatrixMode(GL_MODELVIEW);

  //config->mousecentrex = (windowwidth  / 2.0);
  //config->mousecentrey = (windowheight / 2.0);
}

void callback_windowresize_2d(GLFWwindow *thiswindow __attribute__((unused)), int newwidth, int newheight) {
  /// Callback for handling window resize events
  windowwidth  = newwidth;
  windowheight = newheight;
  glViewport(0, 0, windowwidth, windowheight);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, windowwidth, 0, windowheight);
  glMatrixMode(GL_MODELVIEW);

  //config->mousecentrex = (windowwidth  / 2.0);
  //config->mousecentrey = (windowheight / 2.0);
}

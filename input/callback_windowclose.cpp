#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

bool extern keeprunning;

void callback_windowclose(GLFWwindow *thiswindow __attribute__((unused))) {
  /// Callback for handling window close events
  std::cout << "Window closed, exiting" << std::endl;
  keeprunning = false;
  //_Exit(EXIT_SUCCESS);
}

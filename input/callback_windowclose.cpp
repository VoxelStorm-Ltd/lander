#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void callback_windowclose(GLFWwindow *thiswindow __attribute__((unused))) {
  /// Callback for handling window close events
  std::cout << "Window closed, exiting" << std::endl;
  _Exit(EXIT_SUCCESS);
}

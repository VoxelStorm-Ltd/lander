#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "astronaut.h"

extern astronaut *player;

void callback_mousebutton(GLFWwindow *thiswindow __attribute__((unused)),
                          int button,
                          int action,
                          int mods __attribute__((unused))) {
  /// React to mouse button presses and releases
  switch(button) {
  case GLFW_MOUSE_BUTTON_1:
    if(action == GLFW_PRESS) {
      player->cursor_activate();
    }
    break;
  case GLFW_MOUSE_BUTTON_2:
    if(action == GLFW_PRESS) {
      player->cursor_menu();
    } else {
      player->cursor_menu_close();
    }
    break;
  case GLFW_MOUSE_BUTTON_3:
    break;
  default:
    if(action == GLFW_PRESS) {
      std::cout << "DEBUG: Detected mouse button press on unbound button id " << button << std::endl;
    }
    break;
  }
}

#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <astronaut.h>

extern astronaut *player;

void callback_scroll(GLFWwindow *thiswindow __attribute__((unused)), double xoffset __attribute__((unused)), double yoffset) {
  /// Apply relevant action for using the mouse scroll
  if(xoffset > 0) {
    // left and right cycle tabs, and disables normal scroll for the duration
    #ifdef DEBUG_INPUTSTORM
      std::cout << "DEBUG: inputstorm: scrolled right " << xoffset << std::endl;
    #endif
  } else if(xoffset < 0) {
    #ifdef DEBUG_INPUTSTORM
      std::cout << "DEBUG: inputstorm: scrolled left " << xoffset << std::endl;
    #endif
  } else {
    // up and down scroll the current window
    #ifdef DEBUG_INPUTSTORM
      std::cout << "DEBUG: inputstorm: scrolled vertically " << yoffset << std::endl;
    #endif
    if(yoffset > 0) {
      double const new_angle = player->fov_angle / 2.0;
      if(new_angle < 5.0) {
        player->update_fov(5.0);
      } else {
        player->update_fov(new_angle);
      }
    } else {
      double const new_angle = player->fov_angle * 2.0;
      if(new_angle > 90.0) {
        player->update_fov(90.0);
      } else {
        player->update_fov(new_angle);
      }
    }
  }
}

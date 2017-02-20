#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include "screenshotstorm.h"
#include "control.h"

bool extern keeprunning;
GLFWwindow extern *window_main;

void callback_key(GLFWwindow *thiswindow __attribute__((unused)),
                  int key,
                  int scancode __attribute__((unused)),
                  int action,
                  int mods __attribute__((unused))) {
  // here are the switch-type controls rather than polled realtime controls
  if(action == GLFW_PRESS) {                                                    // here are all the down-presses we care about
    // these first key catches work in all input modes
    switch(key) {
    case GLFW_KEY_EQUAL:
      break;
    case GLFW_KEY_MINUS:
      break;
    case GLFW_KEY_O:
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);                                // wireframe
      glDisable(GL_LIGHTING);
      break;
    case GLFW_KEY_P:
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);                                // filled
      glEnable(GL_LIGHTING);
      break;
    /*
    case GLFW_KEY_PRINT_SCREEN:   // prt scrn to take screenshots
    case GLFW_KEY_F2:             // F2 to take screenshots
      {
        screenshotstorm screenshot;
        screenshot.capture();
        std::string url = screenshot.upload();    // upload the existing file if saved
        if(url.size() == 0) {
          std::cout << "error: could not get upload URL!" << std::endl;
        } else {
          std::cout << "saved and uploaded to " << url << ", URL copied to clipboard" << std::endl;
          glfwSetClipboardString(window_main, url.c_str());
        }
      }
      break;
    */
    case GLFW_KEY_ESCAPE:                                                       // escape to quit
      keeprunning = false;
      std::cout << "Stop requested..." << std::endl;
      break;
    default:
      break;
    }

    control::test_key(key);
  }
}

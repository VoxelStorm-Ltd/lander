#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include "screenshotstorm.h"

bool extern keeprunning;
GLFWwindow extern *window_main;

//extern double temp;

void callback_key(GLFWwindow *thiswindow __attribute__((unused)),
                  int key,
                  int scancode __attribute__((unused)),
                  int action,
                  int mods) {
  // here are the switch-type controls rather than polled realtime controls
  if(action == GLFW_PRESS) {   // here are all the down-presses we care about
    // these first key catches work in all input modes
    switch(key) {
    case GLFW_KEY_O:
      //temp -= 20;
      break;
    case GLFW_KEY_P:
      //temp += 20;
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
    case GLFW_KEY_ESCAPE:         // escape to quit
      keeprunning = false;
      std::cout << "Stop requested..." << std::endl;
      break;
    default:
      break;
    }
  }
}

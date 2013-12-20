#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "astronaut.h"
#include "spacecraft.h"

extern astronaut *player;

void pollcontrols(GLFWwindow *thiswindow) {
  //double const amount = 10.0;
  double const amount = 1.001;
  double const angle = 1.0;

  if(!player->strappeddown) {   // don't check movement while strapped into a seat
    Vector3d movevector;
    if(glfwGetKey(thiswindow, GLFW_KEY_W) == GLFW_PRESS) {
      movevector.z -= 1.0;
    }
    if(glfwGetKey(thiswindow, GLFW_KEY_S) == GLFW_PRESS) {
      movevector.z += 1.0;
    }
    if(glfwGetKey(thiswindow, GLFW_KEY_A) == GLFW_PRESS) {
      movevector.x -= 1.0;
    }
    if(glfwGetKey(thiswindow, GLFW_KEY_D) == GLFW_PRESS) {
      movevector.x += 1.0;
    }
    if(!(movevector.x == 0.0 &&
         movevector.z == 0.0 &&
         movevector.y == 0.0)) {
      movevector.normalise();
      movevector *= 0.05;
      //movevector = player->rotation.rotMatrix() * movevector;   // the slow way
      //movevector = movevector * player->rotation;               // faster
      //movevector *= player->rotation;                           // fastest
      movevector.rotate(player->rotation);                      // same effect but clearer
      player->position += movevector;
    }
  }

  if(glfwGetKey(thiswindow, GLFW_KEY_UP) == GLFW_PRESS) {
    if(player->vessel_in->target.length() == 0) {
      player->vessel_in->target = Vector3d(0, 0, -1);
    }
    player->vessel_in->target *= amount;
  }
  if(glfwGetKey(thiswindow, GLFW_KEY_DOWN) == GLFW_PRESS) {
    player->vessel_in->target /= amount;
  }
  if(glfwGetKey(thiswindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
    //player->vessel_in->target.rotate(0.0, 0.0, -angle);
    player->vessel_in->target.rotate(0.0, angle, 0.0);
  }
  if(glfwGetKey(thiswindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    //player->vessel_in->target.rotate(0.0, 0.0, angle);
    player->vessel_in->target.rotate(0.0, -angle, 0.0);
  }
  if(glfwGetKey(thiswindow, GLFW_KEY_ENTER) == GLFW_PRESS) {
    player->vessel_in->velocity += player->vessel_in->target;
    //std::cout << "Applying delta V " << player->vessel_in->target.length() << "m/s^2" << std::endl;
  }

}

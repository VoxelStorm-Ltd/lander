#include "region.h"
//#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "corner.h"

region::region()
  : parent_corners{nullptr, nullptr, nullptr},
    inner_corners{nullptr, nullptr, nullptr},
    regions{nullptr, nullptr, nullptr, nullptr} {
  /// Default constructor
}

region::~region() {
  /// Default destructor
}

void region::set_corners(corner *corner1, corner *corner2, corner *corner3) {
  /// simple function to set three corner pointers at once
  parent_corners[0] = corner1;
  parent_corners[1] = corner2;
  parent_corners[2] = corner3;
  //update();
}

void region::subdivide(unsigned int depth) {
  /// Initialise the child regions and set their corners correctly
  for(unsigned int i = 0; i != 3; ++i) {
    if(!inner_corners[i]) {
      inner_corners[i] = new corner;
    }
  }
  for(unsigned int i = 0; i != 4; ++i) {
    if(!regions[i]) {
      regions[i] = new region;
    }
  }

  inner_corners[0]->coords = parent_corners[0]->coords.lerp(0.5, parent_corners[1]->coords);
  inner_corners[1]->coords = parent_corners[0]->coords.lerp(0.5, parent_corners[2]->coords);
  inner_corners[2]->coords = parent_corners[1]->coords.lerp(0.5, parent_corners[2]->coords);
  // vertical offsetting
  double radius = parent_corners[1]->coords.length();                           // TODO: get this from planet
  inner_corners[0]->coords.normalise();
  inner_corners[0]->coords *= radius;
  inner_corners[1]->coords.normalise();
  inner_corners[1]->coords *= radius;
  inner_corners[2]->coords.normalise();
  inner_corners[2]->coords *= radius;

  // see https://docs.google.com/drawings/d/1ud3-i8Ua4CfPrwwuEnJJ4xYh_KOTPT-oA6x1aeoe1o0/edit
  // left
  regions[0]->set_corners(parent_corners[0],                                    // left corner
                         inner_corners[0],                                      // lower middle
                         inner_corners[1]);                                     // left middle
  // right
  regions[1]->set_corners(inner_corners[0],                                     // lower middle
                         parent_corners[1],                                     // right corner
                         inner_corners[2]);                                     // right middle
  // top
  regions[2]->set_corners(inner_corners[1],                                     // left middle
                         inner_corners[2],                                      // right middle
                         parent_corners[2]);                                    // top corner
  // middle (rotated 180)
  regions[3]->set_corners(inner_corners[2],                                     // right middle
                         inner_corners[1],                                      // left middle
                         inner_corners[0]);                                     // lower middle

  // recursively subdivide deeper if requested
  if(depth != 0) {
    for(unsigned int i = 0; i != 4; ++i) {
      regions[i]->subdivide(depth - 1);
    }
  }
}

void region::update() {
  Vector3d const line1 = parent_corners[1]->coords - parent_corners[0]->coords;
  Vector3d const line2 = parent_corners[2]->coords - parent_corners[0]->coords;
  normal = line1.crossProduct(line2);
  normal.normalise();

  if(regions[0]) {
    for(unsigned int i = 0; i != 4; ++i) {
      regions[i]->update();
    }
  }
}

void region::render_visible(unsigned int depth) {
  /// Draw this region and all its child regions if appropriate
  if(depth != 0 && regions[0]) {                                                // LOD check placeholder
    regions[0]->render_visible(depth - 1);
    regions[1]->render_visible(depth - 1);
    regions[2]->render_visible(depth - 1);
    regions[3]->render_visible(depth - 1);
  } else {
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.8, 0.8, 0.8, 1.0));
    glMaterialfv(GL_FRONT, GL_SPECULAR,            Vector4f(0.8, 0.8, 0.8, 1.0));
    glMaterialfv(GL_FRONT, GL_EMISSION,            Vector4f(0.0, 0.0, 0.0, 1.0));
    glMaterialf( GL_FRONT, GL_SHININESS,           20.0);                       // 0 to 127

    glBegin(GL_TRIANGLES);
    glNormal3dv(normal);
    //glNormal3dv(parent_corners[0]->coords.normalise_copy());
    glVertex3dv(parent_corners[0]->coords);
    //glNormal3dv(parent_corners[1]->coords.normalise_copy());
    glVertex3dv(parent_corners[1]->coords);
    //glNormal3dv(parent_corners[2]->coords.normalise_copy());
    glVertex3dv(parent_corners[2]->coords);
    glEnd();
  }
}

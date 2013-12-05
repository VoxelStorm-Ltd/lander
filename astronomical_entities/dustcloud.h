#ifndef DUSTCLOUD_H_INCLUDED
#define DUSTCLOUD_H_INCLUDED

#include <string>
#include "body.h"

class dustcloud : public body {
public:
  dustcloud();
  ~dustcloud();

  std::string get_description();
};

#endif // DUSTCLOUD_H_INCLUDED

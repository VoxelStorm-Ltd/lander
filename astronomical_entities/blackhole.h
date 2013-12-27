#ifndef BLACKHOLE_H_INCLUDED
#define BLACKHOLE_H_INCLUDED

#include "body.h"

class blackhole : public body {
public:
  blackhole();
  ~blackhole();

  std::string get_type();
};

#endif // BLACKHOLE_H_INCLUDED

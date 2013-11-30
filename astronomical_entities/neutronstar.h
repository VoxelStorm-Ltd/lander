#ifndef NEUTRONSTAR_H_INCLUDED
#define NEUTRONSTAR_H_INCLUDED

#include "star.h"

class neutronstar : public star {
public:
  virtual std::string get_description();

  neutronstar();
  ~neutronstar();
};

#endif // NEUTRONSTAR_H_INCLUDED

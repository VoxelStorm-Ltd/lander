#ifndef PULSAR_H_INCLUDED
#define PULSAR_H_INCLUDED

#include "neutronstar.h"

class pulsar : public neutronstar {
public:
  double period;                        // the rotational period of this pulsar

  virtual std::string get_description();

  pulsar();
  ~pulsar();
};

#endif // PULSAR_H_INCLUDED

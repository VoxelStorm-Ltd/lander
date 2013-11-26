#ifndef RANDOM_ENGINE_H_INCLUDED
#define RANDOM_ENGINE_H_INCLUDED

#include <random>

class random_engine {
private:
  std::mt19937 generator;
  static const char *const greeknames[];
  unsigned int const greeknames_max;
  static const char *const romannames[];
  unsigned int const romannames_max;

public:
  random_engine();
  ~random_engine();

  // reset the generator to seed
  void random_reset();

  // basic numerical random functions
  bool get_random_bool();
  double get_random_double(double from = 0.0, double to = 1.0);
  int get_random_int(int from = -128, int to = 128);
  unsigned int get_random_uint(unsigned int from = 0, unsigned int to = 255);

  // more advanced semantic random functions
  double get_angle_degrees();
  double get_angle_radians();
  char get_random_char_alpha_upper();
  char get_random_char_alphanum_upper();
  char get_random_char_digit();
  std::string get_random_noun();
  std::string get_random_verb();
  std::string get_random_name_male();
  std::string get_random_name_female();
  std::string get_random_name_neuter();
  std::string get_random_name_surname();
  std::string get_random_name_greek();
  std::string get_random_name_roman();
  std::string get_random_name_ancient();
};

#endif // RANDOM_ENGINE_H_INCLUDED

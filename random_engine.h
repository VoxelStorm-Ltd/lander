#ifndef RANDOM_ENGINE_H_INCLUDED
#define RANDOM_ENGINE_H_INCLUDED

#include <random>

class random_engine {
private:
  std::mt19937 generator;
  // reorder this ugly mess:
  static char const *const greeknames[];
  static char const *const romannames[];
  static char const *const malenames[];
  static char const *const femalenames[];
  static char const *const neuternames[];
  static char const *const surnames[];
  unsigned int static const greeknames_max;
  unsigned int static const romannames_max;
  unsigned int static const malenames_max;
  unsigned int static const femalenames_max;
  unsigned int static const neuternames_max;
  unsigned int static const surnames_max;

  uint32_t seed;              // assigned a value from lastseed on object construction

protected:
  static uint32_t lastseed;   // incremented every time a new child object is created

public:
  random_engine();
  ~random_engine();

  // reset the generator to seed
  void random_reset();

  // basic numerical random functions
  double get_random_double(double from = 0.0, double to = 1.0);
  int get_random_int(int from = -128, int to = 128);
  unsigned int get_random_uint(unsigned int from = 0, unsigned int to = 255);
  bool get_random_bool(double trueprobability = 0.5);

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

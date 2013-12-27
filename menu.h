#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <string>

class menu {
  /// Base class for all classes that have context menus
protected:
  menu();                 // prevent it being instantiated directly
public:
  virtual ~menu();

  virtual std::string get_name();
  virtual std::string get_type();
  virtual std::string get_description();
};

#endif // MENU_H_INCLUDED

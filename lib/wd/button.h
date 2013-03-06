#ifndef BUTTONS_H
#define BUTTONS_H

#include <QPushButton>
#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class Button : public Child
{
  Q_OBJECT

public:
  Button(string n, string s, Form *f, Pane *p);
  void set(string p,string v);
};

#endif

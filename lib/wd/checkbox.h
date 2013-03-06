#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class CheckBox : public Child
{
  Q_OBJECT

public:
  CheckBox(string n, string s, Form *f, Pane *p);
  void set(string p,string v);
  string state();

private slots:
  void stateChanged();

};

#endif

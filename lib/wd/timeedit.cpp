
#include <QTimeEdit>

#include <math.h>

#include "wd.h"
#include "timeedit.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// optional parms are:
// minimum
// single step
// maximum
// value

static void totime(double v, int *h, int *m, int *se, int *mi);

// ---------------------------------------------------------------------
TimeEdit::TimeEdit(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="TimeEdit";
  QTimeEdit *w=new QTimeEdit(p);
  QString qn=s2q(n);
  widget=(QWidget*) w;
  w->setObjectName(qn);
  QStringList opt=qsplit(s);

  int i=0;
  double v;
  int h,m,se,mi;
  if (i<opt.size()) {
    v=c_strtod(q2s(opt.at(i)));
    totime(v, &h, &m, &se, &mi);
    w->setMinimumTime(QTime(h,m,se,mi));
    i++;
  }
  if (i<opt.size()) {
    v=c_strtod(q2s(opt.at(i)));
    totime(v, &h, &m, &se, &mi);
    w->setMaximumTime(QTime(h,m,se,mi));
    i++;
  }
  if (i<opt.size()) {
    v=c_strtod(q2s(opt.at(i)));
    totime(v, &h, &m, &se, &mi);
    w->setTime(QTime(h,m,se,mi));
    i++;
  }
  connect(w,SIGNAL(timeChanged(QTime)),
          this,SLOT(valueChanged()));
}

// ---------------------------------------------------------------------
void TimeEdit::valueChanged()
{
  event="changed";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void TimeEdit::set(string p,string v)
{
  QTimeEdit *w=(QTimeEdit*) widget;
  QString cmd=s2q(p);
  QStringList arg=qsplit(v);
  if (arg.isEmpty()) {
    Child::set(p,v);
    return;
  }
  double i;
  int h,m,se,mi;
  if (cmd=="format") {
    w->setDisplayFormat(s2q(remquotes(v)));
  } else if (cmd=="min") {
    i=c_strtod(q2s(arg.at(0)));
    totime(i, &h, &m, &se, &mi);
    w->setMinimumTime(QTime(h,m,se,mi));
  } else if (cmd=="max") {
    i=c_strtod(q2s(arg.at(0)));
    totime(i, &h, &m, &se, &mi);
    w->setMaximumTime(QTime(h,m,se,mi));
  } else if (cmd=="value") {
    i=c_strtod(q2s(arg.at(0)));
    totime(i, &h, &m, &se, &mi);
    w->setTime(QTime(h,m,se,mi));
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
string TimeEdit::state()
{
  QTimeEdit *w=(QTimeEdit*) widget;
  QTime v=w->time();
  return spair(id,d2s((10000*v.hour())+(100*v.minute())+v.second()+(((double)v.msec())/1000.0)));
}

// ---------------------------------------------------------------------
void totime(double v, int *h, int *m, int *se, int *mi)
{
  int v1=(int)floor(v);
  *h=(int)floor(v1/10000);
  v1=v1%10000;
  *m=(int)floor(v1/100);
  *se=v1%100;
  *mi=(int)floor(1000 * (v-((*h *10000)+(*m *100)+ *se)));
}

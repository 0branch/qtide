
#include <QLabel>
#include <QImage>
#include <QScrollArea>

#include "wd.h"
#include "image.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Image::Image(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="image";

  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  QStringList unopt=qsless(qsless(opt,qsplit("")),defChildStyle);
  if (unopt.size()) {
    error("unrecognized child style: " + q2s(unopt.join(" ")));
    return;
  }

  lab=new QLabel();
  lab->setBackgroundRole(QPalette::Base);
  lab->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  //lab->setScaledContents(true);
  lab->setContentsMargins(0,0,0,0);

  QScrollArea *w = new QScrollArea;
  widget=(QWidget *) w;
  w->setObjectName(qn);
  childStyle(opt);
  w->setBackgroundRole(QPalette::Dark);
  w->setContentsMargins(0,0,0,0);
  w->setWidget(lab);
}

// ---------------------------------------------------------------------
void Image::set(string p,string v)
{
  if (p=="image") {

    QString s=s2q(remquotes(v));
    if (s.isEmpty()) {
      error("set image needs image filename");
      return;
    }
    QImage image(s);
    if (image.isNull()) {
      error("set image cannot load image " + q2s(s));
      return;
    }

    lab->setPixmap(QPixmap::fromImage(image));
    lab->adjustSize();
  } else Child::set(p,v);
}

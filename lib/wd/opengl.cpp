
#include "opengl.h"
#include "opengl2.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Opengl::Opengl(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type = "opengl";

  QStringList m=s2q(s).split(' ',QString::SkipEmptyParts);
#ifdef QT54
  QSurfaceFormat qglFormat;
#else
  QGLFormat qglFormat;
  qglFormat.setSampleBuffers(true);
#endif
#ifdef QT47
  int l=m.indexOf("version");
  if ((l!=-1) && (l<m.size()-1) && 0!=m.at(l+1).toDouble()) {
    int ver1,ver2;
    QString s=m.at(l+1);
    int d=s.indexOf(".");
    if (d==-1) {
      ver1=s.toInt();
      ver2=0;
    } else {
      ver1=s.mid(0,d).toInt();
      ver2=s.mid(d+1).toInt();
    }
//    qDebug() << QString::number(ver1) << QString::number(ver2);
    qglFormat.setVersion(ver1,ver2);
  }
#ifdef QT54
  if (m.contains("compatibility")) qglFormat.setProfile(QSurfaceFormat::CompatibilityProfile);
  else qglFormat.setProfile(QSurfaceFormat::CoreProfile);
#else
  if (m.contains("compatibility")) qglFormat.setProfile(QGLFormat::CompatibilityProfile);
  else qglFormat.setProfile(QGLFormat::CoreProfile);
#endif
#endif
  Opengl2 *w= new Opengl2(this, qglFormat, p);
  widget=(QWidget *) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
  f->opengl = this;
}

// ---------------------------------------------------------------------
void Opengl::setform()
{
  if (!widget) return;
  if (!(event=="paint" || event=="paintz" || event=="resize" || event=="initialize" || event=="print")) form=pform;
  form->opengl=this;
}


#include <QDebug>
#include <QVBoxLayout>

#include "base.h"
#include "dlog.h"
#include "recent.h"
#include "state.h"
#include "tedit.h"
#include "term.h"

using namespace std;

QFile *InputLogFile;

int InputLogMax, InputLogPos;
QStringList InputLog;

void dlog_max();
QStringList makeitems();
QStringList qsreverse(const QStringList list);

// ---------------------------------------------------------------------
Slog::Slog()
{
  setWindowTitle("Input Log");
  QList<int>d=config.winpos_read("Dlog");
#ifdef SMALL_SCREEN
  move(0,0);
  resize(term->width(),term->height());
#else
  resize(qMax(300,d[2]),qMax(300,d[3]));
#endif
  QListWidget *s = new QListWidget();
#ifdef QT_OS_ANDROID
  s->setStyleSheet(scrollbarstyle(config.ScrollBarSize*DM_density));
#endif
  s->addItems(makeitems());
  s->setCurrentRow(InputLog.size()-1);
#ifdef QT_OS_ANDROID
  connect(s, SIGNAL(itemClicked(QListWidgetItem*)),
          this,SLOT(itemActivated()));
  connect(s, SIGNAL(itemClicked(QListWidgetItem*)),
          tedit,SLOT(itemActivated(QListWidgetItem*)));
#else
  connect(s, SIGNAL(itemActivated(QListWidgetItem*)),
          this,SLOT(itemActivated()));
  connect(s, SIGNAL(itemActivated(QListWidgetItem*)),
          tedit,SLOT(itemActivated(QListWidgetItem*)));
#endif
  QVBoxLayout *b = new QVBoxLayout;
  b->setContentsMargins(0,0,0,0);
  b->addWidget(s);
  setLayout(b);
  exec();
}

// ---------------------------------------------------------------------
QStringList makeitems()
{
  QStringList items;
  QString s,t;
  QString p=tedit->getprompt();
  int len=p.size();
  for (int i=0; i<InputLog.size(); i++) {
    t = s = InputLog.value(i);
    t.truncate(len);
    if (p == t)
      s.remove(0,len);
    items.append(s);
  }
  return items;
}

// ---------------------------------------------------------------------
void dlog_add(QString s)
{
  if (s.size() == 0) return;
  int i = InputLog.indexOf(s);
  if (i >= 0)
    InputLog.removeAt(i);
  InputLog.append(s);
  if (config.MaxInputLog>0) cfappend(InputLogFile,s + "\n");
  dlog_max();
}

// ---------------------------------------------------------------------
void dlog_init()
{
  InputLogMax = qMax(config.MaxInputLog,100);
  InputLogPos = 0;
  if (config.MaxInputLog==0) return;
  InputLogFile = new QFile(config.ConfigPath.filePath("inputlog.dat"));
  QString s = cfread(InputLogFile);
  s.remove('\r');
  QStringList t = s.split("\n",QString::SkipEmptyParts);
  t = qsreverse(t);
  t.removeDuplicates();
  InputLog = qsreverse(t);
  dlog_max();
  dlog_write();
}

// ---------------------------------------------------------------------
void dlog_max()
{
  int p = InputLog.size() - InputLogMax;
  if (p > 0)
    for (int i=0; i<p; i++)
      InputLog.removeFirst();
  InputLogPos = InputLog.size();
}

// ---------------------------------------------------------------------
QString dlog_scroll(int m)
{
  int len, pos;
  QString s;
  len = InputLog.size();
  if (len == 0) return "";
  pos = max (0, min(len,InputLogPos + m));
  if (pos == InputLogPos) return "";
  InputLogPos = pos;
  if (InputLogPos == len)
    s = InputLog.at (len - 1);
  else
    s = InputLog.at(InputLogPos);
  return config.DefIndent + s;
}

// ---------------------------------------------------------------------
// write and clean up when session closes
void dlog_write()
{
  if (config.MaxInputLog>0)
    cfwrite(InputLogFile,InputLog.join("\n")+"\n");
}

// ---------------------------------------------------------------------
void Slog::reject()
{
  savepos();
  QDialog::reject();
}

// ---------------------------------------------------------------------
void Slog::itemActivated()
{
  savepos();
  close();
}

// ---------------------------------------------------------------------
void Slog::savepos()
{
  config.winpos_save(this,"Dlog");
}

// ---------------------------------------------------------------------
void Slog::keyReleaseEvent(QKeyEvent *event)
{
#ifdef QT_OS_ANDROID
  if (event->key()==Qt::Key_Back) {
    reject();
  } else QDialog::keyReleaseEvent(event);
#else
  QDialog::keyReleaseEvent(event);
#endif
}

// ---------------------------------------------------------------------
QStringList qsreverse(const QStringList list)
{
  QStringList r;
  for(int i=list.size()-1; i>=0; i--)
    r.append(list.at(i));
  return r;
}

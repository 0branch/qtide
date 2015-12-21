#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QUrl>
#include <QWebView>
#include "child.h"

class Form;
class Pane;
class Qwebview;
class WebViewCom;

// ---------------------------------------------------------------------
class WebView : public Child
{
  Q_OBJECT

public:
  WebView(string n, string s, Form *f, Pane *p);

  void cmd1(QList<QVariant> obj);
  string get(string p,string v);
  void set(string p,string v);
  string state();

  QUrl baseUrl;
  QString cb_name;
  QString cb_value;
  QString curl;
  WebViewCom *qcom;

private slots:
  void addJavaScriptObject();
  void urlChanged (const QUrl & url);

private:
  void cmd_callback(QList<QVariant> obj);
};

// ---------------------------------------------------------------------
class WebViewCom : public QObject
{
  Q_OBJECT

public:
  WebViewCom(WebView *c,QWidget *parent = 0);
  WebView *pchild;

public slots:
  void cmd(QList<QVariant> obj);
};

// ---------------------------------------------------------------------
class Qwebview : public QWebView
{
  Q_OBJECT

public:
  Qwebview(Child *c, QWidget *p=0);

protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void focusInEvent(QFocusEvent *event);
  void focusOutEvent(QFocusEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void wheelEvent(QWheelEvent *event);

private:
  void buttonEvent(QEvent::Type type, QMouseEvent *event);
  Child *pchild;
};

#endif

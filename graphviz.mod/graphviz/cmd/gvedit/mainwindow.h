/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * https://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

/*
Core structures of mdi windowing code is either inspired or directly copied from
Nokia Corporation's QT Toolkit examples. These examples are published under the
terms of the BSD
*/

#pragma once

#include "ui_settings.h"
#include <QMainWindow>
#include <QTextStream>
#include <vector>

extern QTextStream errout;

class MdiChild;
class CFrmSettings;
QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
QT_END_NAMESPACE

class CMainWindow : public QMainWindow {
  Q_OBJECT

public:
  CMainWindow(const QStringList &files);
  QMdiArea *mdiArea;
  void addFile(const QString &fileName);
private slots:
  void slotSettings();
  void slotRun(MdiChild *m = nullptr);
  void slotNew();
  void slotOpen();
  void slotSave();
  void slotSaveAs();
  void slotCut();
  void slotCopy();
  void slotPaste();
  void slotAbout();
  void slotRefreshMenus();
  void slotNewLog();
  void slotSaveLog();
  MdiChild *createMdiChild();
  void activateChild(QWidget *window);

protected:
  void closeEvent(QCloseEvent *event);

private:
  void setChild();
  void createConsole();
  void actions();
  void menus();
  void updateMenus();
  void updateWindowMenu();
  void updateFileMenu();
  void toolBars();
  void readSettings();
  void writeSettings();
  MdiChild *activeMdiChild();
  MdiChild *prevChild;
  QMdiSubWindow *findMdiChild(const QString &fileName);
  int dfltRenderIdx, dfltLayoutIdx;

  QToolBar *tbFile;
  QToolBar *tbEdit;
  QToolBar *tbGraph;

  QAction *newAct;
  QAction *openAct;
  QAction *saveAct;
  QAction *saveAsAct;
  QAction *exitAct;
  QAction *cutAct;
  QAction *copyAct;
  QAction *pasteAct;
  QAction *closeAct;
  QAction *closeAllAct;
  QAction *tileAct;
  QAction *cascadeAct;
  QAction *nextAct;
  QAction *previousAct;
  QAction *separatorAct;
  QAction *aboutAct;

  QAction *settingsAct;
  QAction *layoutAct;

  CFrmSettings *frmSettings;
  // menus
  QMenu *mFile;
  QMenu *mEdit;
  QMenu *mGraph;
  QMenu *mWindow;
  QMenu *mHelp;
};

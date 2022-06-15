//
// Created by PoteznyDominator on 22.05.2022.
//

#ifndef QEDITIOR_MAINWINDOW_H
#define QEDITIOR_MAINWINDOW_H

#include "SideBar.h"
#include <QMainWindow>
#include <QSplitter>
#include <QStackedWidget>
#include <QTreeView>

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit MainWindow(QWidget* parent = nullptr);

private:
  QStackedWidget* stackedWidget_;
  QTabWidget* tabWidget_;
  SideBar* sideBar_;
  QWidget* welcomeWidget_;
  QSplitter* splitter_;

  void initUI();
  void initWelcomeWidget();
  void initTabWidget();
  void initSideBar();
  void initMenuBar();

  void openFile();
  void openFile(const QString& filePath);
  void newFile();
  void saveFile();
  void saveFileAs();
  void zoomIn();
  void zoomOut();
  void executeSavingFile(const QString& filePath);
  void addFileToTabWidget(const QString& filePath,
                          const QString& fileContent = "");
  void closeTab(int index);

  void showSideBar(bool checked);

  QAction* createAction(const QString& text, const QKeySequence& = 0);
  QAction* createAction(const QString& text, void (MainWindow::*slot)(),
                        const QKeySequence& shortCut = 0);
  QAction* createAction(const QString& text, void (MainWindow::*slot)(bool),
                        bool checked, const QKeySequence& shortCut = 0);
};


#endif//QEDITIOR_MAINWINDOW_H
